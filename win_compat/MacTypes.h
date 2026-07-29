#ifndef PSNPLAYERTESTNEW_WIN_COMPAT_MACTYPES_H
#define PSNPLAYERTESTNEW_WIN_COMPAT_MACTYPES_H
//! Windows-Ersatz fuer das macOS-Systemheader <MacTypes.h> (Carbon/
//! CoreServices), das unter Windows nicht existiert.
//!
//! Hintergrund: ProSoundFinder/timelabel.h bindet <MacTypes.h> UNGUARDED
//! ein (kein "#ifdef __APPLE__" drumherum -- anders als z.B. PSndPlayer/
//! BWFExport/tracksplitter.h, wo genau dieses Problem bereits behoben
//! wurde). Gebraucht wird von dort nur der Typ "Float64" (s. TimeLabel::
//! time2str()/setTime()/timeStringToDouble()/doubleClicked() in
//! timelabel.h). PSndPlayers eigenes psnddefinitions.h loest dasselbe
//! Problem unter Windows bereits per "#define Float64 double" -- dieser
//! Shim spiegelt das nur fuer diese eine Datei, die psnddefinitions.h
//! nicht selbst einbindet (bzw. es erst NACH timelabel.h tut).
//!
//! WICHTIG: Dies ist eine reine Workaround-Datei innerhalb von
//! PSNPlayerTestNew (s. CLAUDE.md -- ProSoundFinder/PSndPlayer selbst
//! duerfen von hier aus nicht veraendert werden). Der eigentliche Fix
//! gehoert in ProSoundFinder/timelabel.h selbst: den Include dort analog
//! zu tracksplitter.h auf "#ifdef __APPLE__" beschraenken. Das kann nur
//! der Nutzer direkt in einer eigenen ProSoundFinder-Session vornehmen.
#ifndef Float64
#define Float64 double
#endif
#endif // PSNPLAYERTESTNEW_WIN_COMPAT_MACTYPES_H
