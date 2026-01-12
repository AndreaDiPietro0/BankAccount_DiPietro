//
// Created by Andrea Di Pietro on 07/12/25.
//
#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include <vector>
#include "BankAccount.h"
#include <QListWidgetItem>
#include <QFileDialog> // per aprire i file
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace Ui {
    class TransferDialog;
}

class TransferDialog : public QDialog
{
Q_OBJECT

public:
    explicit TransferDialog(QWidget *parent = nullptr);
    ~TransferDialog();

    //accetta un ptr al vettore per poterlo modificare
    void setContacts(std::vector<BankAccount*>* accounts, const std::string& myIban);

    double getAmount() const;
    QString getName() const;
    QString getIban() const;

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void on_listContatti_itemClicked(QListWidgetItem *item);

    //per il nuovo bottone
    void on_btnImportContact_clicked();

private:
    Ui::TransferDialog *ui;

    //  var x ricordare dov'è la lista principale
    std::vector<BankAccount*>* mainAccountList;
    std::string myCurrentIban;
};

#endif // TRANSFERDIALOG_H