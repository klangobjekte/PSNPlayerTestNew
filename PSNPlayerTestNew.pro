QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../ProSoundfinder/QtAwesome/QtAwesome.pri)



QT += widgets \
        sql \
        network \
        quick

 CONFIG+=_PLAYERWIDGETV2
DEFINES+=_USE_PLAYERWIDGETV2

#! Wie in ProSoundFinder2.pro (dort: greaterThan(QT_MAJOR_VERSION, 5){ CONFIG +=
#! _QT5COMPAT } / _QT5COMPAT{ DEFINES += _USE_QT5COMPAT; QT += core5compat }) --
#! centralwidget.h (ueber abstractplayerwidget.cpp includiert) braucht diesen
#! Zweig, sonst QRegularExpression::PatternSyntax (existiert in Qt6 nicht mehr).
greaterThan(QT_MAJOR_VERSION, 5){
    DEFINES += _USE_QT5COMPAT
    QT += core5compat
}

SOURCES += \
    qssloader.cpp \
    psndplayertestnew_linkstubs.cpp \
    ../ProSoundFinder/abstractplayerwidget.cpp \
    ../ProSoundFinder/playerwidgetV2.cpp \
    ../ProSoundFinder/timelabel.cpp \
    ../ProSoundFinder/preferencescontrol.cpp \
    ../ProSoundFinder/preferencesdialog.cpp \
    ../ProSoundFinder/psfdefinitions.cpp \
    ../ProSoundFinder/psffiledialoghelper/psffiledialoghelper.cpp \
    ../ProSoundFinder/filenamehelper.cpp \
    ../ProSoundFinder/helperfunctions.cpp \
    ../ProSoundFinder/sndfilecommon.cpp \
    ../ProSoundFinder/sndfilecreatesndfile.cpp \
    main.cpp \
    mainwindow.cpp

#! processhelper.h: gleicher Grund/gleiches Muster wie bei sqldriver.h
#! unten -- nur TRANSITIV ueber helperfunctions.cpp eingebunden, deshalb
#! bisher kein echtes MOC fuer die Q_OBJECT-Klasse ProcessHelper. Das
#! Testprojekt braucht bereits einen No-Op-Stub-Konstruktor/-Destruktor
#! dafuer (s. psndplayertestnew_linkstubs.cpp, von helperfunctions.cpp's
#! askForProjectDir() gebraucht) -- sobald der einmal eine echte Definition
#! bekommt, verlangt MSVC fuer die dabei noetige komplette Vtable auch
#! ProcessHelper::metaObject()/qt_metacast()/qt_metacall(). Nur echtes MOC
#! kann diese drei korrekt erzeugen (Handschreiben ist nicht praktikabel).
#! ProcessHelper selbst hat sonst keine weiteren virtuellen Methoden (nur
#! der von QObject geerbte virtuelle Destruktor), daher kein weiterer
#! Kaskadeneffekt wie bei sqldriver.h unten zu erwarten.
HEADERS += \
    ../ProSoundFinder/processhelper.h

#! sqldriver.h wird nur TRANSITIV ueber customtablemodel.h eingebunden und
#! stand deshalb bisher nicht in HEADERS -- qmake/moc scannt aber NUR
#! explizit hier gelistete Header auf Q_OBJECT. Task/SqlSelectThread/
#! SqlDeleteThread/UpdateFieldInTableThread (alle Q_OBJECT, in sqldriver.h)
#! wurden dadurch nie moc't: CustomTableModel haelt SqlSelectThread/
#! UpdateFieldInTableThread als WERT-Member (customtablemodel.h), was deren
#! Konstruktor/Destruktor -- und wegen der virtuellen run()-Overrides auch
#! die Compile/Link-Pflicht ihrer run()-Bodies -- erzwingt, sobald
#! CustomTableModel irgendwo im gelinkten Code vollstaendig gebraucht wird.
#! Ohne echtes MOC fehlten dafuer metaObject()/qt_metacast()/qt_metacall()
#! (LNK2001) sowie die von run() intern per emit ausgeloesten Signale
#! (selectFinished()/resultReady()/upDatedEntry(), LNK2019) -- s. dazu die
#! neuen No-Op-Stubs fuer die restlichen (nicht von MOC generierten)
#! Symbole in psndplayertestnew_linkstubs.cpp. Rein additiv, keine Aenderung
#! an sqldriver.h selbst (liegt in ProSoundFinder, s. CLAUDE.md).
HEADERS += \
    qssloader.h \
    ../ProSoundFinder/abstractplayerwidget.h \
    ../ProSoundFinder/playerwidgetV2.h \
    ../ProSoundFinder/timelabel.h \
    ../ProSoundFinder/preferencescontrol.h \
    ../ProSoundFinder/preferencesdialog.h \
    ../ProSoundFinder/psfdefinitions.h \
    ../ProSoundFinder/psffiledialoghelper/psffiledialoghelper.h \
    ../ProSoundFinder/filenamehelper.h \
    ../ProSoundFinder/helperfunctions.h \
    ../ProSoundFinder/sndfilecommon.h \
    ../ProSoundFinder/sndfilecreatesndfile.h \
    ../ProSoundFinder/customtablemodel.h \
    ../ProSoundFinder/customsortfilterproxmodel.h \
    ../ProSoundFinder/mimedata.h \
    ../ProSoundFinder/sqldriver.h \
    mainwindow.h

FORMS += \
    mainwindow.ui \
    ../ProSoundFinder/preferencesdialog.ui

RESOURCES += \
    qss.qrc


macx {

INCLUDEPATH += .
INCLUDEPATH += ../ProSoundfinder
INCLUDEPATH += /opt/homebrew/include
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/meta_mp
INCLUDEPATH += /usr/local/include/bmx-0.1
INCLUDEPATH += /usr/local/include/libMXF-1.0
INCLUDEPATH += /usr/local/include/libMXF++-1.0
INCLUDEPATH += /usr/local/include/mOPAtomReader
INCLUDEPATH += /usr/local/include/PSndPlayer
INCLUDEPATH += /usr/local/include/PSndPlayer/BWFExport


#HEADERS += GenLinkedList.h
#HEADERS += FSCopyObject.h

#SOURCES += GenLinkedList.c
#SOURCES += FSCopyObject.c


#OBJECTIVE_SOURCES += SpotToRegion/RegionSpotterOSX.mm
#OBJECTIVE_SOURCES += cocoainitializer.mm
#OBJECTIVE_HEADERS += SpotToRegion/RegionSpotterOSX.h
#OBJECTIVE_HEADERS += cocoainitializer.h




LIBS += -L/usr/local/lib
LIBS += -L/opt/homebrew/lib
LIBS += -framework AppKit
LIBS += -framework Foundation
LIBS += -framework ApplicationServices
LIBS += -dead_strip
LIBS += -lsndfile
LIBS += -lsfmeta
LIBS += -lmpg123
 LIBS += -lPSndPlayer

LIBS += -framework Cocoa
LIBS += -framework CoreServices
LIBS += -framework AudioToolbox
LIBS += -framework Foundation

LIBS += -framework AppKit
LIBS += -framework Foundation
LIBS += -framework ScriptingBridge
LIBS += -framework ApplicationServices
LIBS += -framework CoreFoundation

LIBS += -framework \
    CoreAudio \
    -framework \
    CoreServices \
    -framework \
    Foundation

}


# Windows-ARM64-Build (Parallels/Apple-Silicon-VM, Qt 6.11.1 MSVC2022_ARM64).
# Bislang gab es fuer dieses Projekt nur den macx{}-Block oben -- unter
# Windows schlug der Build komplett fehl (C1083 auf
# "pSndPlayer/psnddefinitions.h" in psfdefinitions.h und auf
# "abstractplayerwidget.h" in mainwindow.cpp), weil kein INCLUDEPATH fuer
# die live aus ../ProSoundFinder mitkompilierten Quellen (s. SOURCES/
# HEADERS oben) bzw. fuer PSndPlayer existierte. Analog zum bereits
# etablierten arm64-Block in PSndPlayer.pro. Rein additiv innerhalb dieses
# Testprojekts -- macOS (macx{} oben) bleibt unangetastet, ProSoundFinder
# und PSndPlayer selbst wurden nicht veraendert.
win32 {
    equals(QT_ARCH, arm64) {
        # ".." = Elternordner von PSNPlayerTestNew/ProSoundFinder/PSndPlayer.
        # Loest "pSndPlayer/psnddefinitions.h" (psfdefinitions.h) sowie
        # <pSndPlayer/psndplayer.h>, <PSndPlayer/mediasource.h>,
        # <pSndPlayer/volumeoverlaylabel.h>, <pSndPlayer/BWFExport/
        # exportbwfwave.h> auf (playerwidgetV2.h/.cpp,
        # abstractplayerwidget.cpp, Export/export.h) -- Windows-Gegenstueck
        # zu macOS' "INCLUDEPATH += /usr/local/include" (dort liegen
        # PSndPlayers installierte Header unter .../include/PSndPlayer/).
        INCLUDEPATH += ..
        # Fuer die bar benannten Includes ("abstractplayerwidget.h",
        # "customtablemodel.h" usw.) in denselben Quellen -- Windows-Pendant
        # zu macOS' "INCLUDEPATH += ../ProSoundfinder" oben.
        INCLUDEPATH += ../ProSoundFinder

        # ProSoundFinder/timelabel.h bindet "<MacTypes.h>" (macOS Carbon/
        # CoreServices-Header) UNGUARDED ein, nur um an "Float64" zu kommen
        # -- anders als z.B. das bereits gefixte PSndPlayer/BWFExport/
        # tracksplitter.h ist dieser Include dort NICHT auf "#ifdef
        # __APPLE__" beschraenkt (echter, noch offener Bug in
        # ProSoundFinder selbst). Da PSNPlayerTestNew laut CLAUDE.md
        # ProSoundFinder nicht veraendern darf, liegt hier stattdessen ein
        # minimaler Ersatz-Header (nur "#define Float64 double", identisch
        # zu PSndPlayer/psnddefinitions.h's Windows-Zweig) unter
        # win_compat/MacTypes.h -- reiner Workaround innerhalb dieses
        # Testprojekts. Der eigentliche Fix (Include dort auf __APPLE__
        # beschraenken) gehoert in ProSoundFinder selbst und muesste vom
        # Nutzer direkt dort vorgenommen werden.
        #
        # Zweitzweck desselben Eintrags: loest zugleich "../aafwidget_test/
        # aafwizardwidget.h" (ProSoundFinder/mainwindow.h, ueber
        # playerwidgetV2.cpp's "#include "mainwindow.h"" bei uns
        # mitgezogen) relativ zu diesem win_compat-Ordner auf --
        # ".../PSNPlayerTestNew/win_compat/../aafwidget_test/..." =
        # ".../PSNPlayerTestNew/aafwidget_test/...". Stub-Datei liegt dort,
        # s. deren eigener Kommentar fuer Details/Hintergrund.
        INCLUDEPATH += win_compat

        # "BWFExport/tracksplitter.h" (PSndPlayer/BWFExport/exportbwfwave.h,
        # eingebunden ueber Export/export.h) ist relativ zur PSndPlayer-
        # Wurzel selbst geschrieben (nicht zu deren Elternordner) -- Windows-
        # Pendant zu macOS' "INCLUDEPATH += /usr/local/include/PSndPlayer"
        # bzw. "...+= /usr/local/include/PSndPlayer/BWFExport" oben.
        INCLUDEPATH += ../PSndPlayer
        INCLUDEPATH += ../PSndPlayer/BWFExport

        # customtablemodel.h -> scanHelper.h -> "coutput.h" (nur als
        # Zeigerglied "COutput *cOutput", keine ODR-nutzung von
        # meta_mp-master-Symbolen an dieser Stelle -- reine Compile-Time-
        # Aufloesung, kein zusaetzliches LIBS-Linking noetig). Windows-
        # Pendant zu macOS' "INCLUDEPATH += /usr/local/include/meta_mp" oben.
        INCLUDEPATH += C:/Users/admin/Developer/meta_mp

        # <boost/shared_ptr.hpp>, <boost/multi_index_container.hpp> (u.a.
        # customtablemodel.h, Export/export.h, databasemodel.h via
        # sqldriver.h) und <sndfile.h>/<sndfile.hh> (sndfilecommon.h,
        # sndfilecreatesndfile.h) kommen wie in PSndPlayer.pro's arm64-Block
        # per vcpkg (arm64-windows-Triplet).
        VCPKG_INSTALLED = C:/Users/admin/Developer/vcpkg/installed/arm64-windows
        INCLUDEPATH += $$VCPKG_INSTALLED/include

        # C2872 "byte"/windows.h-Falle (s. PSndPlayer/psnddefinitions.h):
        # windows.h wird hier transitiv ueber psnddefinitions.h eingebunden,
        # deshalb dieselbe NOMINMAX-Absicherung wie in PSndPlayer.pro.
        DEFINES += NOMINMAX
        # _BUFFDATATYPE (psnddefinitions.h, auch in BWFExport/tracksplitter.h
        # verwendet) haengt von diesem Define ab -- psnddefinitions.h wird
        # von BEIDEN Projekten eingebunden (PSndPlayer selbst UND hier), muss
        # also mit demselben Define wie in PSndPlayer.pro kompiliert werden,
        # sonst drohte ein ABI-Mismatch ueber die DLL-Grenze hinweg. Falls
        # PSndPlayer.pro dieses Define je aendert, muss es hier synchron
        # nachgezogen werden.
        DEFINES += _USE_TYPE_CHAR

        # exportbwfwave.h/bwfbext.h/bwfixml.h (PSndPlayer/BWFExport, ueber
        # Export/export.h eingebunden) binden bmx-Header ein (<bmx/wave/
        # WaveFileIO.h> usw.). Export/export.h selbst referenziert keinen
        # dieser bmx-Typen in eigenen Deklarationen (nur der Include ist da)
        # -- bislang daher nur INCLUDEPATH noetig, kein zusaetzliches
        # LIBS-Linking gegen bmx/libMXF. Debug/Release-Trennung analog zu
        # PSndPlayer.pro (bmx-install-debug wegen _ITERATOR_DEBUG_LEVEL).
        CONFIG(debug, debug|release) {
            BMX_INSTALL = C:/Users/admin/Developer/bmx-install-debug
        } else {
            BMX_INSTALL = C:/Users/admin/Developer/bmx-install
        }
        INCLUDEPATH += $$BMX_INSTALL/include

        # Bereits gebaute PSndPlayer-DLL/Importbibliothek dieses Rechners --
        # linken statt selbst mitkompilieren (Windows-Pendant zu macOS'
        # "LIBS += -lPSndPlayer" gegen die installierte dylib). Die "1" im
        # Dateinamen kommt von PSndPlayer.pro's "VERSION = 1.0.0" (qmake
        # haengt bei Shared Libs standardmaessig die Major-Version an).
        PSNDPLAYER_BUILD = C:/Users/admin/Developer/PSndPlayer/build/Desktop_Qt_6_11_1_MSVC2022_ARM64_Debug
        CONFIG(debug, debug|release) {
            LIBS += "$$PSNDPLAYER_BUILD/debug/PSndPlayer1.lib"
            LIBS += "$$VCPKG_INSTALLED/debug/lib/sndfile.lib"
        } else {
            # Release-Build von PSndPlayer existiert auf diesem Rechner noch
            # nicht (s. WINDOWS_ARM64_PORT_STATUS.md, "Bekannte
            # Restrisiken") -- muss dort zuerst gebaut werden, bevor ein
            # Release-Build dieses Testprojekts linken kann.
            LIBS += "$$PSNDPLAYER_BUILD/release/PSndPlayer1.lib"
            LIBS += "$$VCPKG_INSTALLED/lib/sndfile.lib"
        }

        # Laufzeit-Deployment: Gelinkt wird oben nur gegen die .lib-Import-
        # bibliotheken (PSndPlayer1.lib, sndfile.lib) -- die eigentlichen
        # .dll-Dateien braucht Windows davon unabhaengig ERST BEIM
        # PROGRAMMSTART und sucht dafuer zuerst im Ordner der .exe selbst.
        # Compile+Link liefen deshalb bereits erfolgreich durch, der Start
        # von PSNPlayerTestNew.exe scheiterte aber mit "sndfile.dll"/
        # "PSndPlayer1.dll wurde nicht gefunden". Fix: nach dem Linken per
        # win_compat/deploy_runtime_dlls.bat (liegt in diesem Testprojekt,
        # s. dessen eigener Kommentar fuer Details) die noetigen DLLs direkt
        # neben die erzeugte .exe kopieren -- reiner Kopierschritt, keine
        # Aenderung an PSndPlayer/ProSoundFinder/mOPAtomReader/vcpkg selbst.
        CONFIG(debug, debug|release) {
            DEPLOY_CFG = debug
        } else {
            DEPLOY_CFG = release
        }
        QMAKE_POST_LINK += $$shell_quote($$shell_path($$PWD/win_compat/deploy_runtime_dlls.bat)) $$shell_quote($$shell_path($$OUT_PWD/$$DEPLOY_CFG)) $$DEPLOY_CFG
    }
}


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    win_compat/deploy_runtime_dlls.bat
