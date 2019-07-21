
// Application
#include "CFlatFileModel.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CFlatFileModel::CFlatFileModel(CController* pController, QObject* parent)
    : QAbstractListModel(parent)
    , m_pController(pController)
{
    // Command return values
    connect(m_pController->commands(), &CCommands::newOutputString, this, &CFlatFileModel::onNewOutputString);
    connect(m_pController->commands(), &CCommands::newOutputStringList, this, &CFlatFileModel::onNewOutputStringList);
    connect(m_pController->commands(), &CCommands::newOutputListOfCRepoFile, this, &CFlatFileModel::onNewOutputListOfCRepoFile);
    connect(m_pController->commands(), &CCommands::newOutputListOfCLogLine, this, &CFlatFileModel::onNewOutputListOfCLogLine);
}

//-------------------------------------------------------------------------------------------------

CFlatFileModel::~CFlatFileModel()
{
    qDeleteAll(m_vRepoFiles);
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::setRepoFiles(QList<CRepoFile*> lNewFiles)
{
    beginResetModel();

    qDeleteAll(m_vRepoFiles);
    m_vRepoFiles.clear();

    for (CRepoFile* pLine : lNewFiles)
        m_vRepoFiles << pLine;

    endResetModel();
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CFlatFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eFileNameRole] = "fileName";
    hRoleNames[eSizeRole] = "size";
    hRoleNames[eStatusRole] = "status";
    hRoleNames[eStagedRole] = "staged";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CFlatFileModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_vRepoFiles.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CFlatFileModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    if ((row < 0) || (row > (rowCount() - 1)))
        return QVariant();

    switch (role)
    {
    case eFileNameRole:
        return m_vRepoFiles[row]->fileName();

    case eSizeRole:
        return 0;

    case eStatusRole:
        return m_vRepoFiles[row]->statusToString();

    case eStagedRole:
        return m_vRepoFiles[row]->stagedToString();
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

bool CFlatFileModel::isEmpty() const
{
    return m_vRepoFiles.count() == 0;
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::onNewOutputString(CProcessCommand::EProcessCommand eCommand, QString sOutput)
{
    Q_UNUSED(eCommand);
    Q_UNUSED(sOutput);
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::onNewOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue)
{
    Q_UNUSED(eCommand);
    Q_UNUSED(lValue);
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::onNewOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles)
{
    switch (eCommand)
    {

    case CProcessCommand::eAllFileStatus:
    {
        setRepoFiles(lNewRepoFiles);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::onNewOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> lNewGraphLines)
{
    Q_UNUSED(eCommand);
    Q_UNUSED(lNewGraphLines);
}
