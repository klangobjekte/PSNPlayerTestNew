#include "mainwindow.h"
#include "qssloader.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //! Eigener QSettings-Scope, getrennt von ProSoundFinders "Klangobjekte"/
    //! "ProSoundFinder" (s. main.cpp dort) - verhindert, dass PlayerWidget::
    //! readSettings()/writeSettings() oder MainWindow::loadLastDroppedFile()
    //! versehentlich echte ProSoundFinder-Benutzereinstellungen lesen/ueberschreiben.
    a.setOrganizationName("Klangobjekte");
    a.setApplicationName("PSNPlayerTestNew");
    a.setStyleSheet(loadDefaultStyleSheet());
    MainWindow w;
    w.show();
    return a.exec();
}
