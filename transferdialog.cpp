//
// Created by Andrea Di Pietro on 07/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TransferDialog.h" resolved
#include "transferdialog.h"
#include "ui_transferdialog.h"

TransferDialog::TransferDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::TransferDialog)
{
    ui->setupUi(this);
    ui->spinAmount->setMaximum(1000000.0);
}

TransferDialog::~TransferDialog()
{
    delete ui;
}

// salvail puntatore e popola la lista
void TransferDialog::setContacts(std::vector<BankAccount*>* accounts, const std::string& myIban) {
    mainAccountList = accounts; // salva riferimento alla lista originale
    myCurrentIban = myIban;

    ui->listContatti->clear();

    // usa *accounts per accedere al vettore puntato
    for (const auto& acc : *accounts) {
        if (acc->getIban() != myIban) {
            QString voce = QString::fromStdString(acc->getOwnerName()) + " (" + QString::fromStdString(acc->getIban()) + ")";
            ui->listContatti->addItem(voce);
        }
    }
}

void TransferDialog::on_listContatti_itemClicked(QListWidgetItem *item)
{
    QString text = item->text();
    int parentesiIndex = text.lastIndexOf('(');

    if (parentesiIndex != -1) {
        QString nome = text.left(parentesiIndex).trimmed();
        QString iban = text.mid(parentesiIndex + 1, text.length() - parentesiIndex - 2);

        ui->editNome->setText(nome);
        ui->editIban->setText(iban);
    }
}

void TransferDialog::on_btnImportContact_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Importa Contatto", "", "Text Files (*.txt)");
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

    // x duplicati
    for (BankAccount* acc : *mainAccountList) {
        if (acc->getIban() == iban.toStdString()) {
            file.close();
            QMessageBox::warning(this, "Attenzione", "Questo contatto è già in lista!");
            return;
        }
    }

    // crea conto
    double saldo = saldoStr.toDouble();
    BankAccount* nuovoContatto = new BankAccount(nome.toStdString(), saldo, iban.toStdString());
    nuovoContatto->setFilePath(fileName.toStdString());

    // importa Storico
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
                nuovoContatto->loadTransactionFromDB(Transaction(importo, tipo, descrizione.toStdString()));
            }
        }
    }
    file.close();

    //aggiunge alla lista principale
    mainAccountList->push_back(nuovoContatto);

    // aggiunge visivamente alla lista in questa finestra
    QString voce = QString::fromStdString(nuovoContatto->getOwnerName()) + " (" + QString::fromStdString(nuovoContatto->getIban()) + ")";
    ui->listContatti->addItem(voce);

    QMessageBox::information(this, "Fatto", "Contatto aggiunto!");
}

void TransferDialog::on_btnOk_clicked() {
    if (ui->spinAmount->value() <= 0) {
        QMessageBox::warning(this, "Errore", "Inserisci un importo valido.");
        return;
    }
    if (ui->editNome->text().isEmpty() || ui->editIban->text().isEmpty()) {
        QMessageBox::warning(this, "Errore", "Compila tutti i campi.");
        return;
    }
    accept();
}

void TransferDialog::on_btnCancel_clicked() { reject(); }
double TransferDialog::getAmount() const { return ui->spinAmount->value(); }
QString TransferDialog::getName() const { return ui->editNome->text(); }
QString TransferDialog::getIban() const { return ui->editIban->text(); }