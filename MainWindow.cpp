#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>  // scrivere testo nel file

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inizializza un conto di prova
    myAccount = new BankAccount("Utente Mac M1", 1000.0);

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
    // aggiorna la Label
    ui->labelSaldo->setText("Saldo: " + QString::number(myAccount->getBalance()) + " EUR");
    // aggiorna anche il titolo della finestra col nome del proprietario
    this->setWindowTitle("Conto di: " + QString::fromStdString(myAccount->getOwnerName()));
}
