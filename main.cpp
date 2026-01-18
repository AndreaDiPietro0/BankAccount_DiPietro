#include <QApplication>
#include "MainWindow.h"
#include "welcomedialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WelcomeDialog welcome;

    //welcome.exec blocca il programma finché non clicco accedi
    if (welcome.exec() == QDialog::Accepted) {

        // prende dati inseriti
        QString nomeScelto = welcome.getNome();
        double saldoScelto = welcome.getSaldo();
        QString ibanScelto = welcome.getIban();

        QString filePath = welcome.getLoadedFilePath();
        // crea la finestra principale con quei dati
        MainWindow w(nomeScelto, saldoScelto, ibanScelto, filePath);
        w.show();

        return a.exec();
    }

    return 0;
}