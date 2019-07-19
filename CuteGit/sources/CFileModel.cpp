
// Application
#include "CFileModel.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

static inline QString sizeString(const QFileInfo &fi)
{
    if (!fi.isFile())
        return QString();
    const qint64 size = fi.size();
    if (size > 1024 * 1024 * 10)
        return QString::number(size / (1024 * 1024)) + QLatin1Char('M');
    if (size > 1024 * 10)
        return QString::number(size / 1024) + QLatin1Char('K');
    return QString::number(size);
}

//-------------------------------------------------------------------------------------------------

CFileModel::CFileModel(CController* pController, QObject* parent)
    : QFileSystemModel(parent)
    , m_pController(pController)
    , m_pBranchModel(new QStringListModel(this))
    , m_pGraphModel(new QStringListModel(this))
    , m_pDiffModel(new QStringListModel(this))
    , m_pLogModel(new QStringListModel(this))
{
    setRootPath(QDir::homePath());
    setResolveSymlinks(true);

    connect(this, &QFileSystemModel::rootPathChanged, this, &CFileModel::onRootPathChanged);

    connect(m_pController->commands(), &CCommands::execFinished_QString, this, &CFileModel::onCommandFinished_QString);
    connect(m_pController->commands(), &CCommands::execFinished_QStringList, this, &CFileModel::onCommandFinished_QStringList);
    connect(m_pController->commands(), &CCommands::execFinished_QList_CRepoFile, this, &CFileModel::onCommandFinished_QList_CRepoFile);
}

//-------------------------------------------------------------------------------------------------

CFileModel::~CFileModel()
{
    qDeleteAll(m_vRepoFiles);
}

//-------------------------------------------------------------------------------------------------

QModelIndex CFileModel::rootPathIndex() const
{
    return index(rootPath());
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CFileModel::fileByFullName(const QList<CRepoFile*>& vFiles, const QString& sFullName) const
{
    for (CRepoFile* pFile : vFiles)
    {
        if (pFile->fullName() == sFullName)
            return pFile;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames = QFileSystemModel::roleNames();
    hRoleNames[eSizeRole] = "size";
    hRoleNames[eStatusRole] = "status";
    hRoleNames[eStagedRole] = "staged";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

QVariant CFileModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && role >= eSizeRole)
    {
        switch (role)
        {

        case eSizeRole:
        {
            return QVariant(sizeString(fileInfo(index)));
        }

        case eStatusRole:
        {
            QString sFileFullName = fileInfo(index).absoluteFilePath();
            CRepoFile* pFile = fileByFullName(m_vRepoFiles, sFileFullName);

            if (pFile != nullptr)
                return pFile->statusToString();

            return CRepoFile::sTokenClean;
        }

        case eStagedRole:
        {
            QString sFileFullName = fileInfo(index).absoluteFilePath();
            CRepoFile* pFile = fileByFullName(m_vRepoFiles, sFileFullName);

            if (pFile != nullptr)
                return pFile->stagedToString();

            return CRepoFile::sTokenUnstaged;
        }

        default:
            break;
        }

    }

    return QFileSystemModel::data(index, role);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::checkAllFileStatus(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->getAllFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::handleCurrentIndex(QModelIndex qIndex)
{
    QString sFileFullName = fileInfo(qIndex).absoluteFilePath();

    m_pController->commands()->unstagedFileDiff(m_pController->repositoryPath(), sFileFullName);
    m_pController->commands()->fileLog(m_pController->repositoryPath(), sFileFullName);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::refresh()
{
    checkAllFileStatus();
    getBranches();
    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::stageSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        m_pController->commands()->stageFile(m_pController->repositoryPath(), sFileFullName, true);
    }

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::unstageSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        m_pController->commands()->stageFile(m_pController->repositoryPath(), sFileFullName, false);
    }

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::stageAll()
{
    m_pController->commands()->stageAll(m_pController->repositoryPath(), true);

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::revertSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        m_pController->commands()->revertFile(m_pController->repositoryPath(), sFileFullName);
    }

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::commit(const QString& sMessage)
{
    m_pController->commands()->commit(m_pController->repositoryPath(), sMessage);

    checkAllFileStatus();
    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::push()
{
    m_pController->commands()->push(m_pController->repositoryPath());

    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::pull()
{
    m_pController->commands()->pull(m_pController->repositoryPath());

    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::getBranches(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->getBranches(sPath);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::getGraph(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pController->commands()->getGraph(sPath, dFrom, dTo);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onRootPathChanged(const QString& sNewPath)
{
    Q_UNUSED(sNewPath);

    checkAllFileStatus();
    getBranches();
    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onCommandFinished_QString(CProcessCommand::EProcessCommand eCommand, QString sOutput)
{
    switch (eCommand)
    {

    case CProcessCommand::eStageFile:
    case CProcessCommand::eStageAll:
    case CProcessCommand::eRevertFile:
    case CProcessCommand::eCommit:
    case CProcessCommand::ePush:
    case CProcessCommand::ePull:
    {
        emit newOutput(sOutput);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onCommandFinished_QStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue)
{
    switch (eCommand)
    {

    case CProcessCommand::eGetBranches:
    {
        m_pBranchModel->setStringList(lValue);
        break;
    }

    case CProcessCommand::eGetGraph:
    {
        m_pGraphModel->setStringList(lValue);
        break;
    }

    case CProcessCommand::eUnstagedFileDiff:
    {
        m_pDiffModel->setStringList(lValue);
        break;
    }

    case CProcessCommand::eFileLog:
    {
        m_pLogModel->setStringList(lValue);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onCommandFinished_QList_CRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> vNewRepoFiles)
{
    switch (eCommand)
    {

    case CProcessCommand::eGetAllFileStatus:
    {
        QVector<QModelIndex> changedIndices;

        for (CRepoFile* pExistingFile : m_vRepoFiles)
        {
            CRepoFile* pNewFile = fileByFullName(vNewRepoFiles, pExistingFile->fullName());

            if (pNewFile == nullptr)
            {
                QModelIndex qIndex = index(pExistingFile->fullName());
                changedIndices << qIndex;
            }
        }

        for (CRepoFile* pFile : vNewRepoFiles)
        {
            CRepoFile* pExistingFile = fileByFullName(m_vRepoFiles, pFile->fullName());

            if (pExistingFile != nullptr)
            {
                if (pExistingFile->status() != pFile->status() || pExistingFile->staged() != pFile->staged())
                {
                    QModelIndex qIndex = index(pExistingFile->fullName());
                    changedIndices << qIndex;
                }
            }
        }

        qDeleteAll(m_vRepoFiles);
        m_vRepoFiles.clear();

        for (CRepoFile* pFile : vNewRepoFiles)
        {
            m_vRepoFiles << pFile;
        }

        for (QModelIndex qIndex : changedIndices)
        {
            emit dataChanged(qIndex, qIndex);
        }

        break;
    }

    default:
    {
        break;
    }

    }
}
