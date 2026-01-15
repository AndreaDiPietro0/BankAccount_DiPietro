#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>  // scrivere testo nel file
#include <vector>
#include "Transaction.h"
#include "transferdialog.h"

MainWindow::MainWindow(QString nome, double saldo, QString iban, QString filePath, QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBoxImporto->setMaximum(10000000.0);

    if (nome.isEmpty()) nome = "Guest User";

    myAccount = new BankAccount(nome.toStdString(), saldo, iban.toStdString());

    // se ho un percorso file, lo imposta e carica storico
    if (!filePath.isEmpty()) {
        myAccount->setFilePath(filePath.toStdString());

        // carica storico dal file
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            // salto le prime 3 righe
            in.readLine(); in.readLine(); in.readLine();

            // carca storico
            QString marker = in.readLine();
            if (marker == "---STORICO---") {
                while (!in.atEnd()) {
                    QString riga = in.readLine();
                    QStringList parti = riga.split("|");
                    if (parti.size() >= 3) {
                        int tipoInt = parti[0].toInt();
                        double importo = parti[1].toDouble();
                        QString descrizione = parti[2];
                        Transaction::Type tipo = (tipoInt == 0) ? Transaction::INCOME : Transaction::EXPENSE;
                        myAccount->loadTransactionFromDB(Transaction(importo, tipo, descrizione.toStdString()));
                    }
                }
            }
            file.close();
        }
    }

    allAccounts.push_back(myAccount);

    ui->labelIban->setText("IBAN: " + QString::fromStdString(myAccount->getIban()));
    aggiornaInterfaccia();
}

MainWindow::~MainWindow() {
    for (BankAccount* account : allAccounts) {
        delete account;
    }
    allAccounts.clear(); //svuota la lista

    delete ui;
}

void MainWindow::on_btnDeposita_clicked() {
    double importo = ui->spinBoxImporto->value(); // prende il valore dalla spinbox
    myAccount->deposit(importo);
    aggiornaInterfaccia();
}

void MainWindow::on_btnPreleva_clicked() {
    double importo = ui->spinBoxImporto->value();
    if (!myAccount->withdraw(importo)) {
        QMessageBox::warning(this, "Errore", "Fondi insufficienti!"); // il messaggio èmostrato in una nuova finestra
    }
    aggiornaInterfaccia();
}

void MainWindow::on_btnSave_clicked() {
    //chiede dove salvare
    QString fileName = QFileDialog::getSaveFileName(this, "Salva Conto", "",
                                                    "Text Files (*.txt)"); // this perchè si apre una finestra figlia
    if (fileName.isEmpty()) return; // se l'utente preme annulla

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { //file sono x scrittura | tratta file di testo
        QMessageBox::critical(this, "Errore", "Impossibile salvare il file");
        return;
    }

    QTextStream out(&file);// scrive i dati nel file

    // dati intestazione
    out << QString::fromStdString(myAccount->getOwnerName()) << "\n";
    out << myAccount->getBalance() << "\n";
    out << QString::fromStdString(myAccount->getIban()) << "\n";

    // marcatore
    out << "---STORICO---" << "\n";

    // loop transazioni
    // Formato riga: TIPO|IMPORTO|DESCRIZIONE
    for (const auto &t: myAccount->getTransactionHistory()) {
        out << t.getType() << "|"
            << t.getAmount() << "|"
            << QString::fromStdString(t.getDescription()) << "\n";
    }

    file.close();
    QMessageBox::information(this, "Successo", "Conto e storico salvati correttamente");
}

void MainWindow::aggiornaInterfaccia() {

    ui->labelSaldo->setText("Saldo: " + QString::number(myAccount->getBalance()) + " EURO"); //aggiorna label saldo
    ui->listTransactions->clear(); //pulisce x non avere duplicati
    ui->labelIban->setText("IBAN: " + QString::fromStdString(myAccount->getIban()));
    std::vector<Transaction> history = myAccount->getTransactionHistory(); // prende lo storico da bank account

    //iteratore inverso per mostrare prima le più recenti
    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        const Transaction &t = *it; // prende la transazione corrente

        //mostra + o -
        QString simbolo;
        Qt::GlobalColor colore;

        //controlla il tipo
        if (t.getType() == Transaction::EXPENSE) {
            simbolo = "[-]";
            colore = Qt::red;
        } else {
            simbolo = "[+]";
            colore = Qt::darkGreen;
        }

        //qAbs mostra numero sempre positivo, il segno lo mette il simbolo sopra
        QString testo = simbolo + " " +
                        QString::number(std::abs(t.getAmount())) + " EURO | " +
                        QString::fromStdString(t.getDescription());

        //crea lista che verrà mostrata
        QListWidgetItem *item = new QListWidgetItem(testo);
        item->setForeground(colore);

        ui->listTransactions->addItem(item); // aggiunge la riga alla lista

    }
}

void MainWindow::on_btnBonifico_clicked() {
    TransferDialog dialog(this);

    dialog.setContacts(&allAccounts, myAccount->getIban());     //passa lista di tutti i conti

    if (dialog.exec() == QDialog::Accepted) { // mostra finestra e aspetta

        // se premo ok recupera i dati
        double importo = dialog.getAmount();
        QString destNome = dialog.getName();
        QString destIban = dialog.getIban();

        // cerca destinatario interno
        BankAccount* contoDestinatario = nullptr;
        for (BankAccount* account : allAccounts) {
            if (account->getIban() == destIban.toStdString()) {
                contoDestinatario = account;
                break;
            }
        }

        // bonifico
        bool esito = myAccount->transfer(importo, destIban.toStdString(), destNome.toStdString());

        if (esito) {
            if (contoDestinatario != nullptr) {
                // bonifico interno
                std::string desc = "Bonifico da " + myAccount->getOwnerName() + " (IBAN: " + myAccount->getIban() + ")";
                contoDestinatario->deposit(importo, desc);
                QString pathAmico = QString::fromStdString(contoDestinatario->getFilePath());

                if (!pathAmico.isEmpty()) {
                    QFile fileAmico(pathAmico);
                    if (fileAmico.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&fileAmico);
                        // dati base
                        out << QString::fromStdString(contoDestinatario->getOwnerName()) << "\n";
                        out << contoDestinatario->getBalance() << "\n";
                        out << QString::fromStdString(contoDestinatario->getIban()) << "\n";

                        // storico completo
                        out << "---STORICO---" << "\n";
                        for (const auto& t : contoDestinatario->getTransactionHistory()) {
                            out << t.getType() << "|"
                                << t.getAmount() << "|"
                                << QString::fromStdString(t.getDescription()) << "\n";
                        }
                        fileAmico.close();
                    }
                }
                QMessageBox::information(this, "Fatto", "Bonifico interno inviato a " + destNome);
            } else {
                // bonifico esterno
                QMessageBox::information(this, "Fatto", "Bonifico esterno inviato a " + destNome);
            }
        } else {
            QMessageBox::warning(this, "Errore", "Fondi insufficienti");
        }

        aggiornaInterfaccia();
    }
}