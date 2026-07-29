/********************************************************************************
** Form generated from reading UI file 'preferencesdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *exportImport_tab;
    QGroupBox *groupBox_5;
    QLineEdit *targetAppLineEdit;
    QPushButton *targetAppPushButton;
    QGroupBox *groupBox_6;
    QPushButton *targetProjectPushButton;
    QLineEdit *targetProjectLineEdit;
    QGroupBox *cacheGroupBox;
    QLineEdit *cacheLineEdit;
    QPushButton *cachePushButton;
    QGroupBox *groupBox_4;
    QCheckBox *askForReplaceExistingFilesCheckBox;
    QCheckBox *autoDuplicateExistingFilesCheckBox;
    QCheckBox *copyFileOnExportCheckBox;
    QCheckBox *dontCopyExsistingFilesCheckBox;
    QFrame *frame_3;
    QGroupBox *groupBox_8;
    QCheckBox *copyFileOnSpotExportCheckBox;
    QCheckBox *consolidateOnSpotCheckBox;
    QLineEdit *handleLengthLineEdit;
    QLabel *handleLengthLabel;
    QCheckBox *splitFilesOnSpotCheckBox;
    QFrame *frame;
    QCheckBox *splitAllTracksOnSpotcheckBox;
    QFrame *frame_2;
    QCheckBox *autoDuplicateSpotCheckBox;
    QCheckBox *dontCopyExistingSpotCheckBox;
    QCheckBox *askForReplaceExistingSpottFilesCheckBox;
    QComboBox *suffixComboBox;
    QLabel *label;
    QCheckBox *stampFilesCheckBox;
    QWidget *tab;
    QGroupBox *groupBox_2;
    QCheckBox *scanMToolsCheckBox;
    QCheckBox *scanFilteredCheckBox;
    QWidget *player_tab;
    QGroupBox *groupBox_3;
    QCheckBox *playOnClickedCheckBox;
    QCheckBox *loadOnManualSelectionCheckBox;
    QCheckBox *sequentialPlayCheckBox;
    QCheckBox *autoPlayCheckBox;
    QCheckBox *pauseOnStopCheckBox;
    QCheckBox *rememberPlayPositionCheckBox;
    QCheckBox *scrubModeTapeCheckBox;
    QWidget *general_tab;
    QGroupBox *groupBox_7;
    QCheckBox *showFilterInBinCheckBox;
    QCheckBox *connectToHomeCheckBox;
    QCheckBox *databaseBackgroundModelEnabledCheckBox;
    QPushButton *setDefaultPushButton;
    QButtonGroup *buttonGroup_2;
    QButtonGroup *buttonGroup;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName("PreferencesDialog");
        PreferencesDialog->resize(807, 722);
        buttonBox = new QDialogButtonBox(PreferencesDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setGeometry(QRect(540, 680, 231, 26));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy);
        buttonBox->setMinimumSize(QSize(0, 22));
        buttonBox->setMaximumSize(QSize(16777215, 26));
        QFont font;
        font.setPointSize(11);
        buttonBox->setFont(font);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        tabWidget = new QTabWidget(PreferencesDialog);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(20, 10, 771, 661));
        tabWidget->setFont(font);
        exportImport_tab = new QWidget();
        exportImport_tab->setObjectName("exportImport_tab");
        groupBox_5 = new QGroupBox(exportImport_tab);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setEnabled(true);
        groupBox_5->setGeometry(QRect(10, 461, 741, 74));
        groupBox_5->setFont(font);
        targetAppLineEdit = new QLineEdit(groupBox_5);
        targetAppLineEdit->setObjectName("targetAppLineEdit");
        targetAppLineEdit->setEnabled(true);
        targetAppLineEdit->setGeometry(QRect(100, 35, 612, 22));
        sizePolicy.setHeightForWidth(targetAppLineEdit->sizePolicy().hasHeightForWidth());
        targetAppLineEdit->setSizePolicy(sizePolicy);
        targetAppLineEdit->setMinimumSize(QSize(100, 20));
        targetAppLineEdit->setMaximumSize(QSize(16777215, 22));
        targetAppLineEdit->setFont(font);
        targetAppPushButton = new QPushButton(groupBox_5);
        targetAppPushButton->setObjectName("targetAppPushButton");
        targetAppPushButton->setEnabled(true);
        targetAppPushButton->setGeometry(QRect(10, 35, 60, 30));
        sizePolicy.setHeightForWidth(targetAppPushButton->sizePolicy().hasHeightForWidth());
        targetAppPushButton->setSizePolicy(sizePolicy);
        targetAppPushButton->setMinimumSize(QSize(0, 22));
        targetAppPushButton->setMaximumSize(QSize(16777215, 30));
        targetAppPushButton->setFont(font);
        groupBox_6 = new QGroupBox(exportImport_tab);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_6->setEnabled(true);
        groupBox_6->setGeometry(QRect(10, 370, 741, 74));
        groupBox_6->setFont(font);
        targetProjectPushButton = new QPushButton(groupBox_6);
        targetProjectPushButton->setObjectName("targetProjectPushButton");
        targetProjectPushButton->setGeometry(QRect(10, 35, 60, 30));
        sizePolicy.setHeightForWidth(targetProjectPushButton->sizePolicy().hasHeightForWidth());
        targetProjectPushButton->setSizePolicy(sizePolicy);
        targetProjectPushButton->setMinimumSize(QSize(0, 22));
        targetProjectPushButton->setMaximumSize(QSize(16777215, 30));
        targetProjectPushButton->setFont(font);
        targetProjectLineEdit = new QLineEdit(groupBox_6);
        targetProjectLineEdit->setObjectName("targetProjectLineEdit");
        targetProjectLineEdit->setGeometry(QRect(100, 35, 612, 22));
        sizePolicy.setHeightForWidth(targetProjectLineEdit->sizePolicy().hasHeightForWidth());
        targetProjectLineEdit->setSizePolicy(sizePolicy);
        targetProjectLineEdit->setMinimumSize(QSize(100, 20));
        targetProjectLineEdit->setMaximumSize(QSize(16777215, 22));
        targetProjectLineEdit->setFont(font);
        targetProjectLineEdit->raise();
        targetProjectPushButton->raise();
        cacheGroupBox = new QGroupBox(exportImport_tab);
        cacheGroupBox->setObjectName("cacheGroupBox");
        cacheGroupBox->setEnabled(false);
        cacheGroupBox->setGeometry(QRect(10, 550, 741, 74));
        cacheGroupBox->setFont(font);
        cacheLineEdit = new QLineEdit(cacheGroupBox);
        cacheLineEdit->setObjectName("cacheLineEdit");
        cacheLineEdit->setEnabled(false);
        cacheLineEdit->setGeometry(QRect(100, 35, 612, 22));
        sizePolicy.setHeightForWidth(cacheLineEdit->sizePolicy().hasHeightForWidth());
        cacheLineEdit->setSizePolicy(sizePolicy);
        cacheLineEdit->setMinimumSize(QSize(100, 20));
        cacheLineEdit->setMaximumSize(QSize(16777215, 22));
        cacheLineEdit->setFont(font);
        cachePushButton = new QPushButton(cacheGroupBox);
        cachePushButton->setObjectName("cachePushButton");
        cachePushButton->setEnabled(false);
        cachePushButton->setGeometry(QRect(10, 35, 60, 30));
        sizePolicy.setHeightForWidth(cachePushButton->sizePolicy().hasHeightForWidth());
        cachePushButton->setSizePolicy(sizePolicy);
        cachePushButton->setMinimumSize(QSize(0, 22));
        cachePushButton->setMaximumSize(QSize(16777215, 30));
        cachePushButton->setFont(font);
        groupBox_4 = new QGroupBox(exportImport_tab);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(10, 150, 741, 131));
        groupBox_4->setFont(font);
        askForReplaceExistingFilesCheckBox = new QCheckBox(groupBox_4);
        buttonGroup_2 = new QButtonGroup(PreferencesDialog);
        buttonGroup_2->setObjectName("buttonGroup_2");
        buttonGroup_2->addButton(askForReplaceExistingFilesCheckBox);
        askForReplaceExistingFilesCheckBox->setObjectName("askForReplaceExistingFilesCheckBox");
        askForReplaceExistingFilesCheckBox->setGeometry(QRect(170, 63, 211, 20));
        askForReplaceExistingFilesCheckBox->setFont(font);
        autoDuplicateExistingFilesCheckBox = new QCheckBox(groupBox_4);
        buttonGroup_2->addButton(autoDuplicateExistingFilesCheckBox);
        autoDuplicateExistingFilesCheckBox->setObjectName("autoDuplicateExistingFilesCheckBox");
        autoDuplicateExistingFilesCheckBox->setGeometry(QRect(170, 89, 191, 20));
        autoDuplicateExistingFilesCheckBox->setFont(font);
        copyFileOnExportCheckBox = new QCheckBox(groupBox_4);
        copyFileOnExportCheckBox->setObjectName("copyFileOnExportCheckBox");
        copyFileOnExportCheckBox->setGeometry(QRect(10, 37, 121, 20));
        copyFileOnExportCheckBox->setFont(font);
        copyFileOnExportCheckBox->setChecked(true);
        copyFileOnExportCheckBox->setTristate(false);
        dontCopyExsistingFilesCheckBox = new QCheckBox(groupBox_4);
        buttonGroup_2->addButton(dontCopyExsistingFilesCheckBox);
        dontCopyExsistingFilesCheckBox->setObjectName("dontCopyExsistingFilesCheckBox");
        dontCopyExsistingFilesCheckBox->setGeometry(QRect(170, 37, 211, 20));
        dontCopyExsistingFilesCheckBox->setFont(font);
        frame_3 = new QFrame(groupBox_4);
        frame_3->setObjectName("frame_3");
        frame_3->setGeometry(QRect(160, 30, 221, 91));
        frame_3->setFont(font);
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_3->raise();
        askForReplaceExistingFilesCheckBox->raise();
        autoDuplicateExistingFilesCheckBox->raise();
        copyFileOnExportCheckBox->raise();
        dontCopyExsistingFilesCheckBox->raise();
        groupBox_8 = new QGroupBox(exportImport_tab);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(10, 10, 741, 131));
        groupBox_8->setFont(font);
        copyFileOnSpotExportCheckBox = new QCheckBox(groupBox_8);
        copyFileOnSpotExportCheckBox->setObjectName("copyFileOnSpotExportCheckBox");
        copyFileOnSpotExportCheckBox->setGeometry(QRect(10, 40, 121, 21));
        copyFileOnSpotExportCheckBox->setFont(font);
        copyFileOnSpotExportCheckBox->setChecked(false);
        copyFileOnSpotExportCheckBox->setTristate(false);
        consolidateOnSpotCheckBox = new QCheckBox(groupBox_8);
        consolidateOnSpotCheckBox->setObjectName("consolidateOnSpotCheckBox");
        consolidateOnSpotCheckBox->setEnabled(false);
        consolidateOnSpotCheckBox->setGeometry(QRect(421, 42, 141, 20));
        consolidateOnSpotCheckBox->setFont(font);
        handleLengthLineEdit = new QLineEdit(groupBox_8);
        handleLengthLineEdit->setObjectName("handleLengthLineEdit");
        handleLengthLineEdit->setEnabled(false);
        handleLengthLineEdit->setGeometry(QRect(550, 72, 51, 21));
        handleLengthLineEdit->setFont(font);
        handleLengthLabel = new QLabel(groupBox_8);
        handleLengthLabel->setObjectName("handleLengthLabel");
        handleLengthLabel->setEnabled(false);
        handleLengthLabel->setGeometry(QRect(422, 72, 121, 16));
        handleLengthLabel->setFont(font);
        splitFilesOnSpotCheckBox = new QCheckBox(groupBox_8);
        splitFilesOnSpotCheckBox->setObjectName("splitFilesOnSpotCheckBox");
        splitFilesOnSpotCheckBox->setGeometry(QRect(10, 66, 141, 21));
        splitFilesOnSpotCheckBox->setFont(font);
        splitFilesOnSpotCheckBox->setChecked(false);
        splitFilesOnSpotCheckBox->setTristate(false);
        frame = new QFrame(groupBox_8);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(400, 32, 221, 91));
        frame->setFont(font);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        splitAllTracksOnSpotcheckBox = new QCheckBox(groupBox_8);
        splitAllTracksOnSpotcheckBox->setObjectName("splitAllTracksOnSpotcheckBox");
        splitAllTracksOnSpotcheckBox->setGeometry(QRect(10, 96, 111, 20));
        frame_2 = new QFrame(groupBox_8);
        frame_2->setObjectName("frame_2");
        frame_2->setGeometry(QRect(140, 32, 241, 91));
        frame_2->setFont(font);
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        autoDuplicateSpotCheckBox = new QCheckBox(groupBox_8);
        buttonGroup = new QButtonGroup(PreferencesDialog);
        buttonGroup->setObjectName("buttonGroup");
        buttonGroup->addButton(autoDuplicateSpotCheckBox);
        autoDuplicateSpotCheckBox->setObjectName("autoDuplicateSpotCheckBox");
        autoDuplicateSpotCheckBox->setGeometry(QRect(170, 94, 201, 20));
        autoDuplicateSpotCheckBox->setFont(font);
        dontCopyExistingSpotCheckBox = new QCheckBox(groupBox_8);
        buttonGroup->addButton(dontCopyExistingSpotCheckBox);
        dontCopyExistingSpotCheckBox->setObjectName("dontCopyExistingSpotCheckBox");
        dontCopyExistingSpotCheckBox->setGeometry(QRect(170, 42, 201, 21));
        dontCopyExistingSpotCheckBox->setFont(font);
        dontCopyExistingSpotCheckBox->setChecked(true);
        askForReplaceExistingSpottFilesCheckBox = new QCheckBox(groupBox_8);
        buttonGroup->addButton(askForReplaceExistingSpottFilesCheckBox);
        askForReplaceExistingSpottFilesCheckBox->setObjectName("askForReplaceExistingSpottFilesCheckBox");
        askForReplaceExistingSpottFilesCheckBox->setGeometry(QRect(170, 67, 201, 21));
        askForReplaceExistingSpottFilesCheckBox->setFont(font);
        frame->raise();
        copyFileOnSpotExportCheckBox->raise();
        consolidateOnSpotCheckBox->raise();
        handleLengthLineEdit->raise();
        handleLengthLabel->raise();
        splitFilesOnSpotCheckBox->raise();
        splitAllTracksOnSpotcheckBox->raise();
        frame_2->raise();
        autoDuplicateSpotCheckBox->raise();
        dontCopyExistingSpotCheckBox->raise();
        askForReplaceExistingSpottFilesCheckBox->raise();
        suffixComboBox = new QComboBox(exportImport_tab);
        suffixComboBox->setObjectName("suffixComboBox");
        suffixComboBox->setGeometry(QRect(190, 310, 103, 32));
        suffixComboBox->setFont(font);
        label = new QLabel(exportImport_tab);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 310, 161, 31));
        label->setFont(font);
        stampFilesCheckBox = new QCheckBox(exportImport_tab);
        stampFilesCheckBox->setObjectName("stampFilesCheckBox");
        stampFilesCheckBox->setGeometry(QRect(320, 315, 211, 20));
        tabWidget->addTab(exportImport_tab, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(20, 20, 741, 61));
        scanMToolsCheckBox = new QCheckBox(groupBox_2);
        scanMToolsCheckBox->setObjectName("scanMToolsCheckBox");
        scanMToolsCheckBox->setGeometry(QRect(10, 30, 111, 20));
        scanMToolsCheckBox->setTristate(false);
        scanFilteredCheckBox = new QCheckBox(groupBox_2);
        scanFilteredCheckBox->setObjectName("scanFilteredCheckBox");
        scanFilteredCheckBox->setGeometry(QRect(140, 30, 241, 20));
        scanFilteredCheckBox->setTristate(false);
        tabWidget->addTab(tab, QString());
        player_tab = new QWidget();
        player_tab->setObjectName("player_tab");
        groupBox_3 = new QGroupBox(player_tab);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(20, 10, 271, 241));
        playOnClickedCheckBox = new QCheckBox(groupBox_3);
        playOnClickedCheckBox->setObjectName("playOnClickedCheckBox");
        playOnClickedCheckBox->setGeometry(QRect(20, 90, 241, 20));
        loadOnManualSelectionCheckBox = new QCheckBox(groupBox_3);
        loadOnManualSelectionCheckBox->setObjectName("loadOnManualSelectionCheckBox");
        loadOnManualSelectionCheckBox->setGeometry(QRect(20, 120, 241, 20));
        loadOnManualSelectionCheckBox->setTristate(false);
        sequentialPlayCheckBox = new QCheckBox(groupBox_3);
        sequentialPlayCheckBox->setObjectName("sequentialPlayCheckBox");
        sequentialPlayCheckBox->setGeometry(QRect(20, 60, 241, 20));
        autoPlayCheckBox = new QCheckBox(groupBox_3);
        autoPlayCheckBox->setObjectName("autoPlayCheckBox");
        autoPlayCheckBox->setGeometry(QRect(20, 30, 241, 20));
        pauseOnStopCheckBox = new QCheckBox(groupBox_3);
        pauseOnStopCheckBox->setObjectName("pauseOnStopCheckBox");
        pauseOnStopCheckBox->setGeometry(QRect(20, 150, 181, 21));
        rememberPlayPositionCheckBox = new QCheckBox(groupBox_3);
        rememberPlayPositionCheckBox->setObjectName("rememberPlayPositionCheckBox");
        rememberPlayPositionCheckBox->setGeometry(QRect(20, 180, 241, 21));
        scrubModeTapeCheckBox = new QCheckBox(groupBox_3);
        scrubModeTapeCheckBox->setObjectName("scrubModeTapeCheckBox");
        scrubModeTapeCheckBox->setGeometry(QRect(20, 210, 241, 21));
        tabWidget->addTab(player_tab, QString());
        general_tab = new QWidget();
        general_tab->setObjectName("general_tab");
        groupBox_7 = new QGroupBox(general_tab);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setEnabled(true);
        groupBox_7->setGeometry(QRect(40, 20, 271, 161));
        showFilterInBinCheckBox = new QCheckBox(groupBox_7);
        showFilterInBinCheckBox->setObjectName("showFilterInBinCheckBox");
        showFilterInBinCheckBox->setEnabled(true);
        showFilterInBinCheckBox->setGeometry(QRect(20, 30, 241, 20));
        connectToHomeCheckBox = new QCheckBox(groupBox_7);
        connectToHomeCheckBox->setObjectName("connectToHomeCheckBox");
        connectToHomeCheckBox->setGeometry(QRect(20, 60, 171, 20));
        databaseBackgroundModelEnabledCheckBox = new QCheckBox(groupBox_7);
        databaseBackgroundModelEnabledCheckBox->setObjectName("databaseBackgroundModelEnabledCheckBox");
        databaseBackgroundModelEnabledCheckBox->setGeometry(QRect(20, 90, 161, 20));
        tabWidget->addTab(general_tab, QString());
        setDefaultPushButton = new QPushButton(PreferencesDialog);
        setDefaultPushButton->setObjectName("setDefaultPushButton");
        setDefaultPushButton->setGeometry(QRect(440, 680, 100, 26));
        sizePolicy.setHeightForWidth(setDefaultPushButton->sizePolicy().hasHeightForWidth());
        setDefaultPushButton->setSizePolicy(sizePolicy);
        setDefaultPushButton->setMinimumSize(QSize(0, 22));
        setDefaultPushButton->setMaximumSize(QSize(16777215, 26));
        setDefaultPushButton->setFont(font);

        retranslateUi(PreferencesDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, PreferencesDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, PreferencesDialog, qOverload<>(&QDialog::reject));

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferencesDialog)
    {
        PreferencesDialog->setWindowTitle(QCoreApplication::translate("PreferencesDialog", "Preferences", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("PreferencesDialog", "Target application", nullptr));
#if QT_CONFIG(accessibility)
        targetAppPushButton->setAccessibleName(QCoreApplication::translate("PreferencesDialog", "Browse", nullptr));
#endif // QT_CONFIG(accessibility)
        targetAppPushButton->setText(QCoreApplication::translate("PreferencesDialog", "Browse", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("PreferencesDialog", "Folder for copied files:", nullptr));
#if QT_CONFIG(accessibility)
        targetProjectPushButton->setAccessibleName(QCoreApplication::translate("PreferencesDialog", "Browse", nullptr));
#endif // QT_CONFIG(accessibility)
        targetProjectPushButton->setText(QCoreApplication::translate("PreferencesDialog", "Browse", nullptr));
        cacheGroupBox->setTitle(QCoreApplication::translate("PreferencesDialog", "Cache", nullptr));
#if QT_CONFIG(accessibility)
        cachePushButton->setAccessibleName(QCoreApplication::translate("PreferencesDialog", "Browse", nullptr));
#endif // QT_CONFIG(accessibility)
        cachePushButton->setText(QCoreApplication::translate("PreferencesDialog", "Browse", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("PreferencesDialog", "Drag n Drop / Send :", nullptr));
#if QT_CONFIG(tooltip)
        askForReplaceExistingFilesCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If a file with the same name already exists, a copy is created and exported to the target program. Otherwise the existing file is used.", nullptr));
#endif // QT_CONFIG(tooltip)
        askForReplaceExistingFilesCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Ask for replace existing files", nullptr));
#if QT_CONFIG(tooltip)
        autoDuplicateExistingFilesCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "<html><head/><body><p>On Drag and Drop, Send or Spot:</p><p>If a file with the same name already exists,</p><p>a copy is created and exported to the target application. </p><p>Otherwise the existing file is used.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        autoDuplicateExistingFilesCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Auto duplicate exising files", nullptr));
        copyFileOnExportCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Copy the files", nullptr));
#if QT_CONFIG(tooltip)
        dontCopyExsistingFilesCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If a file with the same name already exists, a copy is created and exported to the target program. Otherwise the existing file is used.", nullptr));
#endif // QT_CONFIG(tooltip)
        dontCopyExsistingFilesCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Don't copy  existing files", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("PreferencesDialog", "Spot:", nullptr));
        copyFileOnSpotExportCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Copy the files", nullptr));
#if QT_CONFIG(tooltip)
        consolidateOnSpotCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If a file with the same name already exists, a copy is created and exported to the target program. Otherwise the existing file is used.", nullptr));
#endif // QT_CONFIG(tooltip)
        consolidateOnSpotCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Consolidate files", nullptr));
        handleLengthLabel->setText(QCoreApplication::translate("PreferencesDialog", "Handlelength: (sec)", nullptr));
        splitFilesOnSpotCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Split Multichannel", nullptr));
        splitAllTracksOnSpotcheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Spot All Tracks", nullptr));
#if QT_CONFIG(tooltip)
        autoDuplicateSpotCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "<html><head/><body><p>On Drag and Drop, Send or Spot:</p><p>If a file with the same name already exists,</p><p>a copy is created and exported to the target application. </p><p>Otherwise the existing file is used.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        autoDuplicateSpotCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Auto duplicate exising files", nullptr));
#if QT_CONFIG(tooltip)
        dontCopyExistingSpotCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If a file with the same name already exists, a copy is created and exported to the target program. Otherwise the existing file is used.", nullptr));
#endif // QT_CONFIG(tooltip)
        dontCopyExistingSpotCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Dont copy existing files", nullptr));
#if QT_CONFIG(tooltip)
        askForReplaceExistingSpottFilesCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If a file with the same name already exists, a copy is created and exported to the target program. Otherwise the existing file is used.", nullptr));
#endif // QT_CONFIG(tooltip)
        askForReplaceExistingSpottFilesCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Ask for replace existimg files ", nullptr));
        label->setText(QCoreApplication::translate("PreferencesDialog", "Splitted Channel Suffix:", nullptr));
        stampFilesCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Stamp Files", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(exportImport_tab), QCoreApplication::translate("PreferencesDialog", "Export", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("PreferencesDialog", "Scan Settings:", nullptr));
        scanMToolsCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "scanMTools", nullptr));
        scanFilteredCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "scan files  filtered by  suffix", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("PreferencesDialog", "Import", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("PreferencesDialog", "Player Settings", nullptr));
#if QT_CONFIG(tooltip)
        playOnClickedCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If you click in the Waveform the File will be played at Click Position", nullptr));
#endif // QT_CONFIG(tooltip)
        playOnClickedCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Play on click in waveform", nullptr));
#if QT_CONFIG(tooltip)
        loadOnManualSelectionCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "If you select a File, the File will be loaded to the Player. If a File is already loaded to the Player, playing will be stopped. ", nullptr));
#endif // QT_CONFIG(tooltip)
        loadOnManualSelectionCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Load table entries on selection", nullptr));
        sequentialPlayCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Play table entries sequentially", nullptr));
        autoPlayCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Play Ttble entry on click", nullptr));
        pauseOnStopCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Pause on stop", nullptr));
#if QT_CONFIG(tooltip)
        rememberPlayPositionCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "Remembers the last play position of each file, so playback resumes from there the next time the file is opened. Persists across restarts.", nullptr));
#endif // QT_CONFIG(tooltip)
        rememberPlayPositionCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Remember play position per file", nullptr));
#if QT_CONFIG(tooltip)
        scrubModeTapeCheckBox->setToolTip(QCoreApplication::translate("PreferencesDialog", "Tape scrub: Cmd+Alt+Drag (macOS) / Ctrl+Alt+Drag (Windows) in the waveform plays audio in real time, speed/direction follow the drag. Unchecked: short audio snippets at the mouse position instead.", nullptr));
#endif // QT_CONFIG(tooltip)
        scrubModeTapeCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Tape-style scrubbing (Cmd+Alt+Drag / Ctrl+Alt+Drag)", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(player_tab), QCoreApplication::translate("PreferencesDialog", "Player", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("PreferencesDialog", "Filter", nullptr));
        showFilterInBinCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Show Filter in Bin", nullptr));
        connectToHomeCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Look for updates", nullptr));
        databaseBackgroundModelEnabledCheckBox->setText(QCoreApplication::translate("PreferencesDialog", "Load full database", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(general_tab), QCoreApplication::translate("PreferencesDialog", "General", nullptr));
        setDefaultPushButton->setText(QCoreApplication::translate("PreferencesDialog", "Set Default", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
