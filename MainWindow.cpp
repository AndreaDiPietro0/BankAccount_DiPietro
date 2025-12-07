#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>  // scrivere testo nel file
#include <vector>
#include "Transaction.h"
#include "transferdialog.h"

MainWindow::MainWindow(QString nome, double saldo, QString iban, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->spinBoxImporto->setMaximum(10000000.0);

    if (nome.isEmpty()) nome = "Guest User"; // nome di default se vuoto

    myAccount = new BankAccount(nome.toStdString(), saldo, iban.toStdString());

    allAccounts.push_back(myAccount);
    ui->labelIban->setText("IBAN: N/D"); // testo predefinito prima del primo aggiornamento
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
    // chiede all'utente dove salvare
    QString fileName = QFileDialog::getSaveFileName(this, "Salva Conto", "","Text Files (*.txt)"); // this perchè si apre una finestra figlia

    if (fileName.isEmpty()) return; // se l'utente preme annulla

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { //file sono x scrittura | tratta file di testo
        QMessageBox::critical(this, "Errore", "Impossibile salvare il file");
        return;
    }

    QTextStream out(&file); // scrive i dati nel file
    // scrive su due righe prima il nome poi il saldo
    out << QString::fromStdString(myAccount->getOwnerName()) << "\n";
    out << myAccount->getBalance() << "\n";
    out << QString::fromStdString(myAccount->getIban()) << "\n";

    file.close();
    QMessageBox::information(this, "Successo", "Dati salvati correttamente");
}

void MainWindow::on_btnLoad_clicked() {
    // chiede all'utente quale file aprire
    QString fileName = QFileDialog::getOpenFileName(this, "Carica Conto", "", "Text Files (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Errore", "Impossibile aprire il file");
        return;
    }

    QTextStream in(&file);

    QString nome = in.readLine();// legge i dati riga per riga
    QString saldoStr = in.readLine();
    QString ibanLetto = in.readLine();

    // converte la stringa del saldo in numero
    bool ok;
    double saldo = saldoStr.toDouble(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Errore", "Il file è sbagliato o il formato non è valido");
        return;
    }

    for (size_t i = 0; i < allAccounts.size(); ++i) {
        if (allAccounts[i] == myAccount) {  // se trovo il puntatore che corrisponde a myAccount attuale
            delete allAccounts[i]; // cancella vecchio
            myAccount = new BankAccount(nome.toStdString(), saldo, ibanLetto.toStdString()); // crea nuovo oggetto con i dati letti dal file
            allAccounts[i] = myAccount;// aggiorna lista e mette il nuovo in quella posizione

            break;
        }
    }

    file.close();
    aggiornaInterfaccia();
    QMessageBox::information(this, "Successo", "Conto caricato");
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

    dialog.setContacts(allAccounts, myAccount->getIban());     //passa lista di tutti i conti

    if (dialog.exec() == QDialog::Accepted) { // Mostra finestra e aspetta

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

void MainWindow::on_btnImporta_clicked() {
    //chiede quale aprire
    QString fileName = QFileDialog::getOpenFileName(this, "Importa Conto Amico", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Errore", "Impossibile aprire il file");
        return;
    }

    QTextStream in(&file);
    QString nome = in.readLine();
    QString saldoStr = in.readLine();
    QString iban = in.readLine();
    file.close();

    // controllo x duplicati
    for (BankAccount* acc : allAccounts) {
        if (acc->getIban() == iban.toStdString()) {
            QMessageBox::warning(this, "Attenzione", "Questo conto è già stato caricato nel sistema");
            return;
        }
    }

    // crea nuovo conto
    double saldo = saldoStr.toDouble();
    BankAccount* contoAmico = new BankAccount(nome.toStdString(), saldo, iban.toStdString());

    allAccounts.push_back(contoAmico);

    QMessageBox::information(this, "OK", "Conto di " + nome + " importato correttamente\nOra puoi inviare un bonifico.");
}