#include <QApplication>
#include "MainWindow.h"
#include "welcomedialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WelcomeDialog welcome;

    //welcome.exec() blocca il programma finché l'utente non clicca accedi
    if (welcome.exec() == QDialog::Accepted) {

        // prende dati inseriti
        QString nomeScelto = welcome.getNome();
        double saldoScelto = welcome.getSaldo();
        QString ibanScelto = welcome.getIban();

        // crea la finestra principale con quei dati
        MainWindow w(nomeScelto, saldoScelto, ibanScelto);
        w.show();

        return a.exec();
    }

    // Se chiudo la finestra di benvenuto il programma finisce qui.
    return 0;
}