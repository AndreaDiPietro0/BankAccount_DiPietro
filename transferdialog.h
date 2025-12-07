//
// Created by Andrea Di Pietro on 07/12/25.
//
#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include <vector>
#include "BankAccount.h"
#include <QListWidgetItem>

namespace Ui {
    class TransferDialog;
}

class TransferDialog : public QDialog
{
Q_OBJECT

public:
    explicit TransferDialog(QWidget *parent = nullptr);
    ~TransferDialog();

    //passa lista dei conti e il mio iban (per non mostrarmi nella lista)
    void setContacts(const std::vector<BankAccount*>& accounts, const std::string& myIban);

    double getAmount() const;
    QString getName() const;
    QString getIban() const;

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_listContatti_itemClicked(QListWidgetItem *item); // se clicco un contatto

private:
    Ui::TransferDialog *ui;
};

#endif // TRANSFERDIALOG_H
