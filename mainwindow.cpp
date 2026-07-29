#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "abstractplayerwidget.h"
#include "playerwidgetV2.h"
#include "psfdefinitions.h"
#include "preferencesdialog.h"
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

void MainWindow::loadOutputSettings()
{
    QSettings settings;
    settings.beginGroup(PSNTEST_SETTINGS_GROUP);
    const bool hasDeviceId = settings.contains(PSNTEST_DEVICE_ID_KEY);
    const int deviceId = settings.value(PSNTEST_DEVICE_ID_KEY, -1).toInt();
    const QString samplerate = settings.value(PSNTEST_SAMPLERATE_KEY).toString();
    const QString bitDepthText = settings.value(PSNTEST_BITDEPTH_KEY).toString();
    settings.endGroup();

    //! Reihenfolge bewusst: Geraet zuerst (aktiviert es und befuellt die
    //! Samplerate-Combobox neu, s. PlayerWidget::activateOutputdevice()),
    //! erst danach Samplerate/Bittiefe angewendet -- sonst wuerde
    //! activateOutputdevice() sie gleich wieder auf den Geraete-Default
    //! zuruecksetzen.
    if (hasDeviceId)
        on_deviceChanged(deviceId);
    if (!samplerate.isEmpty())
        on_sampleRateChanged(samplerate);
    if (!bitDepthText.isEmpty())
        on_bitDepthChanged(bitDepthText);
}




void MainWindow::set_PlayerButton_checkedStates(QMap<QString,bool> playerButtonStates)
{
    qDebug() << "MainWindow set_PlayerButton_checkedStates " << playerButtonStates;
}

MainWindow::~MainWindow()
{
    delete ui;
}

