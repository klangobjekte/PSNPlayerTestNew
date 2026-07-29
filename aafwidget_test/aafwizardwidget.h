#ifndef PSNTESTNEW_AAFWIDGET_TEST_AAFWIZARDWIDGET_STUB_H
#define PSNTESTNEW_AAFWIDGET_TEST_AAFWIZARDWIDGET_STUB_H
//! Workaround-Stub innerhalb von PSNPlayerTestNew (s. CLAUDE.md -- ProSoundFinder
//! selbst darf von hier aus nicht veraendert werden).
//!
//! Hintergrund: playerwidgetV2.cpp (ProSoundFinder, wird bei uns live
//! mitkompiliert) macht "#include "mainwindow.h"" (Zeile 27) fuer einen
//! qobject_cast<MainWindow*>. Da playerwidgetV2.cpp physisch in
//! ProSoundFinder/ liegt, findet der Compiler bei diesem quoted Include
//! IMMER zuerst ProSoundFinder/mainwindow.h selbst (Standardverhalten von
//! MSVC/Clang/GCC: die Suche im Verzeichnis der inkludierenden Datei geht
//! JEDEM -I-Pfad vor) -- nicht unser eigenes, schlankes
//! PSNPlayerTestNew/mainwindow.h. Das laesst sich ueber INCLUDEPATH in der
//! .pro-Datei nicht umgehen.
//!
//! ProSoundFinder/mainwindow.h (Zeile 30) bindet seinerseits ungeschuetzt
//! "../aafwidget_test/aafwizardwidget.h" ein -- ein Geschwisterordner von
//! ProSoundFinder, der auf diesem Rechner nicht existiert. Laut Rueckfrage
//! beim Nutzer ist das dort inzwischen toter/veralteter Code (vermutlich
//! Ueberbleibsel von vor dem Umzug in den heutigen ProSoundFinder/AAF/-
//! Ordner) -- der eigentliche, richtige Fix waere, diesen Include in
//! ProSoundFinder/mainwindow.h selbst zu entfernen oder auf "#ifdef
//! __APPLE__" zu beschraenken. Das darf nur der Nutzer direkt in
//! ProSoundFinder vornehmen.
//!
//! Bis dahin: ProSoundFinder/mainwindow.h braucht "AafWizardWidget" nur
//! als Zeigerglied ("AafWizardWidget *m_aafWizard = nullptr;", Zeile 564,
//! nirgends dereferenziert innerhalb von mainwindow.h selbst) -- eine
//! reine Vorwaertsdeklaration genuegt daher zum Kompilieren.
//!
//! Aufgeloest wird diese Datei ueber den bereits vorhandenen INCLUDEPATH-
//! Eintrag "win_compat" (s. PSNPlayerTestNew.pro): "../aafwidget_test/
//! aafwizardwidget.h" relativ zu ".../PSNPlayerTestNew/win_compat"
//! ergibt ".../PSNPlayerTestNew/aafwidget_test/aafwizardwidget.h" -- exakt
//! dieser Pfad. Liegt also komplett innerhalb von PSNPlayerTestNew, nichts
//! an ProSoundFinder oder PSndPlayer wurde veraendert.
class AafWizardWidget;
#endif
