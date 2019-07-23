
// Qt
#include <QDebug>

// Application
#include "CTreeFileModel.h"
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

CTreeFileModel::CTreeFileModel(CController* pController, QObject* parent)
    : QFileSystemModel(parent)
    , m_pController(pController)
    , m_pBranchModel(new QStringListModel(this))
    , m_pLogModel(new CLogModel(this))
    , m_pDiffModel(new QStringListModel(this))
    , m_pFileLogModel(new CLogModel(this))
    , m_pFileSystemWatcher(new QFileSystemWatcher(this))
{
    setRootPath(QDir::homePath());
    setResolveSymlinks(true);

    connect(this, &QFileSystemModel::rootPathChanged, this, &CTreeFileModel::onRootPathChanged);

    // Command return values
    connect(m_pController->commands(), &CCommands::newOutputString, this, &CTreeFileModel::onNewOutputString);
    connect(m_pController->commands(), &CCommands::newOutputStringList, this, &CTreeFileModel::onNewOutputStringList);
    connect(m_pController->commands(), &CCommands::newOutputListOfCRepoFile, this, &CTreeFileModel::onNewOutputListOfCRepoFile);
    connect(m_pController->commands(), &CCommands::newOutputListOfCLogLine, this, &CTreeFileModel::onNewOutputListOfCLogLine);

    connect(m_pFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &CTreeFileModel::onFileChanged);
    connect(m_pFileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &CTreeFileModel::onFileChanged);
}

//-------------------------------------------------------------------------------------------------

CTreeFileModel::~CTreeFileModel()
{
    qDeleteAll(m_vRepoFiles);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::setCurrentBranch(QString sValue)
{
    if (m_sCurrentBranch != sValue)
    {
        m_pController->commands()->setCurrentBranch(m_pController->repositoryPath(), sValue);
    }
}

//-------------------------------------------------------------------------------------------------

QModelIndex CTreeFileModel::rootPathIndex() const
{
    return index(rootPath());
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CTreeFileModel::fileByFullName(const QList<CRepoFile*>& vFiles, const QString& sFullName) const
{
    for (CRepoFile* pFile : vFiles)
    {
        if (pFile->fullName() == sFullName)
            return pFile;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CTreeFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames = QFileSystemModel::roleNames();
    hRoleNames[eSizeRole] = "size";
    hRoleNames[eStatusRole] = "status";
    hRoleNames[eStagedRole] = "staged";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

QVariant CTreeFileModel::data(const QModelIndex& index, int role) const
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

void CTreeFileModel::checkAllFileStatus(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->allFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::checkRepositoryStatus(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->repositoryStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::handleCurrentIndex(QModelIndex qIndex)
{
    QString sFileFullName = fileInfo(qIndex).absoluteFilePath();

    emit currentFileFullName(sFileFullName);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::refresh()
{
    checkRepositoryStatus();
    checkAllFileStatus();
    getBranches();
    getLog();
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::stageSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        m_pController->commands()->stageFile(m_pController->repositoryPath(), sFileFullName, true);
    }
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::unstageSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        m_pController->commands()->stageFile(m_pController->repositoryPath(), sFileFullName, false);
    }
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::stageAll()
{
    m_pController->commands()->stageAll(m_pController->repositoryPath(), true);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::unstageAll()
{
    m_pController->commands()->stageAll(m_pController->repositoryPath(), false);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::revertSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        m_pController->commands()->revertFile(m_pController->repositoryPath(), sFileFullName);
    }
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::commit(const QString& sMessage, bool bAmend)
{
    if (m_eRepositoryStatus == NoMerge && bAmend == false)
    {
        m_pController->commands()->commit(m_pController->repositoryPath(), sMessage);
    }
    else
    {
        m_pController->commands()->amend(m_pController->repositoryPath());
    }
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::continueRebase()
{
    m_pController->commands()->continueRebase(m_pController->repositoryPath());
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::abortRebase()
{
    m_pController->commands()->abortRebase(m_pController->repositoryPath());
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::push()
{
    m_pController->commands()->push(m_pController->repositoryPath());
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::pull()
{
    m_pController->commands()->pull(m_pController->repositoryPath());
}


//-------------------------------------------------------------------------------------------------

void CTreeFileModel::commitRebase(const QString& sCommitId)
{
    m_pController->commands()->commitRebase(m_pController->repositoryPath(), sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::changeCommitMessage(const QString& sCommitId, const QString& sMessage)
{
    m_pController->commands()->changeCommitMessage(m_pController->repositoryPath(), sCommitId, sMessage);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::getBranches(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->branches(sPath);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::getLog(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pController->commands()->branchLog(sPath, dFrom, dTo);
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::onRootPathChanged(const QString& sNewPath)
{
    Q_UNUSED(sNewPath);

    m_pFileSystemWatcher->addPath(sNewPath);

    refresh();
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::onFileChanged(const QString& path)
{
    Q_UNUSED(path);

    refresh();
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::onNewOutputString(CProcessCommand::EProcessCommand eCommand, QString sOutput)
{
    switch (eCommand)
    {

    case CProcessCommand::eRepositoryStatus:
    {
        if (sOutput == CRepoFile::sRepositoryStatusClean)
            setRepositoryStatus(NoMerge);
        else if (sOutput == CRepoFile::sRepositoryStatusMerge)
            setRepositoryStatus(Merge);
        else if (sOutput == CRepoFile::sRepositoryStatusRebase)
            setRepositoryStatus(Rebase);
        else if (sOutput == CRepoFile::sRepositoryStatusInteractiveRebase)
            setRepositoryStatus(InteractiveRebase);
        break;
    }

    case CProcessCommand::eNotification:
    case CProcessCommand::eStageFile:
    case CProcessCommand::eStageAll:
    case CProcessCommand::eRevertFile:
    case CProcessCommand::eCommit:
    case CProcessCommand::eAmend:
    case CProcessCommand::ePush:
    case CProcessCommand::ePull:
    {
        emit newOutput(sOutput);
        checkAllFileStatus();
        getLog();
        break;
    }

    case CProcessCommand::eSetCurrentBranch:
    case CProcessCommand::eCommitRebase:
    case CProcessCommand::eChangeCommitMessage:
    case CProcessCommand::eContinueRebase:
    case CProcessCommand::eAbortRebase:
    {
        emit newOutput(sOutput);
        refresh();
        break;
    }

    case CProcessCommand::eCurrentBranch:
    {
        m_sCurrentBranch = sOutput;

        emit currentBranchChanged();
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::onNewOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue)
{
    switch (eCommand)
    {

    case CProcessCommand::eBranches:
    {
        m_pBranchModel->setStringList(lValue);
        break;
    }

    case CProcessCommand::eUnstagedFileDiff:
    {
        m_pDiffModel->setStringList(lValue);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::onNewOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles)
{
    switch (eCommand)
    {

    case CProcessCommand::eAllFileStatus:
    {
        QVector<QModelIndex> changedIndices;

        for (CRepoFile* pExistingFile : m_vRepoFiles)
        {
            CRepoFile* pNewFile = fileByFullName(lNewRepoFiles, pExistingFile->fullName());

            if (pNewFile == nullptr)
            {
                QModelIndex qIndex = index(pExistingFile->fullName());
                changedIndices << qIndex;
            }
        }

        for (CRepoFile* pFile : lNewRepoFiles)
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

        for (CRepoFile* pFile : lNewRepoFiles)
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

//-------------------------------------------------------------------------------------------------

void CTreeFileModel::onNewOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> lNewGraphLines)
{
    switch (eCommand)
    {

    case CProcessCommand::eBranchLog:
    {
        m_pLogModel->setGraphLines(lNewGraphLines);
        break;
    }

    case CProcessCommand::eFileLog:
    {
        m_pFileLogModel->setGraphLines(lNewGraphLines);
        break;
    }

    default:
    {
        break;
    }

    }
}
