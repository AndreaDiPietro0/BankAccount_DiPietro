#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

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
    // Prende il valore dalla SpinBox (assicurati che in QtDesigner si chiami spinBoxImporto)
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

void MainWindow::aggiornaInterfaccia() {
    // Aggiorna la Label (assicurati che si chiami labelSaldo)
    ui->labelSaldo->setText("Saldo: " + QString::number(myAccount->getBalance()) + " EUR");
}