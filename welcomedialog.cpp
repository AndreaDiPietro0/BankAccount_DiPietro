//
// Created by Andrea Di Pietro on 20/11/25.
//

#include "welcomedialog.h"
#include "ui_WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::WelcomeDialog) {
    ui->setupUi(this);
    ui->spinSaldo->setValue(0.0);

    // collega bottone alla funzione "accept" di QDialog, accept() chiude la finestra
    connect(ui->btnEnter, &QPushButton::clicked, this, &WelcomeDialog::accept);
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
