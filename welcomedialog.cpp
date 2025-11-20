//
// Created by Andrea Di Pietro on 20/11/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WelcomeDialog.h" resolved

#include "WelcomeDialog.h"
#include "ui_WelcomeDialog.h"

WelcomeDialog::WelcomeDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);

    // collega bottone alla funzione "accept" di QDialog, accept() chiude la finestra
    connect(ui->btnEnter, &QPushButton::clicked, this, &WelcomeDialog::accept);
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}
