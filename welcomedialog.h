//
// Created by Andrea Di Pietro on 20/11/25.
//

#ifndef BANKACCOUNT_WELCOMEDIALOG_H
#define BANKACCOUNT_WELCOMEDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class WelcomeDialog; }
QT_END_NAMESPACE

class WelcomeDialog : public QDialog {
Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);

    ~WelcomeDialog() override;

    QString getNome() const;
    double getSaldo() const;

private:
    Ui::WelcomeDialog *ui;
};


#endif //BANKACCOUNT_WELCOMEDIALOG_H
