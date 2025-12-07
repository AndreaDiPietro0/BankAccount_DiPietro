//
// Created by Andrea Di Pietro on 07/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TransferDialog.h" resolved

#include "transferdialog.h"
#include "ui_transferdialog.h"
#include <QMessageBox>

TransferDialog::TransferDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::TransferDialog)
{
    ui->setupUi(this);
}

TransferDialog::~TransferDialog()
{
    delete ui;
}

// Popola la lista dei contatti
void TransferDialog::setContacts(const std::vector<BankAccount*>& accounts, const std::string& myIban) {
    ui->listContatti->clear();

    for (const auto& acc : accounts) { // non mostrare me stesso
        if (acc->getIban() != myIban) {
            QString voce = QString::fromStdString(acc->getOwnerName()) + " (" + QString::fromStdString(acc->getIban()) + ")";
            ui->listContatti->addItem(voce);
        }
    }
}

// se clicco su un nome nella lista autocompila i campi
void TransferDialog::on_listContatti_itemClicked(QListWidgetItem *item)
{
    // il testo è formato "nome (IBAN)"
    QString text = item->text();

    // estrae nome e iban
    int parentesiIndex = text.lastIndexOf('(');

    if (parentesiIndex != -1) {
        QString nome = text.left(parentesiIndex).trimmed();
        QString iban = text.mid(parentesiIndex + 1, text.length() - parentesiIndex - 2); // leva parentesi

        ui->editNome->setText(nome);
        ui->editIban->setText(iban);
    }
}

void TransferDialog::on_btnOk_clicked()
{
    if (ui->spinAmount->value() <= 0) {
        QMessageBox::warning(this, "Errore", "inserisci un importo valido");
        return;
    }
    if (ui->editNome->text().isEmpty() || ui->editIban->text().isEmpty()) {
        QMessageBox::warning(this, "Errore", "compila tutti i campi.");
        return;
    }
    accept();
}

void TransferDialog::on_btnCancel_clicked()
{
    reject(); // chiude finestra rejected
}

double TransferDialog::getAmount() const { return ui->spinAmount->value(); }
QString TransferDialog::getName() const { return ui->editNome->text(); }
QString TransferDialog::getIban() const { return ui->editIban->text(); }