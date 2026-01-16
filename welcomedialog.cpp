//
// Created by Andrea Di Pietro on 20/11/25.
//

#include "welcomedialog.h"
#include "ui_WelcomeDialog.h"
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>

WelcomeDialog::WelcomeDialog(QWidget *parent) : QDialog(parent), ui(new Ui::WelcomeDialog) {
    ui->setupUi(this);
    ui->spinSaldo->setValue(0.0);
    ui->spinSaldo->setSpecialValueText("Saldo iniziale ");

    ui->editIban->setMaxLength(27);
}

WelcomeDialog::~WelcomeDialog() {
    delete ui;
}

QString WelcomeDialog::getNome() const {
    return ui->editNome->text();
}

double WelcomeDialog::getSaldo() const {
    return ui->spinSaldo->value();
}

QString WelcomeDialog::getIban() const {
    return ui->editIban->text();
}

QString WelcomeDialog::getLoadedFilePath() const {
    return loadedFilePath;
}

void WelcomeDialog::on_btnLoadFile_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Carica Conto", "", "Text Files (*.txt)");

    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Errore", "Impossibile aprire il file.");
        return;
    }

    QTextStream in(&file);

    QString nome = in.readLine();
    QString saldoStr = in.readLine();
    QString iban = in.readLine();

    file.close();

    bool ok;
    double saldo = saldoStr.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Errore", "Il file sembra corrotto");
        return;
    }
    ui->editNome->setText(nome);
    ui->spinSaldo->setValue(saldo);
    ui->editIban->setText(iban);

    loadedFilePath = fileName;

    accept();
}

void WelcomeDialog::on_btnEnter_clicked() {
    QString nome = ui->editNome->text();

    // .simplified() -> toglie spazi all'inizio e fine
    // .remove(" ") -> toglie gli spazi in mezzo (se l'utente scrive "IT00 123...")
    // .toUpper() -> trasforma tutto in maiuscolo
    QString iban = ui->editIban->text().simplified().remove(" ").toUpper();

    if (nome.isEmpty() || iban.isEmpty()) {
        QMessageBox::warning(this, "Dati mancanti", "Inserisci Nome e IBAN per continuare.\noppure carica un conto esistente.");
        return;
    }

    if (iban.length() != 27) {
        QString msg = QString("L'IBAN deve essere di 27 caratteri.\n ne hai inseriti: %1").arg(iban.length());
        QMessageBox::warning(this, "Lunghezza IBAN Errata", msg);
        return;
    }

    if (!iban.startsWith("IT")) {
        QMessageBox::warning(this, "Formato Errato", "L'IBAN deve iniziare con 'IT'.");
        return;
    }

    ui->editIban->setText(iban);

    accept();
}