@echo off
setlocal
rem Wird ausschliesslich von PSNPlayerTestNew.pro per QMAKE_POST_LINK
rem aufgerufen (nur im win32/equals(QT_ARCH, arm64)-Zweig).
rem
rem Hintergrund: Gelinkt wird gegen .lib-Importbibliotheken (PSndPlayer1.lib,
rem sndfile.lib, ...) -- die eigentlichen .dll-Dateien braucht Windows davon
rem unabhaengig ERST BEIM PROGRAMMSTART, und sucht dafuer zuerst im Ordner
rem der .exe selbst. Ohne diesen Kopierschritt liefen Compile+Link zwar
rem erfolgreich durch, der Start von PSNPlayerTestNew.exe scheitert aber mit
rem "... .dll wurde nicht gefunden". Kopiert werden:
rem   - PSndPlayer1.dll (PSndPlayer, s. PSNDPLAYER_BUILD in der .pro-Datei)
rem   - mOPAtomReader.dll (von PSndPlayer transitiv gebraucht -- anders als
rem     bmx/libMXF/libMXF++/uriparser/expat/libsfmeta, die alle rein
rem     statisch sind, s. PSndPlayer/WINDOWS_ARM64_PORT_STATUS.md
rem     "Bekannte Restrisiken": dort als unverifiziertes Risiko benannt,
rem     hier verifiziert -- mOPAtomReader.dll existiert tatsaechlich neben
rem     mOPAtomReader.lib)
rem   - alle .dll's aus dem vcpkg-arm64-windows-bin-Ordner (sndfile.dll
rem     direkt gebraucht; die uebrigen dort liegenden DLLs -- portaudio,
rem     mpg123, FLAC, ogg, vorbis, opus, libmp3lame, boost_* usw. -- sind
rem     nicht einzeln als transitive Abhaengigkeit von PSndPlayer1.dll
rem     verifiziert, werden aber vorsorglich mitkopiert, um nicht bei jeder
rem     einzelnen einen eigenen Fehler-/Fix-Rundgang zu brauchen; reines
rem     Kopieren, kein Risiko)
rem
rem Reiner Deployment-Schritt innerhalb von PSNPlayerTestNew -- kopiert nur
rem bereits vorhandene Dateien, veraendert nichts an PSndPlayer/
rem ProSoundFinder/mOPAtomReader/vcpkg selbst.
rem
rem Aufruf (aus der .pro-Datei): deploy_runtime_dlls.bat <ZielOrdner> <debug|release>

set "DEST=%~1"
set "CFG=%~2"

if /I "%CFG%"=="debug" (
    set "PSNDPLAYER_DLL=C:\Users\admin\Developer\PSndPlayer\build\Desktop_Qt_6_11_1_MSVC2022_ARM64_Debug\debug\PSndPlayer1.dll"
    set "MOPATOM_DLL=C:\Users\admin\Developer\mOPAtomReader\build\Desktop_Qt_6_11_1_MSVC2022_ARM64_Debug\debug\mOPAtomReader.dll"
    set "VCPKG_BIN=C:\Users\admin\Developer\vcpkg\installed\arm64-windows\debug\bin"
) else (
    set "PSNDPLAYER_DLL=C:\Users\admin\Developer\PSndPlayer\build\Desktop_Qt_6_11_1_MSVC2022_ARM64_Debug\release\PSndPlayer1.dll"
    set "MOPATOM_DLL=C:\Users\admin\Developer\mOPAtomReader\build\Desktop_Qt_6_11_1_MSVC2022_ARM64_Release\release\mOPAtomReader.dll"
    set "VCPKG_BIN=C:\Users\admin\Developer\vcpkg\installed\arm64-windows\bin"
)

if not exist "%DEST%" mkdir "%DEST%" >nul 2>&1

rem ---------------------------------------------------------------------------
rem DEAKTIVIERT: PSndPlayer1.dll wird NICHT mehr hierher kopiert.
rem
rem Grund: Dieses Skript laeuft per QMAKE_POST_LINK (s. PSNPlayerTestNew.pro)
rem als letzter Befehl der LINK-Regel von PSNPlayerTestNew.exe
rem (Makefile.Debug/Makefile.Release, jeweils Zeile ~298). nmake fuehrt diese
rem Regel nur aus, wenn die .exe selbst neu gelinkt werden muss -- PSndPlayer1.dll
rem taucht in den Abhaengigkeiten dieses Makefiles gar nicht auf. Ein reiner
rem Rebuild von PSndPlayer loeste den Kopierschritt also NICHT aus, und die App
rem lief weiter gegen eine veraltete DLL. Schlimmer noch: das zugehoerige PDB im
rem PSndPlayer-Build-Ordner wurde vom Rebuild ueberschrieben und passte dann
rem nicht mehr zur alten DLL-Kopie -- Folge waren unbrauchbare Stacktraces, in
rem denen der Debugger auf das naechstgelegene exportierte Symbol zurueckfiel
rem (beobachtet: drei voellig verschiedene Adressen alle als
rem "AudioReverse::~AudioReverse" ausgewiesen).
rem
rem Stattdessen wird PSndPlayer1.dll jetzt ueber PATH aus ihrem Build-Verzeichnis
rem geladen (Qt Creator: Projekte -> Ausfuehren -> Umgebung -> PATH, den Ordner
rem C:\Users\admin\Developer\PSndPlayer\build\Desktop_Qt_6_11_1_MSVC2022_ARM64_Debug\debug
rem voranstellen). Damit ist immer der frischeste Build aktiv, ohne Kopierschritt,
rem und DLL und PDB passen automatisch zusammen.
rem
rem WICHTIG: Es darf keine Kopie von PSndPlayer1.dll neben der .exe liegen --
rem Windows durchsucht das Verzeichnis der .exe VOR dem PATH, eine alte Kopie
rem wuerde also weiterhin gewinnen.
rem
rem Rueckgaengig: einfach die vier "rem"-Praefixe unten entfernen. Die Zeilen
rem set "PSNDPLAYER_DLL=..." weiter oben stehen bewusst noch da.
rem ---------------------------------------------------------------------------
rem if exist "%PSNDPLAYER_DLL%" (
rem     copy /y "%PSNDPLAYER_DLL%" "%DEST%" >nul
rem ) else (
rem     echo [deploy_runtime_dlls] WARNUNG: %PSNDPLAYER_DLL% nicht gefunden -- PSndPlayer zuerst fuer %CFG% bauen.
rem )

rem ---------------------------------------------------------------------------
rem DEAKTIVIERT: mOPAtomReader.dll wird NICHT mehr hierher kopiert -- gleiche
rem Begruendung wie bei PSndPlayer1.dll oben (Kopie veraltet still, weil diese
rem Regel nur beim Linken der .exe laeuft). Solange mOPAtomReader unter Windows
rem aktiv getestet/geaendert wird, ist der PATH-Weg der sichere.
rem
rem PATH-Eintrag (Qt Creator: Projekte -> Ausfuehren -> Umgebung -> PATH):
rem   C:\Users\admin\Developer\mOPAtomReader\build\Desktop_Qt_6_11_1_MSVC2022_ARM64_Debug\debug
rem Fuer Release liegt sie abweichend unter ...MSVC2022_ARM64_Release\release\.
rem
rem NICHT betroffen: meta_mp. Dessen libsfmeta.lib ist eine STATISCHE Bibliothek
rem (7,7 MB Objektcode, kein DLL-Pendant vorhanden) und wird beim Linken
rem vollstaendig in PSndPlayer1.dll aufgenommen -- dort gibt es nichts zu
rem deployen und nichts in den PATH zu setzen.
rem
rem Rueckgaengig: die vier "rem"-Praefixe unten entfernen.
rem ---------------------------------------------------------------------------
rem if exist "%MOPATOM_DLL%" (
rem     copy /y "%MOPATOM_DLL%" "%DEST%" >nul
rem ) else (
rem     echo [deploy_runtime_dlls] WARNUNG: %MOPATOM_DLL% nicht gefunden -- mOPAtomReader zuerst fuer %CFG% bauen.
rem )

if exist "%VCPKG_BIN%\*.dll" (
    xcopy /y /q "%VCPKG_BIN%\*.dll" "%DEST%\" >nul
) else (
    echo [deploy_runtime_dlls] WARNUNG: keine DLLs in %VCPKG_BIN% gefunden.
)

endlocal
exit /b 0
