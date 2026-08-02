# Scrub: Stocken + zu viel Mausweg nötig

Beobachtung beim Testen (Cmd+Alt+Drag / Ctrl+Alt+Drag in der Wellenform):
"Nachlauf" ist weg (gut), aber es stockt wieder beim Ziehen, und es braucht
extreme Mausausschläge, damit sich überhaupt etwas bewegt/verändert.

Betroffene Datei: `playerwidgetV2.cpp` (ca. Zeile 550-635), Scrub-Funktion.

## 1) Stocken beim Ziehen

`SCRUB_STILLNESS_TIMEOUT_MS = 200` (Zeile 564).

`_scrubStillnessTimer` wird bei **jeder** `scrubMoved()`-Mausbewegung neu
gestartet. Kommt 200ms lang kein neues Mausevent, gilt das als "Maus steht
still" -> `mediaPlayer->pause()`. Aus dem Log:

```
[SCRUBDIAG-AUDIO] retuneStreamForSpeed FAST-PATH speed: 1 alreadyActive: false stop_ms: 126 start_ms: 197 TOTAL_ms: 323 ok: true
PSndPlayer pause ...
PSndPlayer play ... startPlay ...
```

Wiederholte `pause`/`play`/`startPlay`-Zyklen mitten im Scrub, jeweils mit
~300ms blockierendem PortAudio-Stream-Reopen dazwischen (`TOTAL_ms: 323`) --
das fühlt sich wie Stocken an. Wenn die Mausbewegungs-Events seltener als
alle 200ms ankommen (Timing-/Event-Delivery-abhängig), triggert der
Stillstands-Timer viel öfter als beabsichtigt.

**Ansatzpunkt:** `SCRUB_STILLNESS_TIMEOUT_MS` hochsetzen (z.B. 350-400ms),
falls das vorzeitige Pausieren das Stocken verursacht. Ggf. auch prüfen, ob
Mausbewegungs-Events irgendwo gedrosselt/verzögert ankommen (Event-
Kompression, Widget-Verschachtelungstiefe).

## 2) Zu viel Mausweg nötig

`SCRUB_ZONES` (Zeile 582-587):

```cpp
const QVector<ScrubZone> SCRUB_ZONES = {
    {60.0,  SCRUB_MIN_SPEED},   // 0.15x
    {150.0, 0.4},
    {260.0, 0.7},
    {1.0e18, SCRUB_MAX_SPEED},  // 1.0x
};
```

Reine Pixel-Distanz-Schwellen vom Klickpunkt, unabhängig von Fenstergröße
oder Zoomstufe der Wellenform. 60px für die langsamste Stufe, erst ab 260px
volle Geschwindigkeit (1.0x, bewusst gedeckelt -- Scrub soll zum Finden von
Schnittpunkten dienen, nicht zur schnellen Navigation, s. Kommentar bei
`SCRUB_MAX_SPEED`).

**Ansatzpunkt:** Schwellenwerte (60.0 / 150.0 / 260.0) verkleinern, falls
weniger Mausweg für denselben Geschwindigkeitseffekt gewünscht ist.

## Einordnung

Beides sind reine Tuning-Konstanten in PSndPlayer/ProSoundFinder, keine
Logikfehler. Nicht in PSNPlayerTestNew änderbar/relevant -- Anpassung muss
direkt in `playerwidgetV2.cpp` (ProSoundFinder) erfolgen.
