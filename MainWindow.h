#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BankAccount.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
            void on_btnDeposita_clicked();
    void on_btnPreleva_clicked();

private:
    Ui::MainWindow *ui;
    BankAccount* myAccount;
    void aggiornaInterfaccia();
};
#endif // MAINWINDOW_H