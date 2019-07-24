
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
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::setRepoFiles(QList<CRepoFile*> lNewFiles)
{
    beginResetModel();

    m_vRepoFiles.clear();

    for (CRepoFile* pFile : lNewFiles)
    {
        if (pFile->status() != CRepoFile::eIgnored)
            m_vRepoFiles << pFile;
    }

    endResetModel();
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CFlatFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eFullNameRole] = "fullName";
    hRoleNames[eFileNameRole] = "fileName";
    hRoleNames[eRelativeNameRole] = "relativeName";
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

QVariant CFlatFileModel::data(const QModelIndex& qIndex, int iRole) const
{
    if (qIndex.isValid())
    {
        switch (iRole)
        {
        case eFullNameRole:
            return m_vRepoFiles[qIndex.row()]->fullName();

        case eFileNameRole:
            return m_vRepoFiles[qIndex.row()]->fileName();

        case eRelativeNameRole:
            return m_vRepoFiles[qIndex.row()]->relativeName();

        case eSizeRole:
            return 0;

        case eStatusRole:
            return m_vRepoFiles[qIndex.row()]->statusToString();

        case eStagedRole:
            return m_vRepoFiles[qIndex.row()]->stagedToString();
        }
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

bool CFlatFileModel::isEmpty() const
{
    return m_vRepoFiles.count() == 0;
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::handleCurrentIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        emit currentFileFullName(m_vRepoFiles[qIndex.row()]->fullName());
    }
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::toggleStaged(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        QString sFileFullName = m_vRepoFiles[qIndex.row()]->fullName();
        // m_pController->commands()->toggleStaged(m_pController->repositoryPath(), sFileFullName);
    }
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
