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



# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
