#include <QApplication>
#include "MainWindow.h"
#include "welcomedialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WelcomeDialog welcome;

    //welcome.exec() blocca il programma finché l'utente non clicca accedi
    if (welcome.exec() == QDialog::Accepted) {

        // entrato, apre  banca
        MainWindow w;
        w.show();

        // ciclo principale
        return a.exec();
    }

    // Se chiudo la finestra di benvenuto il programma finisce qui.
    return 0;
}