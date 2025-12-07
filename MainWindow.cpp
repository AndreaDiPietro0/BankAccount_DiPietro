#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>  // scrivere testo nel file
#include <vector>
#include "Transaction.h"

MainWindow::MainWindow(QString nome, double saldo, QString iban, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->spinBoxImporto->setMaximum(10000000.0);

    if (nome.isEmpty()) nome = "Guest User"; // nome di default se vuoto

    myAccount = new BankAccount(nome.toStdString(), saldo, iban.toStdString());
    ui->labelIban->setText("IBAN: N/D"); // testo predefinito prima del primo aggiornamento
    aggiornaInterfaccia();
}

MainWindow::~MainWindow() {
    delete myAccount;
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

    // sostituisce il vecchio oggetto bankaccount con quello nuovo
    delete myAccount; // cancella quello vecchio dalla memoria
    myAccount = new BankAccount(nome.toStdString(), saldo, ibanLetto.toStdString());

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

    double importo = ui->spinBoxImporto->value();  //prende l'importo dalla stessa casella

    QString destinatarioNome = ui->editDestinatario->text();  //prende nome

    QString destinatarioIban = ui->editIbanDestinatario->text();

    //nome non vuoto
    if (destinatarioNome.isEmpty()) {
        QMessageBox::warning(this, "Errore", "inserisci il nome del destinatario");
        return;
    }

    if (destinatarioIban.isEmpty()) {
        QMessageBox::warning(this, "Errore", "Inserisci l'IBAN del destinatario!");
        return;
    }

    bool esito = myAccount->transfer(importo, destinatarioIban.toStdString(), destinatarioNome.toStdString());

    if (esito) {
        QMessageBox::information(this, "Successo", "Bonifico di " + QString::number(importo, 'f', 2) + " EUR inviato a " + destinatarioNome + " (" + destinatarioIban + ")");

        // Cancella i campi dopo il successo
        ui->editDestinatario->clear();
        ui->editIbanDestinatario->clear();
        ui->spinBoxImporto->setValue(0.0); // Resetta anche l'importo
    } else {
        QMessageBox::warning(this, "Errore Bonifico", "Fondi insufficienti per il bonifico!");
    }
    // aggiorna saldo
    aggiornaInterfaccia();
}
