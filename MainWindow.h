#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BankAccount.h"
#include <vector>
#include <QMessageBox>
#include <QFileDialog>  //  aprire le finestre di salvataggio
#include <QFile>        //  gestire il file fisico
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QString nome, double saldo, QString iban, QString filePath = "", QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnDeposita_clicked();
    void on_btnPreleva_clicked();
    void on_btnBonifico_clicked();

    void on_btnSave_clicked();

private:
    Ui::MainWindow *ui;
    BankAccount* myAccount;
    std::vector<BankAccount*> allAccounts;

    void aggiornaInterfaccia();
};
#endif // MAINWINDOW_H