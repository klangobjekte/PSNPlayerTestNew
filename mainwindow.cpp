#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "abstractplayerwidget.h"
#include "playerwidgetV2.h"
#include "psfdefinitions.h"
#include "preferencesdialog.h"
//! NEU: fuer die beiden Betriebsart-Actions in createMenus()
//! (setPauseOnStop()/setSequentialPlay()).
#include "preferencescontrol.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QKeySequence>
#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QtAwesome.h>
#include <pSndPlayer/psnddefinitions.h>
//! NEU: fuer MediaSource::getFilename() im sourceChanged-Handler, s.
//! createPlayerDock().
#include <pSndPlayer/mediasource.h>
#include <QDropEvent>
#include <QMimeData>
#include <QSettings>
#include <QFileInfo>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QUrl>

//! Eigene, von ProSoundFinder unabhaengige QSettings-Gruppe fuer
//! PSNPlayerTestNew-spezifische Testeinstellungen (s. eventFilter()/
//! loadDroppedFiles()).
static const QString PSNTEST_SETTINGS_GROUP = QStringLiteral("PSNPlayerTestNew");
static const QString PSNTEST_LAST_DROPPED_FILE_KEY = QStringLiteral("lastDroppedFile");
//! NEU: s. Chat "Mehrfach-Lade-Test" -- Liste statt Einzeldatei, s.
//! loadDroppedFiles(). Reihenfolge: aeltester Eintrag zuerst.
static const QString PSNTEST_DROPPED_FILES_KEY = QStringLiteral("droppedFiles");
static const int PSNTEST_MAX_DROPPED_FILES = 5;
//! Zeitabstand zwischen den gestaffelten addFile()-Aufrufen -- bewusst kurz
//! (statt z.B. mehrerer Sekunden), damit der Generator der vorherigen Datei
//! plausibel noch laufen kann, wenn die naechste startet (s. Chat: genau das
//! Szenario, das requestAsyncGeneratorStop()s Race Condition braucht, um
//! ueberhaupt aufzutreten -- ein einzelner, isolierter Ladevorgang kann sie
//! nie ausloesen).
static const int PSNTEST_DROPPED_FILES_STAGGER_MS = 500;
static const QString PSNTEST_GEOMETRY_KEY = QStringLiteral("windowGeometry");
static const QString PSNTEST_STATE_KEY = QStringLiteral("windowState");
static const QString PSNTEST_DEVICE_ID_KEY = QStringLiteral("outputDeviceId");
static const QString PSNTEST_SAMPLERATE_KEY = QStringLiteral("outputSamplerate");
static const QString PSNTEST_BITDEPTH_KEY = QStringLiteral("outputBitDepthText");
//! NEU: s. Chat "RME-Mehrkanal-Routing".
static const QString PSNTEST_CHANNELOFFSET_KEY = QStringLiteral("outputChannelOffsetText");
//! NEU: s. Chat "Vorhaben A -- umschaltbares Mehrkanal-Routing".
static const QString PSNTEST_PASSTHROUGH_KEY = QStringLiteral("multichannelPassthrough");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //! NEU: PlayerWidget::readSettings() unterscheidet zwischen "MainWindow"
    //! und "Zweitfenster" anhand von _windowTitle == PSF_STATIC_MainWindowName
    //! (parentWidget()->parentWidget()->windowTitle(), s. AbstractPlayerWidget::
    //! initVariables()) - im Zweitfenster-Zweig defaulten Ruler/Button-Gruppen
    //! auf false. Ohne diesen Titel (wie in echtem ProSoundFinder mainwindow.cpp)
    //! bleiben sie in diesem Testfenster unsichtbar.
    setWindowTitle(PSF_STATIC_MainWindowName);
    //playerWidget = new PlayerWidget(this);
    //playerWidget->show();
    //this->setCentralWidget(playerWidget);

    //! AbstractPlayerWidget::initView() greift auf PSF_GLOBAL_Awesome zu
    //! (siehe ProSoundFinder mainwindow.cpp) - ohne diese Initialisierung
    //! bleibt der Pointer NULL und initView() stuerzt ab.
    PSF_GLOBAL_Awesome = new QtAwesome(qApp);
    PSF_GLOBAL_Awesome->initFontAwesome();
    PSF_GLOBAL_Awesome->setDefaultOption("scale-factor", 0.6);

    //! PreferencesControl::set*(...) ruft unbedingt _mpD->refreshDialogAndSettings()
    //! auf (siehe ProSoundFinder preferencescontrol.cpp) - _mpD wird erst durch den
    //! PreferencesDialog-Konstruktor via registerPreferencesDialog() gesetzt.
    //! In ProSoundFinder existiert der Dialog schon vor dem ersten PlayerWidget;
    //! hier legen wir ihn (unsichtbar) frueh genug an, damit z.B. der
    //! on_soloButton_clicked()-Aufruf im PlayerWidget-Konstruktor nicht auf
    //! einen NULL-Pointer trifft.
    new PreferencesDialog(this);

    createPlayerDock();
    //! NEU: s. Doku bei createMenus() im Header -- muss nach createPlayerDock()
    //! stehen, alle Actions greifen auf playerWidget zu.
    createMenus();

    //! s. eventFilter(): global auf qApp installiert, damit wir QEvent::Drop
    //! sehen, auch wenn das eigentliche Drop-Ziel ein Enkel-Widget tief in
    //! PlayerWidget ist (WaveformController/GraphicsView in PSndPlayer).
    qApp->installEventFilter(this);

    //! NEU: s. Chat "manueller Resize immer noch noetig -- Wellenform bleibt
    //! grob". loadLastDroppedFile() lief bisher HIER, also noch im
    //! Konstruktor -- main.cpp ruft w.show() aber erst NACH der kompletten
    //! Konstruktion auf (s. dort: "MainWindow w; w.show();"). Das Laden der
    //! Datei (-> WaveformDrawer::setMediaSource() -> WaveformController::
    //! getTrimmedSize(), s. dort) passierte dadurch, WAEHREND das Fenster
    //! noch gar nicht gezeigt war -- selbst ein live aus this->size()
    //! berechneter Wert ist zu diesem Zeitpunkt zwangslaeufig noch der
    //! Qt-Vorgabewert, nicht die echte Fenstergroesse (die erst mit/nach
    //! show() feststeht). Kein Timing-Feinschliff in PSndPlayer kann das
    //! reparieren, wenn der Aufruf schon vor show() passiert. Fix: das
    //! Laden auf die naechste Event-Loop-Runde verschieben (QTimer::
    //! singleShot(0, ...)) -- main() ruft w.show() SYNCHRON direkt nach der
    //! Konstruktion auf, bevor a.exec() die Event-Loop startet; der
    //! verzoegerte Aufruf feuert deshalb garantiert danach.
    QTimer::singleShot(0, this, [this](){
        loadDroppedFiles();
    });
    loadWindowGeometry();
    loadOutputSettings();
}

//! NEU: s. Doku bei createMenus() im Header. Nachbildung des Player-/
//! Waveform-Teils aus ProSoundFinders MainWindow-Menue. Bewusst ueber
//! dieselben oeffentlichen Aufrufe wie dort -- die Fundstellen stehen jeweils
//! als Kommentar dabei, damit ein Abgleich moeglich bleibt, wenn sich
//! ProSoundFinder aendert.
void MainWindow::createMenus()
{
    if(!playerWidget){
        qWarning() << "MainWindow::createMenus: kein playerWidget -- Menue wird nicht angelegt";
        return;
    }

    //! ++++ Player ++++ (ProSoundFinder: playAction/playReverseAction/
    //! pauseAction/stopAction im exklusiven playerActionsAlignmentGroup,
    //! mainwindow.cpp dort ~6090-6154)
    QMenu *playerMenu = menuBar()->addMenu(tr("&Player"));

    _playAction = new QAction(tr("&Play"), this);
    _playAction->setCheckable(true);
    //! NEU: s. Doku bei syncTransportActions() -- die LEERTASTE wandert
    //! zustandsabhaengig zwischen Play und Pause, genau wie in ProSoundFinder.
    //! Startzustand ist "gestoppt", also beginnt sie hier (identisch zu
    //! mainwindow.cpp:6094 dort: playAction->setShortcut(tr(" "))).
    _playAction->setShortcut(QKeySequence(Qt::Key_Space));
    connect(_playAction, &QAction::triggered, this, [this](){ playerWidget->play(); });

    _playReverseAction = new QAction(tr("Play &Reverse"), this);
    _playReverseAction->setCheckable(true);
    connect(_playReverseAction, &QAction::triggered, this, [this](){ playerWidget->playRev(); });

    _pauseAction = new QAction(tr("Pa&use"), this);
    _pauseAction->setCheckable(true);
    connect(_pauseAction, &QAction::triggered, this, [this](){ playerWidget->pause(); });

    _stopAction = new QAction(tr("&Stop"), this);
    _stopAction->setCheckable(true);
    _stopAction->setChecked(true);
    connect(_stopAction, &QAction::triggered, this, [this](){ playerWidget->stop(); });

    //! Exklusiv wie in ProSoundFinder -- immer genau ein Zustand markiert.
    _transportGroup = new QActionGroup(this);
    _transportGroup->addAction(_playAction);
    _transportGroup->addAction(_playReverseAction);
    _transportGroup->addAction(_pauseAction);
    _transportGroup->addAction(_stopAction);

    playerMenu->addAction(_playAction);
    playerMenu->addAction(_playReverseAction);
    playerMenu->addAction(_pauseAction);
    playerMenu->addAction(_stopAction);
    playerMenu->addSeparator();

    //! ProSoundFinder: returnToZeroAction -> PlayerWidget::on_setToZeroAction_triggered()
    QAction *returnToZeroAction = new QAction(tr("Return to &Zero"), this);
    connect(returnToZeroAction, &QAction::triggered, this,
            [this](){ playerWidget->on_setToZeroAction_triggered(true); });
    playerMenu->addAction(returnToZeroAction);
    playerMenu->addSeparator();

    //! ProSoundFinder: repeatAction -> PlayerWidget::setRepeat(bool), Ctrl+R
    _repeatAction = new QAction(tr("Re&peat"), this);
    _repeatAction->setCheckable(true);
    _repeatAction->setShortcut(QKeySequence(tr("Ctrl+R")));
    connect(_repeatAction, &QAction::triggered, this,
            [this](bool checked){ playerWidget->setRepeat(checked); });
    playerMenu->addAction(_repeatAction);
    playerMenu->addSeparator();

    //! NEU: s. Chat "QPushButton fuer Scrubbing, permanent an" --
    //! ProSoundFinder: scrubbingAction -> PlayerWidget::setScrubAlwaysOn().
    //! Rueckweg (scrubBtn im PlayerWidget -> Menue-Haekchen hier) ueber
    //! publishScrubAlwaysOnState, gleiches Muster wie im ProSoundFinder-
    //! MainWindow.
    //! Anzeige-Shortcut "\tH" nur als Beschriftung -- die echte
    //! Tastenbehandlung macht der 'h'-QShortcut im PlayerWidget (s. dort);
    //! ein zusaetzlicher setShortcut() hier wuerde damit kollidieren
    //! (Ambiguitaet, keiner von beiden feuert).
    QAction *scrubbingAction = new QAction(tr("Scru&bbing\tH"), this);
    scrubbingAction->setCheckable(true);
    connect(scrubbingAction, &QAction::triggered, this,
            [this](bool checked){ playerWidget->setScrubAlwaysOn(checked); });
    connect(playerWidget, &PlayerWidget::publishScrubAlwaysOnState,
            scrubbingAction, &QAction::setChecked);
    //! createMenus() laeuft NACH createPlayerDock() (s. Konstruktor) --
    //! den beim PlayerWidget-Aufbau aus den Settings geladenen Zustand
    //! (dessen publish-Signal vor diesem connect feuerte) hier nachziehen.
    scrubbingAction->setChecked(playerWidget->scrubAlwaysOn());
    playerMenu->addAction(scrubbingAction);
    playerMenu->addSeparator();

    //! ProSoundFinder: increaseSpeedAction/decreaseSpeedAction
    QAction *speedUpAction = new QAction(tr("Speed &+"), this);
    connect(speedUpAction, &QAction::triggered, this, [this](){ playerWidget->increaseSpeed(); });
    QAction *speedDownAction = new QAction(tr("Speed &-"), this);
    connect(speedDownAction, &QAction::triggered, this, [this](){ playerWidget->decreaseSpeed(); });
    playerMenu->addAction(speedUpAction);
    playerMenu->addAction(speedDownAction);

    //! ++++ Waveform ++++ (ProSoundFinder: scrollingActionGroup ~6132-6147,
    //! Handler on_noScrollingAction_triggered() usw. ~7087-7103)
    QMenu *waveformMenu = menuBar()->addMenu(tr("&Waveform"));

    _noScrollingAction = new QAction(tr("&No Scrolling"), this);
    _noScrollingAction->setCheckable(true);
    connect(_noScrollingAction, &QAction::triggered, this,
            [this](){ playerWidget->setScrollType(PSNDWAVE::NO_SCROLLING); });

    _pageScrollingAction = new QAction(tr("&Page Scrolling"), this);
    _pageScrollingAction->setCheckable(true);
    connect(_pageScrollingAction, &QAction::triggered, this,
            [this](){ playerWidget->setScrollType(PSNDWAVE::PAGE_SCROLLING); });

    _continuousScrollingAction = new QAction(tr("&Continuous Scrolling"), this);
    _continuousScrollingAction->setCheckable(true);
    connect(_continuousScrollingAction, &QAction::triggered, this,
            [this](){ playerWidget->setScrollType(PSNDWAVE::CONTINUOUS_SCROLLING); });

    _scrollingGroup = new QActionGroup(this);
    _scrollingGroup->addAction(_noScrollingAction);
    _scrollingGroup->addAction(_pageScrollingAction);
    _scrollingGroup->addAction(_continuousScrollingAction);
    //! Vorbelegung wie WaveformController::_scrolling (PSNDWAVE::PAGE_SCROLLING,
    //! s. waveformcontroller.h) -- das Menue soll den Ist-Zustand zeigen, nicht
    //! einen anderen behaupten.
    _pageScrollingAction->setChecked(true);

    waveformMenu->addAction(_noScrollingAction);
    waveformMenu->addAction(_pageScrollingAction);
    waveformMenu->addAction(_continuousScrollingAction);
    waveformMenu->addSeparator();

    //! ProSoundFinder: verticalZoomInAction/verticalZoomOutAction ->
    //! PlayerWidget::on_waveVPlusBtn_clicked()/on_waveVMinusBtn_clicked()
    QAction *vZoomInAction = new QAction(tr("Vertical Zoom &In"), this);
    vZoomInAction->setShortcut(QKeySequence(tr("Ctrl+Alt+]")));
    connect(vZoomInAction, &QAction::triggered, this,
            [this](){ playerWidget->on_waveVPlusBtn_clicked(); });
    QAction *vZoomOutAction = new QAction(tr("Vertical Zoom &Out"), this);
    vZoomOutAction->setShortcut(QKeySequence(tr("Ctrl+Alt+[")));
    connect(vZoomOutAction, &QAction::triggered, this,
            [this](){ playerWidget->on_waveVMinusBtn_clicked(); });
    waveformMenu->addAction(vZoomInAction);
    waveformMenu->addAction(vZoomOutAction);
    waveformMenu->addSeparator();

    //! ProSoundFinder: expandWaveformViewAction -> PlayerWidget::on_e_Action_triggered()
    QAction *expandAction = new QAction(tr("&Expand Waveform View"), this);
    connect(expandAction, &QAction::triggered, this,
            [this](){ playerWidget->on_e_Action_triggered(); });
    waveformMenu->addAction(expandAction);

    //! ++++ Options ++++
    QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));

    //! ProSoundFinder: cursorFollowsPlaybackAction -> PreferencesControl::
    //! setPauseOnStop() (mainwindow.cpp dort ~3614). Der Name ist historisch:
    //! "Cursor Follows Playback" und "pause on stop" bezeichnen dieselbe
    //! Betriebsart.
    _cursorFollowsPlaybackAction = new QAction(tr("Cursor Follows Play&back"), this);
    _cursorFollowsPlaybackAction->setCheckable(true);
    connect(_cursorFollowsPlaybackAction, &QAction::triggered, this, [this](bool checked){
        qDebug() << "[MENUDIAG] Cursor Follows Playback ->" << checked
                 << "windowTitle:" << this->windowTitle();
        PreferencesControl::instance()->setPauseOnStop(checked, this->windowTitle());
        //! Gegenprobe: hat der Wert PreferencesControl tatsaechlich erreicht?
        //! Ob er von dort bis ins PlayerWidget durchschlaegt, zeigt die Zeile
        //! "PlayerWidget getSettingsFromPreferencesControl _pauseOnStop" --
        //! die kommt ueber das preferencesChanged()-Signal.
        qDebug() << "[MENUDIAG] PreferencesControl::pauseOnStop() ist jetzt"
                 << PreferencesControl::instance()->pauseOnStop();
    });
    optionsMenu->addAction(_cursorFollowsPlaybackAction);

    //! In ProSoundFinder nur ueber den PreferencesDialog erreichbar -- hier als
    //! direkter Menuepunkt, weil es eine der beiden Betriebsarten ist, die in
    //! der Testanwendung gebraucht werden.
    _sequentialPlayAction = new QAction(tr("Contin&uous Play (Sequential)"), this);
    _sequentialPlayAction->setCheckable(true);
    connect(_sequentialPlayAction, &QAction::triggered, this, [this](bool checked){
        qDebug() << "[MENUDIAG] Continuous Play (Sequential) ->" << checked
                 << "windowTitle:" << this->windowTitle();
        PreferencesControl::instance()->setSequentialPlay(checked, this->windowTitle());
        qDebug() << "[MENUDIAG] PreferencesControl::sequentialPlay() ist jetzt"
                 << PreferencesControl::instance()->sequentialPlay();
    });
    optionsMenu->addAction(_sequentialPlayAction);

    //! ProSoundFinder: soloAction -> PlayerWidget::setSolo(bool)
    _soloAction = new QAction(tr("&Solo"), this);
    _soloAction->setCheckable(true);
    connect(_soloAction, &QAction::triggered, this,
            [this](bool checked){ playerWidget->setSolo(checked); });
    optionsMenu->addAction(_soloAction);
    optionsMenu->addSeparator();

    //! Der Dialog existiert bereits (s. Konstruktor) -- hier nur erreichbar
    //! machen, damit auch die uebrigen Preferences (autoPlay, playOnClicked,
    //! rememberPlayPosition, ...) in der Testanwendung umstellbar sind.
    QAction *preferencesAction = new QAction(tr("&Preferences..."), this);
    connect(preferencesAction, &QAction::triggered, this, [this](){
        if(PreferencesDialog *dlg = this->findChild<PreferencesDialog*>()){
            dlg->show();
            dlg->raise();
            dlg->activateWindow();
        }
    });
    optionsMenu->addAction(preferencesAction);

    //! Zustaende initial und danach bei jeder Aenderung nachziehen.
    syncPreferenceActions();
    connect(PreferencesControl::instance(), &PreferencesControl::preferencesChanged,
            this, &MainWindow::syncPreferenceActions);

    //! PlayerWidget meldet Zustandswechsel ueber dieses Signal -- dasselbe, das
    //! ProSoundFinders MainWindow::setPlayerActions() bedient. Bewusst mit der
    //! Zeiger-Syntax verbunden: die String-Variante hat in ProSoundFinder
    //! wegen einer falschen Signatur jahrelang still nicht funktioniert.
    connect(playerWidget, &PlayerWidget::setPlayerActions, this,
            [this](PSNDPLAYER::PLAYERSTATE state){ syncTransportActions((int)state); });
}

void MainWindow::syncTransportActions(int state)
{
    _lastPlayerState = state;

    //! NEU: s. Chat "Cursor Follows Playback greift in der Testapp nicht" --
    //! woertliche Nachbildung von MainWindow::setPlayerActions() aus
    //! ProSoundFinder (mainwindow.cpp dort ~7956-7999). Dort gibt es KEINEN
    //! Toggle-Eintrag; stattdessen wandert die LEERTASTE zustandsabhaengig
    //! zwischen playAction und pauseAction:
    //!   spielt es  -> Leertaste = Pause  -> PlayerWidget::pause()
    //!   steht es   -> Leertaste = Play   -> PlayerWidget::play()
    //! Genau dadurch laeuft ein "Play/Stop"-Toggle ueber pause() -- die einzige
    //! Stelle, die _pauseOnStop ("Cursor Follows Playback") auswertet.
    //! PlayerWidget::stop() und die K-Taste ignorieren die Einstellung bewusst
    //! (s. playerwidgetV2.cpp:2461 bzw. :594-598 dort).
    //! Space-Handling gibt es weder in PlayerWidget noch in
    //! AbstractPlayerWidget -- deren QShortcut-Liste kennt nur j/k/l/e/s/r/t/
    //! n/p/Return/b/Pfeile/Backspace, und keyPressEvent() nur macOS-
    //! Sondertasten. Die Leertaste kommt ausschliesslich von hier.
    const bool isPlaying = (state == PSNDPLAYER::PLAYING
                            || state == PSNDPLAYER::REVERSE
                            || state == PSNDPLAYER::FAST_FORWARD
                            || state == PSNDPLAYER::FAST_REVERSE);
    if(_playAction && _pauseAction){
        //! Immer erst beide loesen, sonst haette Qt kurzzeitig zwei Actions mit
        //! derselben Taste (mehrdeutiges Shortcut -> keine feuert).
        _playAction->setShortcut(QKeySequence());
        _pauseAction->setShortcut(QKeySequence());
        if(isPlaying){
            _pauseAction->setShortcut(QKeySequence(Qt::Key_Space));
        }
        else{
            _playAction->setShortcut(QKeySequence(Qt::Key_Space));
        }
    }

    QAction *target = nullptr;
    switch(state){
    case PSNDPLAYER::PLAYING:
    case PSNDPLAYER::FAST_FORWARD:  target = _playAction;        break;
    case PSNDPLAYER::REVERSE:
    case PSNDPLAYER::FAST_REVERSE:  target = _playReverseAction; break;
    case PSNDPLAYER::PAUSED:        target = _pauseAction;       break;
    case PSNDPLAYER::STOPPED:       target = _stopAction;        break;
    default: break;
    }
    if(target){
        //! setChecked() loest bei einem QActionGroup-Mitglied kein triggered()
        //! aus -- es gibt hier also keine Rueckkopplung auf den Player.
        target->setChecked(true);
    }
}

void MainWindow::syncPreferenceActions()
{
    if(_cursorFollowsPlaybackAction){
        _cursorFollowsPlaybackAction->setChecked(PreferencesControl::instance()->pauseOnStop());
    }
    if(_sequentialPlayAction){
        _sequentialPlayAction->setChecked(PreferencesControl::instance()->sequentialPlay());
    }
}

//! NEU: s. Doku bei viewDidLoad() im Header -- 1:1 aus ProSoundFinders
//! "Hack - Dragon" (mainwindow.cpp dort, Zeile 771-783) uebernommen. Erzwingt
//! einen echten Resize, um WaveformController zur Feinzeichnung mit der
//! endgueltigen Fenstergroesse zu bewegen.
void MainWindow::viewDidLoad()
{
    qDebug() << "MainWindow viewDidLoad";
    this->resize(this->height(),this->width()+1);
    this->resize(this->height(),this->width()-1);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    settings.setValue(PSNTEST_GEOMETRY_KEY, saveGeometry());
    settings.setValue(PSNTEST_STATE_KEY, saveState());
    settings.endGroup();
    QMainWindow::closeEvent(event);
}

void MainWindow::loadWindowGeometry()
{
    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    const QByteArray geometry = settings.value(PSNTEST_GEOMETRY_KEY).toByteArray();
    const QByteArray state = settings.value(PSNTEST_STATE_KEY).toByteArray();
    settings.endGroup();

    if (!geometry.isEmpty())
        restoreGeometry(geometry);
    if (!state.isEmpty())
        restoreState(state);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    //! Leertaste toggelt Play/Stop, nachgebaut analog zum in ProSoundFinder
    //! erinnerten Verhalten (im dortigen Source aktuell nicht mehr auffindbar).
    //! Ueber den bereits global auf qApp installierten Filter abgefangen,
    //! damit es unabhaengig vom fokussierten Kind-Widget funktioniert -- ein
    //! keyPressEvent()-Override auf MainWindow wuerde nur greifen, wenn das
    //! Fenster selbst den Fokus haelt. Textfelder/ComboBoxen werden
    //! ausgenommen, damit dort weiterhin normal Leerzeichen getippt werden
    //! koennen.
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Space && keyEvent->modifiers() == Qt::NoModifier
            && !qobject_cast<QLineEdit *>(obj) && !qobject_cast<QComboBox *>(obj)) {
            if (playerWidget) {
                if (playerWidget->isPlaying())
                    playerWidget->stop();
                else
                    playerWidget->play();
            }
            return true;
        }
    }

    //! NEU: s. Doku bei viewDidLoad() im Header -- 1:1 aus ProSoundFinders
    //! eventFilter() QEvent::Polish-Behandlung uebernommen (mainwindow.cpp
    //! dort, Zeile 2149-2166), auf das dort ProSoundFinder-spezifische
    //! on_inited()/500ms bewusst verzichtet (hier ohne Bedeutung).
    if (event->type() == QEvent::Polish) {
        if (obj->objectName() == "MainWindow" && windowTitle() == PSF_STATIC_MainWindowName) {
            if (!_viewDidLoad) {
                _viewDidLoad = true;
                QTimer::singleShot(2000, this, SLOT(viewDidLoad()));
            }
        }
    }

    //! NEU: Crash-Fix, s. cancelPendingLoads() im Header. Der Abbruch muss
    //! schon beim DragEnter passieren, nicht erst beim Drop -- das Crash-
    //! Fenster war genau die Zeitspanne zwischen DragEnter und Drop: feuert
    //! dazwischen ein gestaffelter Lade-Timer, zerstoert addFile() die
    //! GraphicsView, ueber der der Drag gerade schwebt, und macOS liefert
    //! den Drop anschliessend an das freigegebene Widget aus.
    if (event->type() == QEvent::DragEnter) {
        cancelPendingLoads();
    }

    //! NEU: "&& obj->isWidgetType()" -- der qApp-Filter sieht dasselbe
    //! QEvent::Drop ZWEIMAL (einmal fuer das QWidgetWindow (ein QWindow,
    //! kein Widget), einmal fuer das Viewport-Widget der GraphicsView --
    //! dieselbe doppelte Zustellkette war auch im Crash-Stacktrace sichtbar).
    //! Ohne den Filter wurde jeder Drop doppelt verarbeitet (Dateien doppelt
    //! an die Liste gehaengt, s. Log-Diagnose: identische Zeile zweimal).
    if (event->type() == QEvent::Drop && obj->isWidgetType()) {
        QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
        if (dropEvent->mimeData()->hasUrls()) {
            const QString path = dropEvent->mimeData()->urls().first().toLocalFile();
            if (!path.isEmpty()) {
                QSettings settings;
                settings.beginGroup(PSNTEST_SETTINGS_GROUP);
                settings.setValue(PSNTEST_LAST_DROPPED_FILE_KEY, path);

                //! NEU: s. Chat "Mehrfach-Lade-Test" -- ALLE bei diesem Drop
                //! enthaltenen Dateien (nicht nur die erste) in die Liste
                //! uebernehmen, s. loadDroppedFiles(). Erlaubt, das
                //! Mehrfach-Lade-Szenario einfach durch Draufziehen mehrerer
                //! Dateien auf einmal zu erzeugen, ohne zusaetzliche UI.
                //!
                //! NEU: s. Chat "eine einzelne Drop-Datei loescht die Liste,
                //! mehrere Drop-Dateien bilden eine Liste". Die Liste wird
                //! jetzt bei JEDEM Drop ERSETZT statt ergaenzt -- vorher wurde
                //! hier der gespeicherte Stand eingelesen und angehaengt, was
                //! bedeutete: wer EINE Datei droppte, um einen Einzeldatei-Fall
                //! zu testen, bekam beim naechsten Start trotzdem die vier
                //! zuvor gedroppten Dateien mitgeladen -- also unfreiwillig
                //! genau das Mehrfach-Lade-Szenario, das isoliert werden
                //! sollte. Ein Drop mit einer Datei ergibt damit eine Liste der
                //! Laenge 1, ein Drop mit n Dateien eine Liste der Laenge n
                //! (weiterhin gedeckelt auf PSNTEST_MAX_DROPPED_FILES).
                QStringList droppedFiles;
                for (const QUrl &url : dropEvent->mimeData()->urls()) {
                    const QString p = url.toLocalFile();
                    if (!p.isEmpty() && QFileInfo::exists(p)) {
                        droppedFiles.append(p);
                    }
                }
                while (droppedFiles.size() > PSNTEST_MAX_DROPPED_FILES) {
                    droppedFiles.removeFirst();
                }
                settings.setValue(PSNTEST_DROPPED_FILES_KEY, droppedFiles);
                settings.endGroup();
                qDebug() << "MainWindow eventFilter: gespeichert als letzte Datei:" << path
                         << "-- rollierende Liste jetzt:" << droppedFiles;

                //! NEU: Der frueher hier stehende QTimer::singleShot(0, ...)
                //! -> updatePlayerHeader(path) ist ENTFALLEN. Er beschriftete den
                //! Header mit der Datei, deren Laden gerade ANGESTOSSEN wurde --
                //! nicht mit der, die hoerbar ist. Bei mehreren gedroppten Dateien
                //! (500 ms Staffelung, Ladevorgang laut Log aber 1-3,6 s) lief er
                //! dem tatsaechlich klingenden File dadurch voraus. Der Header
                //! wird jetzt reaktiv ueber PSndPlayer::sourceChanged() gesetzt,
                //! s. createPlayerDock().

                //! NEU: s. Chat "es wird keine Liste abgearbeitet" -- werden
                //! MEHRERE Dateien auf einmal gedroppt, direkt in DIESER
                //! Session gestaffelt nacheinander laden. Die ERSTE Datei
                //! laedt der normale GraphicsView-Drop-Pfad (publishdropEvent
                //! -> addFile) selbst -- GraphicsView::dropEvent() emittiert
                //! (seit dem Fix in PSndPlayer/waveform.cpp) nur noch fuer
                //! die erste URL; hier werden Datei 2..n nachgeschoben.
                //! Genau das stellt ProSoundFinders Bug-Szenario nach:
                //! mehrere Ladevorgaenge kurz nacheinander in einer
                //! laufenden Session.
                const QList<QUrl> urls = dropEvent->mimeData()->urls();
                int delayMs = PSNTEST_DROPPED_FILES_STAGGER_MS;
                for (int i = 1; i < urls.size(); ++i) {
                    const QString p = urls.at(i).toLocalFile();
                    if (p.isEmpty() || !QFileInfo::exists(p)) {
                        continue;
                    }
                    scheduleLoad(p, delayMs);
                    delayMs += PSNTEST_DROPPED_FILES_STAGGER_MS;
                }
            }
        }
    }
    //! Event nicht konsumieren -- muss normal an PlayerWidget/WaveformController
    //! weiterlaufen, damit das eigentliche Laden wie gehabt passiert.
    return QMainWindow::eventFilter(obj, event);
}

//! NEU: s. Doku im Header -- laedt nicht mehr nur die eine zuletzt gedroppte
//! Datei, sondern die rollierende Liste (s. eventFilter() QEvent::Drop),
//! gestaffelt kurz nacheinander. Simuliert ProSoundFinders eigentliches
//! Bug-Szenario: mehrere Dateien nacheinander in einer laufenden Session
//! laden (Tabellenklicks), statt nur den Einzelfall "eine Datei beim Start".
//! Faellt auf die alte Einzeldatei zurueck, falls die Liste (noch) leer ist
//! (z.B. bestehende Settings von vor dieser Aenderung).
void MainWindow::loadDroppedFiles()
{
    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    QStringList files = settings.value(PSNTEST_DROPPED_FILES_KEY).toStringList();
    const QString lastSingle = settings.value(PSNTEST_LAST_DROPPED_FILE_KEY).toString();
    settings.endGroup();

    if (files.isEmpty() && !lastSingle.isEmpty()) {
        files.append(lastSingle);
    }

    int delayMs = 0;
    for (const QString &path : files) {
        if (path.isEmpty() || !QFileInfo::exists(path)) {
            continue;
        }
        scheduleLoad(path, delayMs);
        delayMs += PSNTEST_DROPPED_FILES_STAGGER_MS;
    }
}

//! NEU: s. Doku im Header. Abbrechbare QTimer-Objekte statt
//! QTimer::singleShot() -- s. cancelPendingLoads(): ein neuer Nutzer-Drag
//! muss die noch ausstehenden automatischen Ladevorgaenge stoppen koennen
//! (Crash-Fix).
void MainWindow::scheduleLoad(const QString &path, int delayMs)
{
    QTimer *t = new QTimer(this);
    t->setSingleShot(true);
    connect(t, &QTimer::timeout, this, [this, path, t](){
        _pendingLoadTimers.removeAll(t);
        t->deleteLater();
        qDebug() << "MainWindow scheduleLoad: lade" << path;
        playerWidget->addFile(path);
        //! NEU: Der frueher hier stehende updatePlayerHeader(path)-Aufruf ist
        //! ENTFALLEN -- s. Begruendung im Drop-Zweig des eventFilter() und bei
        //! createPlayerDock(). Er setzte den Header auf die gerade angestossene
        //! Datei, obwohl deren Laden erst Sekunden spaeter fertig ist.
    });
    _pendingLoadTimers.append(t);
    t->start(delayMs);
}

//! NEU: s. Doku im Header (Header zeigt sonst die vorherige Datei).
void MainWindow::updatePlayerHeader(const QString &path)
{
    if (!playerHeader) {
        return;
    }
    playerHeader->setText(QFileInfo(path).fileName());
}

//! NEU: s. Doku im Header (Crash-Fix "SIGSEGV in GraphicsView::dropEvent").
void MainWindow::cancelPendingLoads()
{
    if (_pendingLoadTimers.isEmpty()) {
        return;
    }
    qDebug() << "MainWindow cancelPendingLoads: breche" << _pendingLoadTimers.size()
             << "ausstehende automatische Ladevorgaenge ab (Nutzer-Drop gewinnt)";
    for (QTimer *t : _pendingLoadTimers) {
        t->stop();
        t->deleteLater();
    }
    _pendingLoadTimers.clear();
}



//! erzeugt   Player Dock
void MainWindow::createPlayerDock()
{
    _playerDockWidget = new QDockWidget(tr("Player Window"),this);
    _playerDockWidget->setObjectName("playerDockWidget");

    _playerDockWidget->setContentsMargins(0,0,0,0);
    _playerDockWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    _playerDockWidget->setMinimumHeight(2);
    _playerDockWidget->setMinimumWidth(30);
    _playerDockWidget->setMaximumHeight(524287);
    _playerDockWidget->setMaximumWidth(524287);
    _playerDockWidget->setSizeIncrement(0,0);
    _playerDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    //! NEU: s. Chat "TestApp muss sich genauso verhalten wie ProSoundFinder" --
    //! 1:1 aus ProSoundFinder::MainWindow::createPlayerDockWidget()
    //! uebernommen (mainwindow.cpp dort, Zeile 2311-2319, "MainWindow"-Zweig).
    //! ProSoundFinder setzt hier VOR addDockWidget()/setWidget() (die erst
    //! spaeter unten passieren) eine manuelle, provisorische Geometrie mit
    //! hart codierter Hoehe -- PSNPlayerTestNew hatte das bisher gar nicht,
    //! was die beiden Apps strukturell unterschiedlich macht.
    QRect r1(0, 0, _playerDockWidget->parentWidget()->width()
             - _playerDockWidget->parentWidget()->contentsMargins().left()
             - _playerDockWidget->parentWidget()->contentsMargins().right(),
             250);
    _playerDockWidget->setGeometry(r1);
    _playerDockWidget->setVisible(true);

    if(this->playerWidget && this->playerWidget->isPlaying()){
        this->playerWidget->setWasPlaying(true);
        this->playerWidget->setStateToRemeber();
        this->playerWidget->pause();
    }

    playerWidget = new PlayerWidget(_playerDockWidget);
    playerWidget->setFocusPolicy(Qt::StrongFocus);

    qDebug() << "playerWidget->frameSize() " << playerWidget->frameSize();
    playerWidget->setObjectName("playerWidget");

    //! NEU: s. Chat "der header im player wird nicht upgedatet durch das
    //! aktuell hoerbare file". updatePlayerHeader() wurde bisher NUR beim
    //! ANSTOSSEN eines Ladevorgangs gerufen (nach dem Drop bzw. in
    //! scheduleLoad()) -- also push-basiert zum falschen Zeitpunkt. Bei
    //! gestaffelten Mehrfach-Ladevorgaengen setzte damit jeder Timer den
    //! Header auf die Datei, die er gerade anstoesst, waehrend hoerbar noch
    //! eine ganz andere lief; die zuletzt angestossene gewann. Aus demselben
    //! Grund half auch PlayerWidget::filePlayingStatus() nicht weiter -- das
    //! transportiert _qCurrentPath, und genau diese Variable wird beim
    //! Drop-Pfad (GraphicsView::dropEvent -> publishdropEvent ->
    //! WaveformController::on_publishdropEvent_received -> PSndPlayer::
    //! setSource(), ohne Umweg ueber PlayerWidget::addFile()) gar nicht
    //! gesetzt.
    //!
    //! Verlaesslich ist allein PSndPlayer::sourceChanged() -- laut dessen
    //! Doku (psndplayer.h) "always emitted when we did try to set a new
    //! Source and the result is ready". Wir haengen uns also direkt dort an
    //! und ziehen den Namen aus der gelieferten MediaSource. Der
    //! PSndPlayer ist ein QObject-Kind des PlayerWidget (s. dessen
    //! Konstruktor: mediaPlayer(new psndplayer::PSndPlayer(this))), das
    //! Member selbst aber privat -- findChild() ist deshalb der Weg, der
    //! OHNE Aenderung an ProSoundFinder auskommt.
    //!
    //! nullptr ist ein gueltiger Fall: sourceChanged() liefert ihn, wenn das
    //! Oeffnen fehlgeschlagen ist (s. Doku dort) -- dann den Header leeren,
    //! statt den alten Namen stehen zu lassen.
    if(psndplayer::PSndPlayer *mp = playerWidget->findChild<psndplayer::PSndPlayer*>()){
        connect(mp, &psndplayer::PSndPlayer::sourceChanged, this,
                [this](psndplayer::MediaSource *mediaSource){
            if(!mediaSource){
                updatePlayerHeader(QString());
                return;
            }
            updatePlayerHeader(QString::psnd_fromStdString(mediaSource->getFilename()));
        });
    }
    else{
        qWarning() << "MainWindow createPlayerDock: PSndPlayer-Kind im PlayerWidget nicht gefunden --"
                   << "Header wird nicht auf Quellwechsel aktualisiert.";
    }

    if(this->playerWidget->wasPlaying()){
        switch(this->playerWidget->rememberedState()){
        case PSNDPLAYER::PLAYING:
        case PSNDPLAYER::FAST_FORWARD:
            this->playerWidget->play();
            break;
        case PSNDPLAYER::REVERSE:
        case PSNDPLAYER::FAST_REVERSE:
            this->playerWidget->playRev();
            break;
        }


        //mainWin->playerWidget->play();
        this->playerWidget->setWasPlaying(false);
    }




    //playerWidget->setModel(_centralWidget->model());
    //playerWidget->setSortFilterProxyModel(_centralWidget->proxyModel());

    playerWidget->setContentsMargins(0,0,0,0);
    playerWidget->setMinimumWidth(50);
    playerWidget->setMinimumHeight(2);

    //! NEU: s. Doku beim r1/setGeometry() oben -- 1:1 aus ProSoundFinder
    //! uebernommen (mainwindow.cpp dort, Zeile 2349).
    playerWidget->show();
    playerWidget->setGeometry(_playerDockWidget->geometry());


    // to hide the title bar completely we must replace the default widget with a generic one
    playerHeader = new QLabel(_playerDockWidget);
    playerHeader->setAccessibleName("Player Window");
    playerHeader->setObjectName("playerHeader");//wichtig fuer z.b. Style Sheet
    playerHeader->setContentsMargins(0,0,0,0);

    playerHeader->setMinimumHeight(0);
    playerHeader->setMaximumHeight(1);
    //playerHeader->setFixedHeight(2);
    playerHeader->setMargin(1);
    playerHeader->setBuddy(_playerDockWidget);
    playerHeader->setVisible(false);
    _playerDockWidget->setTitleBarWidget(playerHeader);//hides the title Bar

    playerHeader->setAlignment(Qt::AlignCenter);

    this->addDockWidget(Qt::TopDockWidgetArea, _playerDockWidget);
    _playerDockWidget->setWidget(playerWidget);
    playerWidget->show();
    _playerDockWidget->show();

    //! s. Doku bei on_deviceChanged()/on_sampleRateChanged()/on_bitDepthChanged()
    //! im Header -- ohne diese Verbindungen bleibt die Auswahl im Device-/
    //! Samplerate-/Bittiefe-Dropdown rein kosmetisch.
    connect(playerWidget, SIGNAL(deviceChanged(int)),
            this, SLOT(on_deviceChanged(int)));
    connect(playerWidget, SIGNAL(sampleRateChanged(const QString &)),
            this, SLOT(on_sampleRateChanged(const QString &)));
    connect(playerWidget, SIGNAL(bitDepthChanged(const QString &)),
            this, SLOT(on_bitDepthChanged(const QString &)));
    connect(playerWidget, SIGNAL(channelOffsetChanged(const QString &)),
            this, SLOT(on_channelOffsetChanged(const QString &)));
    connect(playerWidget, SIGNAL(multichannelPassthroughChanged(bool)),
            this, SLOT(on_multichannelPassthroughChanged(bool)));
}

void MainWindow::on_deviceChanged(int id)
{
    qDebug() << "MainWindow on_deviceChanged" << id;
    playerWidget->activateOutputdevice(id);

    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    settings.setValue(PSNTEST_DEVICE_ID_KEY, id);
    settings.endGroup();
}

void MainWindow::on_sampleRateChanged(const QString &sr)
{
    qDebug() << "MainWindow on_sampleRateChanged" << sr;
    playerWidget->setSampleRate(sr);

    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    settings.setValue(PSNTEST_SAMPLERATE_KEY, sr);
    settings.endGroup();
}

void MainWindow::on_bitDepthChanged(const QString &bd)
{
    qDebug() << "MainWindow on_bitDepthChanged" << bd;
    //! Mapping 1:1 aus ProSoundFinders MainWindow::on_bitDepthChanged()
    //! (mainwindow.cpp:3797) uebernommen -- 132 = Float32, s. dortige Doku
    //! bei AudioOutput::setOutputBitDepth().
    bool isAuto = bd.startsWith("Auto");
    int bits = 16;
    if (bd.startsWith("32-bit Float")) bits = 132;
    else if (bd.startsWith("24")) bits = 32;

    if (isAuto)
        playerWidget->setOutputBitDepthAuto(true);
    else
        playerWidget->setOutputBitDepth(bits);

    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    settings.setValue(PSNTEST_BITDEPTH_KEY, bd);
    settings.endGroup();
}

//! NEU: s. Chat "RME-Mehrkanal-Routing" -- analog zu on_bitDepthChanged()
//! oben, aber einfacher: kein "Auto"-Modus, kein Reload (s. Doku bei
//! PlayerWidget::setOutputChannelOffset()). Text-Format "Channel N-M" wird
//! zurueck auf den 0-basierten Offset (N-1) geparst.
void MainWindow::on_channelOffsetChanged(const QString &co)
{
    qDebug() << "MainWindow on_channelOffsetChanged" << co;
    QStringList parts = co.section(' ', 1).split('-');
    if (!parts.isEmpty()) {
        bool ok = false;
        int firstChannel = parts.first().toInt(&ok);
        if (ok) {
            playerWidget->setOutputChannelOffset(firstChannel - 1);
        }
    }

    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    settings.setValue(PSNTEST_CHANNELOFFSET_KEY, co);
    settings.endGroup();
}

//! NEU: s. Chat "Vorhaben A -- umschaltbares Mehrkanal-Routing" -- analog zu
//! on_channelOffsetChanged() oben, aber einfacher: bool statt Text, kein
//! Parsing noetig.
void MainWindow::on_multichannelPassthroughChanged(bool enabled)
{
    qDebug() << "MainWindow on_multichannelPassthroughChanged" << enabled;
    playerWidget->setMultichannelPassthrough(enabled);

    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    settings.setValue(PSNTEST_PASSTHROUGH_KEY, enabled);
    settings.endGroup();
}

void MainWindow::loadOutputSettings()
{
    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    const bool hasDeviceId = settings.contains(PSNTEST_DEVICE_ID_KEY);
    const int deviceId = settings.value(PSNTEST_DEVICE_ID_KEY, -1).toInt();
    const QString samplerate = settings.value(PSNTEST_SAMPLERATE_KEY).toString();
    const QString bitDepthText = settings.value(PSNTEST_BITDEPTH_KEY).toString();
    const QString channelOffsetText = settings.value(PSNTEST_CHANNELOFFSET_KEY).toString();
    const bool hasPassthrough = settings.contains(PSNTEST_PASSTHROUGH_KEY);
    const bool passthrough = settings.value(PSNTEST_PASSTHROUGH_KEY, false).toBool();
    settings.endGroup();

    //! Reihenfolge bewusst: Geraet zuerst (aktiviert es und befuellt die
    //! Samplerate-Combobox neu, s. PlayerWidget::activateOutputdevice()),
    //! erst danach Samplerate/Bittiefe angewendet -- sonst wuerde
    //! activateOutputdevice() sie gleich wieder auf den Geraete-Default
    //! zuruecksetzen. Kanal-Offset danach, aus demselben Grund unkritisch
    //! (kein Reload/Stream-Neuaufbau, s. Doku bei PlayerWidget::
    //! setOutputChannelOffset()).
    if (hasDeviceId)
        on_deviceChanged(deviceId);
    if (!samplerate.isEmpty())
        on_sampleRateChanged(samplerate);
    if (!bitDepthText.isEmpty())
        on_bitDepthChanged(bitDepthText);
    if (!channelOffsetText.isEmpty())
        on_channelOffsetChanged(channelOffsetText);
    if (hasPassthrough)
        on_multichannelPassthroughChanged(passthrough);
}




void MainWindow::set_PlayerButton_checkedStates(QMap<QString,bool> playerButtonStates)
{
    qDebug() << "MainWindow set_PlayerButton_checkedStates " << playerButtonStates;
}

MainWindow::~MainWindow()
{
    delete ui;
}

