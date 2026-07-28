//! Minimal-Implementierungen fuer die Handvoll CustomTableModel-/Export-
//! Methoden, die playerwidgetV2.cpp/abstractplayerwidget.cpp (jetzt live aus
//! ../ProSoundFinder referenziert, s. PSNPlayerTestNew.pro) tatsaechlich
//! aufrufen. abstractplayerwidget.h liegt in ../ProSoundFinder und loest sein
//! #include "customtablemodel.h" relativ zu seinem eigenen Verzeichnis auf --
//! landet also bei ProSoundFinders ECHTER, SQL/DB-gestuetzter
//! customtablemodel.h/Export/export.h, nicht bei den (dadurch toten) lokalen
//! Stub-Headern, die dieses Projekt vorher hatte. Die echten .cpp-Dateien
//! dafuer wollen wir bewusst NICHT mitbauen (kompletter SQL/DB-Stack, genau
//! das soll dieses Testprojekt laut CLAUDE.md vermeiden) -- deshalb hier
//! No-Op-Definitionen gegen die ECHTEN Klassendeklarationen (sonst ODR-
//! Verletzung durch zwei verschiedene Klassenlayouts unter selbem Namen).
//! m_CustomTableModel bleibt in diesem Testprojekt immer nullptr (setModel()
//! wird nie aufgerufen), diese Methoden werden also nie wirklich erreicht --
//! muessen nur linken, nicht sinnvoll arbeiten.
//! CustomTableModel/CustomSortFilterProxModel sind Q_OBJECT-Klassen; sobald
//! MOC ihr Meta-Objekt braucht (hier: wegen der setModel(CustomTableModel*)/
//! setSortFilterProxyModel(CustomSortFilterProxModel*)-Slot-Signaturen in
//! AbstractPlayerWidget, s. moc_abstractplayerwidget.cpp), generiert MOC die
//! komplette qt_static_metacall-Dispatchtabelle fuer ALLE eigenen Slots der
//! Klasse, nicht nur die von PlayerWidget tatsaechlich aufgerufenen. Deshalb
//! muessen hier alle "public slots:" beider Klassen No-Op-Definitionen
//! bekommen, sonst bricht der Linker Symbol fuer Symbol ab.
#include "../ProSoundFinder/customtablemodel.h"
#include "../ProSoundFinder/customsortfilterproxmodel.h"
#include "../ProSoundFinder/mimedata.h"
#include "../ProSoundFinder/Export/export.h"

#include <QItemSelection>
#include <QStringList>
#include <QUrl>
#include <QSqlIndex>
#include <QSqlTableModel>
#include <QPoint>

int CustomTableModel::columnCount() const
{
    return 0;
}

int CustomTableModel::columnNamesToNumber(const QString /*columnName*/, const QString /*tableName*/) const
{
    return -1;
}

QString CustomTableModel::makePath(const QString filePath, const QString /*existingRelativePath*/, const QString /*archivPath*/) const
{
    return filePath;
}

QItemSelection CustomTableModel::getItemSelection()
{
    return QItemSelection();
}

bool CustomTableModel::getArchives(QStringList &/*allArchivePaths*/)
{
    return false;
}

QList<QUrl> Export::dragSelection(psndplayer::MediaSource */*mediaSource*/,
                                   double /*copystart*/, double /*copyduration*/,
                                   QHash<QString, std::vector<int>> /*selectedChannels*/,
                                   std::pair<QString, bool> /*soloEnabled*/)
{
    return QList<QUrl>();
}

bool Export::sendSelection(psndplayer::MediaSource */*mediaSource*/,
                            double /*copystart*/, double /*copyduration*/,
                            QHash<QString, std::vector<int>> /*selectedChannels*/,
                            std::pair<QString, bool> /*soloEnabled*/)
{
    return false;
}

bool Export::spotFiles(psndplayer::MediaSource */*mediaSource*/, double /*copyduration*/, double /*copystart*/,
                        QHash<QString, std::vector<int>> /*selectedChannels*/)
{
    return false;
}

QString Export::doCopySelection() const
{
    return QString();
}

bool Export::doSendSelection() const
{
    return false;
}

//! ---- CustomTableModel: restliche "public slots:" (s. Kommentar oben) ----

bool CustomTableModel::submitAll() { return false; }
bool CustomTableModel::submit() { return false; }
void CustomTableModel::revertAll() {}
void CustomTableModel::revert() {}
void CustomTableModel::moveDestiantionRow(QModelIndex) {}
void CustomTableModel::leaveDestiantionRow(QModelIndex) {}
void CustomTableModel::enterDestinationRow(QModelIndex) {}
void CustomTableModel::setIndexAtMousePress(QModelIndex) {}
void CustomTableModel::indexAtMouseRelease(QModelIndex) {}
bool CustomTableModel::setDataForRow(const QList<QVariant> &, psfArchiveEntry &, const int) { return false; }
bool CustomTableModel::copyFiles(QString, QString, bool) { return false; }
void CustomTableModel::setCustomMimeData(MimeData *) {}
void CustomTableModel::setSelectedIndexes(QModelIndexList) {}
void CustomTableModel::setItemSelection(QItemSelection) {}
void CustomTableModel::copyClipboard() {}
int CustomTableModel::writeRowToDb(const QList<QVariant>, psfArchiveEntry &) { return -1; }
bool CustomTableModel::writeImportRowToDb(const QList<QVariant>) { return false; }
bool CustomTableModel::insertEmptyRow() { return false; }
void CustomTableModel::resetModel() {}
void CustomTableModel::applyPartialFilter(const QString &, const QString &, int, bool, bool, bool) {}
bool CustomTableModel::updateModelByPath(QString) { return false; }
bool CustomTableModel::updateModelByPath(const QStringList &) { return false; }
QModelIndex CustomTableModel::indexOfGeneratedPathOfSingleRow(const QString) { return QModelIndex(); }
QModelIndex CustomTableModel::indexOfPathOfSingleRow(const QString) { return QModelIndex(); }
QModelIndex CustomTableModel::indexOfPathOfSingleRowThreaded(const QString) { return QModelIndex(); }
void CustomTableModel::setSelectedIndexesForSingleRow(const QString) {}
void CustomTableModel::setSelectedIndexesForMultiRows(const QStringList) {}
QString CustomTableModel::tableName() const { return QString(); }
QSqlIndex CustomTableModel::primaryKey() const { return QSqlIndex(); }
void CustomTableModel::setEditStrategy(QSqlTableModel::EditStrategy) {}
bool CustomTableModel::addDataFromFile(const QString &, psfArchiveEntry &, bool) { return false; }
void CustomTableModel::setUrls(QList<QUrl>) {}
void CustomTableModel::sendMouseReleaseEvent(const QPoint &) const {}
bool CustomTableModel::deleteBinContent(const QString &) { return false; }
void CustomTableModel::on_SqlSelectThread_finished() {}
void CustomTableModel::on_SqlSelectThread_selectFinished() {}
void CustomTableModel::on_SqlSelectThread_destroyed() {}
void CustomTableModel::on_UpdateFieldInTableThread_finished() {}
void CustomTableModel::on_sqlUpdateSoundfilesFieldThread_resultReady() {}
void CustomTableModel::on_lookForPathThread_resultReady(const QString, const QModelIndex &) {}

//! ---- CustomTableModel: Destruktor + alle uebrigen public/protected
//! Methoden ohne Inline-Body (Overrides von QAbstractItemModel wie rowCount/
//! columnCount/data/flags/mimeData/... werden fuer die Vtable gebraucht,
//! sobald das Meta-Objekt einmal vollstaendig gemoct wird -- der Rest ist
//! nur deklariert und wuerde sonst als "referenced from vtable" auftauchen).
//! Vollstaendig gegen die reale Klassendeklaration abgeglichen (nicht nur
//! Symbol-fuer-Symbol auf Linker-Fehler reagiert).
CustomTableModel::~CustomTableModel() {}
void CustomTableModel::showWarnings(bool) {}
void CustomTableModel::setConnectionName(QString) {}
void CustomTableModel::setColumnsList(QStringList) {}
void CustomTableModel::setColumnsList(const QString &) {}
void CustomTableModel::setEmptyModel() {}
QStringList CustomTableModel::getAllTableNames() { return QStringList(); }
QModelIndex CustomTableModel::index(int, int, QModelIndex) const { return QModelIndex(); }
Qt::DropActions CustomTableModel::supportedDropActions() const { return {}; }
Qt::DropActions CustomTableModel::supportedDragActions() const { return {}; }
Qt::KeyboardModifiers CustomTableModel::keyboardModifiers(QDropEvent *) const { return {}; }
Qt::ItemFlags CustomTableModel::flags(const QModelIndex &) const { return {}; }
QStringList CustomTableModel::mimeTypes() const { return QStringList(); }
bool CustomTableModel::dropMimeData(const QMimeData *, Qt::DropAction, int, int, const QModelIndex &) { return false; }
MimeData *CustomTableModel::mimeData(const QModelIndexList &) const { return nullptr; }
MimeData *CustomTableModel::mimeData() { return nullptr; }
void CustomTableModel::mimeDataSetNull() {}
void CustomTableModel::clearRelations() {}
void CustomTableModel::setInitialRelations() {}
void CustomTableModel::setRelation(int, const QString &, const QSqlRelation &) {}
void CustomTableModel::setTempColumns() {}
QSqlRelation CustomTableModel::relationAtColumn(int) const { return QSqlRelation(); }
int CustomTableModel::setMyQuery(QStringList, QString) { return -1; }
int CustomTableModel::setCustomData(int, int, QStringList) { return -1; }
QStringList CustomTableModel::fieldNames(QString) { return QStringList(); }
QStringList CustomTableModel::getColumnsList() { return QStringList(); }
int CustomTableModel::fieldIndex(QString, QString) { return -1; }
QSqlRecord CustomTableModel::record(int) const { return QSqlRecord(); }
QSqlRecord CustomTableModel::record() const { return QSqlRecord(); }
int CustomTableModel::rowCount(const QModelIndex &) const { return 0; }
int CustomTableModel::rowCount() { return 0; }
void CustomTableModel::upDateRowCount() {}
int CustomTableModel::columnCount(const QModelIndex &) const { return 0; }
QVariant CustomTableModel::data(const QModelIndex &, int) const { return QVariant(); }
QVariant CustomTableModel::headerData(int, Qt::Orientation, int) const { return QVariant(); }
bool CustomTableModel::setHeaderData(int, Qt::Orientation, const QVariant &, int) { return false; }
bool CustomTableModel::setData(const QModelIndex &, const QString &, int) { return false; }
bool CustomTableModel::writeDataForRows(const QModelIndex &, const QVariantList &) { return false; }
bool CustomTableModel::upDateData(const QModelIndex &, const QString &, int) { return false; }
bool CustomTableModel::upDateData(const QString, int, QString) { return false; }
QList<QStringList> CustomTableModel::getRowList() { return QList<QStringList>(); }
bool CustomTableModel::insertRows(int, int, const QModelIndex &) { return false; }
bool CustomTableModel::insertColumns(int, int, const QModelIndex &) { return false; }
bool CustomTableModel::removeRows(int, int, const QModelIndex &) { return false; }
bool CustomTableModel::removeRow(int, const QModelIndex &) { return false; }
bool CustomTableModel::removeColumns(int, int, const QModelIndex &) { return false; }
bool CustomTableModel::canFetchMore() const { return false; }
bool CustomTableModel::canFetchMore(const QModelIndex &) const { return false; }
void CustomTableModel::fetchMore() {}
void CustomTableModel::fetchMore(const QModelIndex &) {}
bool CustomTableModel::submitColumns(QString, int) { return false; }
void CustomTableModel::sort(int, Qt::SortOrder) {}
int CustomTableModel::columnNamesToNumber(QString, int) { return -1; }
bool CustomTableModel::select(QStringList) { return false; }
void CustomTableModel::setFilter(QString) {}
void CustomTableModel::setTable(QString) {}
void CustomTableModel::updateRowInTable() {}
bool CustomTableModel::insertBinRecord(QString, int, QString) { return false; }
long CustomTableModel::translateBinnameToBinId(QString) { return -1; }
QString CustomTableModel::pathToSoundFile_ID(QString) { return QString(); }
QList<RelationTracking> CustomTableModel::getRelationtrackingList() { return QList<RelationTracking>(); }
QByteArray CustomTableModel::getCurrentMimeDataList() { return QByteArray(); }
QModelIndexList CustomTableModel::getSelectedIndexList() { return QModelIndexList(); }
QString CustomTableModel::makeRelativeDir(const QString) const { return QString(); }
bool CustomTableModel::updatePath(QHash<QString, QString>) { return false; }
QString CustomTableModel::selectStatement() const { return QString(); }
bool CustomTableModel::updateRowInTable(int, const QSqlRecord &) { return false; }
bool CustomTableModel::insertRowIntoTable(const QSqlRecord &) { return false; }
bool CustomTableModel::deleteRowFromTable(int) { return false; }
void CustomTableModel::setPrimaryKey(const QSqlIndex &) {}

//! ---- CustomSortFilterProxModel: Konstruktor (Qt6-Metatype-System braucht
//! eine aufrufbare Default-Konstruktor-Funktion, s. QMetaTypeForType<T>::
//! getDefaultCtr()), der Override von filterAcceptsRow (Vtable) + einziger
//! Slot + die restlichen deklarierten public-Methoden ----

CustomSortFilterProxModel::CustomSortFilterProxModel(QObject *parent) : QSortFilterProxyModel(parent) {}
bool CustomSortFilterProxModel::filterAcceptsRow(int, const QModelIndex &) const { return true; }
void CustomSortFilterProxModel::on_RowsAboutToBeInserted(const QModelIndex &, int, int) {}
void CustomSortFilterProxModel::setPreFilterColumn(const int) {}
void CustomSortFilterProxModel::setPreFilterValues(QStringList) {}
void CustomSortFilterProxModel::setCaseSensitivity(Qt::CaseSensitivity) {}
void CustomSortFilterProxModel::setFilterKeyColumns(const QList<qint32> &) {}
void CustomSortFilterProxModel::addFilterFixedString(qint32, const QString &, int, int, int) {}
void CustomSortFilterProxModel::addFilterFixedStringList(QList<QVariantList>) {}
void CustomSortFilterProxModel::setBinFilter(int) {}
//! moveUp()/moveDown() sind in customsortfilterproxmodel.h hinter
//! "#if defined QT_VERSION > 0x050000" versteckt -- das wertet als
//! (defined QT_VERSION) > 0x050000 aus, also 1 > 0x050000, IMMER false.
//! Praeprozessor-Bug in ProSoundFinder selbst (vermutlich sollte es ohne
//! "defined" heissen) -- die beiden Methoden sind dadurch nie deklariert,
//! in keinem Build. Hier daher bewusst NICHT definiert (wuerde sonst
//! "does not match any declaration" geben, s. Build-Fehler).

//! ---- MimeData: Konstruktor/Destruktor, die beiden Vtable-Overrides
//! (formats/retrieveData), die beiden private slots + setExporter ----

MimeData::MimeData() {}
MimeData::~MimeData() {}
void MimeData::setExporter(Export *) {}
QStringList MimeData::formats() const { return QStringList(); }
QVariant MimeData::retrieveData(const QString &, QMetaType) const { return QVariant(); }
void MimeData::sendDataRequest() {}
void MimeData::generateUrls() {}
