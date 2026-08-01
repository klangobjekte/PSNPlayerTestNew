#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QLabel>
#include <QMap>
#include <QString>
#include <QList>

class PlayerWidget;
class QTimer;
class QAction;
class QActionGroup;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void createPlayerDock();
    ~MainWindow();

    //! Testprojekt-eigene Persistenz der zuletzt per Drag&Drop geladenen Datei
    //! (s. CLAUDE.md: nur hier in PSNPlayerTestNew, keine Aenderung an
    //! ProSoundFinder/PSndPlayer noetig). PlayerWidget/AbstractPlayerWidget
    //! haben keinen oeffentlichen Zugriff auf den vollen Pfad der geladenen
    //! Datei (_qCurrentPath ist protected, fileSpotted() liefert nur den
    //! gestrippten Namen) -- deshalb faengt MainWindow den QEvent::Drop
    //! stattdessen global ab, per Event-Filter auf qApp, bevor/waehrend er
    //! normal an das Waveform-Widget weitergereicht wird.
    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    //! Persistiert Fenstergeometrie + Dock-Layout (v.a. Groesse des
    //! Player-/Waveform-Docks) beim Schliessen, s. loadWindowGeometry().
    void closeEvent(QCloseEvent *event) override;

public slots:
    //! NEU: s. Chat "Polish-Mechanismus 1:1 aus ProSoundFinder uebernommen"
    //! (mainwindow.cpp dort, "Hack - Dragon", Zeile 771-783 + eventFilter()
    //! QEvent::Polish-Behandlung Zeile 2149-2166). Erzwingt einen echten
    //! Resize des Fensters 2 Sekunden nach dem ersten QEvent::Polish, um
    //! WaveformController zur Feinzeichnung mit der endgueltigen Fenstergroesse
    //! zu bewegen -- bewusst der bewaehrte, unveraenderte Mechanismus statt
    //! eines weiteren, ungetesteten Timing-Ansatzes (s. Chat: mehrere Tage
    //! erfolglose Alternativversuche in PSndPlayer selbst).
    void viewDidLoad();

    //! wird von PlayerWidget aufgerufen, um Menu/Action-Texte an den
    //! aktuellen Sichtbarkeitsstatus der Player-Controls anzupassen
    //! (siehe PSndPlayerTestNew CLAUDE.md: benoetigt fuer PlayerWidget-Konstruktor)
    void set_PlayerButton_checkedStates(QMap<QString,bool> playerButtonStates);

    //! NEU: PlayerWidget::on_devicesComboBox_activated()/on_sampleratesComboBox_
    //! activated()/on_bitDepthComboBox_activated() wenden eine Dropdown-Auswahl
    //! NICHT selbst an, sondern emittieren nur deviceChanged()/sampleRateChanged()/
    //! bitDepthChanged() (s. playerwidgetV2.cpp) -- das tatsaechliche Anwenden
    //! (und damit Aktualisieren von _currentOutputDeviceId/_currentSamplerate/
    //! _currentOutputBitDepth, die schliesslich in writeSettings() landen)
    //! passiert erst hier, analog zu MainWindow::on_deviceChanged()/
    //! on_sampleRateChanged()/on_bitDepthChanged() in ProSoundFinders
    //! mainwindow.cpp. Ohne diese drei Slots bleibt die Dropdown-Auswahl rein
    //! kosmetisch und wird beim naechsten Start nicht wiederhergestellt.
    void on_deviceChanged(int id);
    void on_sampleRateChanged(const QString &sr);
    void on_bitDepthChanged(const QString &bd);

private:
    //! NEU: s. Chat "Mehrfach-Lade-Test" -- laedt nicht mehr nur die eine
    //! zuletzt gedroppte Datei, sondern eine kurze Liste (kurz zeitversetzt
    //! nacheinander), um ProSoundFinders eigentliches Bug-Szenario (Klick auf
    //! mehrere Tabelleneintraege nacheinander in einer laufenden Session)
    //! nachzustellen -- das einmalige Laden EINER Datei beim Start konnte das
    //! nie testen.
    void loadDroppedFiles();
    //! NEU: gemeinsamer Helfer fuer loadDroppedFiles() und den Multi-Drop-
    //! Zweig in eventFilter() -- legt einen abbrechbaren Lade-Timer an
    //! (s. cancelPendingLoads()).
    void scheduleLoad(const QString &path, int delayMs);
    //! NEU: s. Chat "Header zeigt nicht die geladene Datei". Ursache (Stand
    //! aktuelles ProSoundFinder/PSndPlayer, von hier aus nicht aenderbar, s.
    //! CLAUDE.md): ein normaler Drag&Drop-Drop auf die Waveform-Ansicht laeuft
    //! ueber GraphicsView::dropEvent() (PSndPlayer/waveform.cpp) ->
    //! WaveformController::on_publishdropEvent_received()
    //! (waveformcontroller.cpp), das die Quelle OHNE Umweg ueber PlayerWidget::
    //! addFile() direkt an PSndPlayer::setSource() weiterreicht.
    //! PlayerWidget::_qCurrentPath (aus dem on_Source_Changed() den Dock-Header
    //! beschriftet, s. playerwidgetV2.cpp) wird aber NUR von addFile()/
    //! addFileRecursion() gesetzt -- bleibt bei einem Drop also auf dem alten
    //! Pfad stehen (der Tabellenklick-Pfad addFileRecursion() ruft direkt
    //! addFile()-Logik auf, deshalb faellt es in ProSoundFinder beim normalen
    //! Bedienweg nie auf). Workaround in der TestApp: wir kennen bei jedem
    //! Ladevorgang den Pfad selbst und setzen das Header-Label nach dem Laden
    //! explizit nach.
    void updatePlayerHeader(const QString &path);
    //! NEU: s. Crash-Diagnose "SIGSEGV in GraphicsView::dropEvent nach
    //! Drag&Drop". Die gestaffelten Lade-Timer aus loadDroppedFiles()
    //! feuerten blind weiter, waehrend der Nutzer per Drag&Drop interagierte --
    //! jeder addFile()-Aufruf zerstoert aber die GraphicsViews der vorherigen
    //! Datei (WaveformController::unregisterCurrentWaveform(), "delete
    //! waveform"). Wurde eine View mitten in einem laufenden macOS-Drag
    //! geloescht, lieferte das System den Drop trotzdem an das freigegebene
    //! Widget aus -> Use-after-free-Crash beim ersten Signal-Emit. Deshalb:
    //! sobald der Nutzer selbst droppt, werden alle noch ausstehenden
    //! automatischen Ladevorgaenge abgebrochen -- Nutzeraktion gewinnt.
    void cancelPendingLoads();
    //! NEU: Menue mit den Player-relevanten Actions aus ProSoundFinders
    //! MainWindow, damit sich in der Testanwendung dieselben Betriebsarten
    //! umschalten lassen -- vor allem Scrollverhalten (No/Page/Continuous) und
    //! Continuous Play. ProSoundFinders MainWindow hat rund hundert Actions,
    //! die ueberwiegend an Datenbank/Archiv/Bin haengen; hier wird bewusst NUR
    //! der Player-/Waveform-Teil nachgebildet, und zwar ueber dieselben
    //! oeffentlichen Aufrufe wie dort (PlayerWidget::setScrollType(),
    //! ::setRepeat(), PreferencesControl::setPauseOnStop() usw.) -- damit
    //! testet die App denselben Weg, den ProSoundFinder auch nimmt.
    //! Muss NACH createPlayerDock() laufen, da alle Actions playerWidget
    //! brauchen.
    void createMenus();
    //! Haelt die vier Transport-Actions mit dem tatsaechlichen Player-Zustand
    //! in Deckung (PlayerWidget::setPlayerActions ist das Signal, das
    //! ProSoundFinders MainWindow dafuer benutzt).
    void syncTransportActions(int state);
    //! Haakchen der beiden Preference-Actions aus PreferencesControl
    //! nachziehen -- die Werte koennen auch ueber den PreferencesDialog
    //! geaendert werden.
    void syncPreferenceActions();

    //! Letzter ueber PlayerWidget::setPlayerActions gemeldeter Zustand.
    //! -1 = noch unbekannt (wird wie "steht" behandelt).
    int _lastPlayerState = -1;

    QAction *_playAction = nullptr;
    QAction *_playReverseAction = nullptr;
    QAction *_pauseAction = nullptr;
    QAction *_stopAction = nullptr;
    QAction *_repeatAction = nullptr;
    QAction *_soloAction = nullptr;
    QAction *_noScrollingAction = nullptr;
    QAction *_pageScrollingAction = nullptr;
    QAction *_continuousScrollingAction = nullptr;
    QAction *_cursorFollowsPlaybackAction = nullptr;
    QAction *_sequentialPlayAction = nullptr;
    QActionGroup *_transportGroup = nullptr;
    QActionGroup *_scrollingGroup = nullptr;

    void loadWindowGeometry();
    //! NEU: PlayerWidget::writeSettings() persistiert nur currentOutputDeviceId,
    //! NICHT currentOutputBitDepth/outputBitDepthAuto/currentSamplerate (Bug in
    //! ProSoundFinder, s. playerwidgetV2.cpp:3153 -- kann/darf hier nicht behoben
    //! werden). _currentOutputBitDepth/_currentSamplerate sind zudem private in
    //! PlayerWidget, kein Zugriff von aussen moeglich. Deshalb eigene, von
    //! PlayerWidgets readSettings()/writeSettings() unabhaengige Persistenz: wir
    //! speichern die Werte selbst in on_deviceChanged()/on_sampleRateChanged()/
    //! on_bitDepthChanged() und wenden sie hier beim Start erneut ueber dieselben
    //! (bereits oeffentlichen) Slots an -- kein Zugriff auf PlayerWidget-Interna
    //! noetig, loest nebenbei auch den "Samplerate wird nie angewendet"-Bug, weil
    //! wir den PlayerWidget-Konstruktor dafuer gar nicht mehr brauchen.
    void loadOutputSettings();

    Ui::MainWindow *ui;
        PlayerWidget *playerWidget=nullptr;
        QDockWidget *_playerDockWidget=nullptr;
        QLabel *playerHeader=nullptr;
        //! NEU: s. viewDidLoad() -- verhindert Mehrfachauslösung, falls
        //! QEvent::Polish erneut feuert (dieselbe Absicherung wie
        //! ProSoundFinders _viewDidLoad, mainwindow.h dort).
        bool _viewDidLoad = false;
        //! NEU: s. cancelPendingLoads() -- die noch nicht gefeuerten,
        //! gestaffelten Lade-Timer aus loadDroppedFiles().
        QList<QTimer*> _pendingLoadTimers;

};
#endif // MAINWINDOW_H
