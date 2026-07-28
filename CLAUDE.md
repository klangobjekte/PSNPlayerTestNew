# Projektregeln für den AI-Agenten – PSNPlayerTestNew

## Kontext

- Dies ist ein **Testprojekt für PSndPlayer** – schneller kompilierbar als
  ProSoundFinder selbst, gedacht zum isolierten Testen/Debuggen von
  PSndPlayer-Funktionalität.
- Es **inkludiert/referenziert Dateien direkt aus ProSoundFinder** über
  relative Pfade (`../ProSoundFinder`) – es gibt keine eigenen Kopien.
- **Aktuelles Problem:** Der Build schlägt fehl, vermutlich weil sich
  Dateien in ProSoundFinder geändert haben, auf die dieses Projekt sich
  bezieht (z.B. geänderte Pfade, Funktionssignaturen, entfernte/umbenannte
  Dateien).

## Absolute Regel: Nur lesen, niemals schreiben in ProSoundFinder/PSndPlayer

- Dieses Projekt (PSNPlayerTestNew) darf **unter keinen Umständen** Dateien in
  `../ProSoundFinder` oder `../PSndPlayer` verändern, anlegen oder löschen –
  auch nicht nach Rückfrage oder mit ausdrücklicher Zustimmung des Nutzers
  innerhalb dieser Session.
- Diese beiden Verzeichnisse dienen ausschließlich als **Lesekontext** zur
  Diagnose. Jede Änderung, die dort nötig wäre, wird nur als Vorschlag/Text
  formuliert – niemals direkt umgesetzt.
- Falls ein Fix nur durch eine Änderung in ProSoundFinder oder PSndPlayer
  selbst möglich wäre: das explizit benennen und beschreiben, was dort zu
  tun wäre – aber diese Änderung dort selbst vornehmen kann nur der Nutzer,
  in einer eigenen Session direkt in dem jeweiligen Projekt.
- Alle Fixes für Build-Probleme werden ausschließlich innerhalb von
  PSNPlayerTestNew selbst umgesetzt (z.B. Include-Pfade, .pro/.pri-Dateien,
  CMake-Referenzen, eigene Wrapper/Kopien).

## Auftrag bei Build-Fehlern

1. Zuerst **analysieren, nicht sofort fixen**: welche konkreten Includes/
   Referenzen auf ProSoundFinder schlagen fehl, und was hat sich dort
   geändert (Datei verschoben, umbenannt, Signatur geändert, entfernt)?
2. Kurz zusammenfassen: was genau ist die Diskrepanz, und seit wann
   vermutlich (falls aus Git-Historie in ProSoundFinder ersichtlich).
3. **Bevorzugte Lösung:** Das Testprojekt (PSNPlayerTestNew) an den neuen
   Stand von ProSoundFinder anpassen (z.B. Include-Pfade, .pro/.pri-Dateien,
   CMake-Referenzen) – NICHT ProSoundFinder selbst verändern, um das
   Testprojekt zum Laufen zu bringen.
4. Falls ein Fix nur durch eine Änderung in ProSoundFinder oder PSndPlayer
   selbst möglich wäre: siehe Regel oben – niemals selbst dort schreiben,
   auch nicht mit Bestätigung. Nur benennen, was dort nötig wäre.
5. Änderungsvorschlag vor Anwendung kurz erklären: welche Dateien, warum,
   was sich dadurch am Verhalten/Build ändert.

## Git & Versionskontrolle

- **Niemals** `git commit`, `git push`, `git merge`, `git rebase` oder andere
  verlaufsverändernde Git-Kommandos selbstständig ausführen.
- `git add` / Staging nur nach expliziter Aufforderung durch den Nutzer.
- Commit-Nachrichten dürfen vorgeschlagen werden, aber der Commit selbst wird
  immer manuell vom Nutzer ausgeführt.

## Vorgehen bei Änderungen

- Nicht mehr Dateien anfassen als für den Fix nötig.
- Auch rein interne/private Details NICHT leichtfertig ändern – vor der
  Änderung kurz erklären, warum sie nötig ist und was sie betrifft.
- Keine großflächigen Refactorings "nebenbei", auch wenn sie sinnvoll
  erscheinen.
- Keine Builds/Terminalkommandos außerhalb dessen, was explizit angefragt
  wurde (außer zur reinen Diagnose, z.B. Datei suchen/lesen).

## Kommunikation

- Am Ende: kurze Zusammenfassung, was geändert wurde, ob ProSoundFinder
  davon betroffen ist, und ob der Build jetzt wieder funktionieren sollte.
