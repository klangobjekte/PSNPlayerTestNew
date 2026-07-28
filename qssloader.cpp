#include "qssloader.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

//! Ersetzt LESS-artige "$variable: value;"-Deklarationen im Kopf-Kommentar
//! des Stylesheets durch ihren Wert im restlichen Text. Portierte Kopie der
//! gleichnamigen Funktion aus ProSoundFinders stylesheetloader.cpp (dort
//! Teil der StyleSheetLoader-Klasse) - hier bewusst als freie Funktion ohne
//! deren QSettings-/Dokumentverwaltungs-Unterbau.
static QString replaceQssVariables(const QString &sheet)
{
    int t = sheet.indexOf(QStringLiteral("*/"));
    QString mSheet = sheet;
    QString variableDeclaration = mSheet.left(t + 2);
    QString truncatedStyle = mSheet.mid(t + 2);

    int i = 0;
    int begVar = 0;
    int endVar = 0;
    int begVal = 0;
    int endVal = 0;

    while (true) {
        begVar = i = variableDeclaration.indexOf('$', i);
        i = endVar = begVal = variableDeclaration.indexOf(':', i) + 1;
        i = endVal = variableDeclaration.indexOf(';', i);
        if (begVar < 0 || begVal <= 0 || endVal < 0)
            break;
        const QString var = variableDeclaration.mid(begVar, endVar - begVar - 1);
        const QString val = variableDeclaration.mid(begVal, endVal - begVal);
        if (var.isEmpty())
            break;
        QRegularExpression rx("\\" + var + "\\b");
        truncatedStyle.replace(rx, val);
    }

    return truncatedStyle;
}

QString loadDefaultStyleSheet()
{
    QFile file(QStringLiteral(":/qss/default.qss"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream stream(&file);
    const QString sheet = stream.readAll();
    return replaceQssVariables(sheet);
}
