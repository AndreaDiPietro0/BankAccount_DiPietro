#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>  // scrivere testo nel file
#include <vector>
#include "Transaction.h"

MainWindow::MainWindow(QString nome, double saldo, QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBoxImporto->setMaximum(10000000.0);

    //ui->editDestinatario->setPlaceholderText("Nome Destinatario");
    // nome di default se vuoto
    if(nome.isEmpty()) nome = "Guest User";

    myAccount = new BankAccount(nome.toStdString(), saldo);

    aggiornaInterfaccia();
}

MainWindow::~MainWindow() {
    delete myAccount;
    delete ui;
}

void MainWindow::on_btnDeposita_clicked() {
    // prende il valore dalla SpinBox
    double importo = ui->spinBoxImporto->value();
    myAccount->deposit(importo);
    aggiornaInterfaccia();
}

void MainWindow::on_btnPreleva_clicked() {
    double importo = ui->spinBoxImporto->value();
    if (!myAccount->withdraw(importo)) {
        QMessageBox::warning(this, "Errore", "Fondi insufficienti!");
    }
    aggiornaInterfaccia();
}

void MainWindow::on_btnSave_clicked() {
    // chiede all'utente dove salvare (apre il Finder)
    QString fileName = QFileDialog::getSaveFileName(this, "Salva Conto", "", "Text Files (*.txt)");

    if (fileName.isEmpty()) return; // se l'utente preme Annulla

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Errore", "Impossibile salvare il file!");
        return;
    }

    // scrive i dati nel file
    QTextStream out(&file);
    // scrive su due righe: prima il nome poi il saldo
    out << QString::fromStdString(myAccount->getOwnerName()) << "\n";
    out << myAccount->getBalance() << "\n";

    file.close();
    QMessageBox::information(this, "Successo", "Dati salvati correttamente!");
}

void MainWindow::on_btnLoad_clicked() {
    // chiede all'utente quale file aprire
    QString fileName = QFileDialog::getOpenFileName(this, "Carica Conto", "", "Text Files (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Errore", "Impossibile aprire il file!");
        return;
    }

    QTextStream in(&file);

    // legge i dati riga per riga
    QString nome = in.readLine();
    QString saldoStr = in.readLine();

    // coverte la stringa del saldo in numero
    bool ok;
    double saldo = saldoStr.toDouble(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Errore", "Il file è corrotto o il formato non è valido.");
        return;
    }

    // sostituisce il vecchio oggetto BankAccount con quello nuovo
    delete myAccount; // cancella quello vecchio dalla memoria
    myAccount = new BankAccount(nome.toStdString(), saldo);

    file.close();
    aggiornaInterfaccia();
    QMessageBox::information(this, "Successo", "Conto caricato!");
}

void MainWindow::aggiornaInterfaccia() {

        ui->labelSaldo->setText("Saldo: " + QString::number(myAccount->getBalance()) + " EUR"); //aggiorna label saldo
        ui->listTransactions->clear(); //pulisce x non avere duplicati

        std::vector<Transaction> history = myAccount->getTransactionHistory(); // prende lo storico da bank account

        //iteratore inverso per mostrare prima le più recenti
        for (auto it = history.rbegin(); it != history.rend(); ++it) {
            const Transaction& t = *it; // prende la transazione corrente

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

            //qAbs per mostra numero sempre positivo, il segno lo mette il simbolo sopra
            QString testo = simbolo + " " +
                            QString::number(std::abs(t.getAmount())) + " EUR | " +
                            QString::fromStdString(t.getDescription());

            //crea lista che verrà mostrata
            QListWidgetItem* item = new QListWidgetItem(testo);
            item->setForeground(colore);

            // aggiunge la riga alla lista
            ui->listTransactions->addItem(item);
        }
}

void MainWindow::on_btnBonifico_clicked() {
    //prende l'importo dalla stessa casella
    double importo = ui->spinBoxImporto->value();

    //prende nome
    QString destinatario = ui->editDestinatario->text();

    //nome non vuoto
    if (destinatario.isEmpty()) {
        QMessageBox::warning(this, "Errore", "Devi inserire il nome del destinatario!");
        return;
    }

    //conto x destinatario
    BankAccount contoDestinatario(destinatario.toStdString(), 0.0);

    //bonifico transfer  TRUE se funziona, FALSE se soldi non bastano
    bool esito = myAccount->transfer(contoDestinatario, importo);

    if (esito) {
        QMessageBox::information(this, "Successo",
                                 "Bonifico di " + QString::number(importo) + "€ inviato a " + destinatario);

        //cancella nome
        ui->editDestinatario->clear();
    } else {
        QMessageBox::warning(this, "Errore", "Fondi insufficienti per il bonifico!");
    }

    // aggiorna saldo
    aggiornaInterfaccia();
}
