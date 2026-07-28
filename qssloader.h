#ifndef QSSLOADER_H
#define QSSLOADER_H

#include <QString>

//! Laedt das in qss.qrc eingebettete ProSoundFinder-Stylesheet (":/qss/default.qss")
//! und wendet dessen LESS-artige $variable-Ersetzung an. Eigenstaendige Kopie fuer
//! PSNPlayerTestNew, unabhaengig von ProSoundFinders StyleSheetLoader/AbstractLoader
//! (die dortige Klassenkette haengt an QSettings-Persistenz/Dokumentverwaltung, die
//! fuer den reinen Testzweck hier nicht gebraucht wird).
QString loadDefaultStyleSheet();

#endif // QSSLOADER_H
