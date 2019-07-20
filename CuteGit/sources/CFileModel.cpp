
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
    , m_pLogModel(new CLogModel(this))
    , m_pDiffModel(new QStringListModel(this))
    , m_pFileLogModel(new QStringListModel(this))
{
    setRootPath(QDir::homePath());
    setResolveSymlinks(true);

    connect(this, &QFileSystemModel::rootPathChanged, this, &CFileModel::onRootPathChanged);

    // Command return values
    connect(m_pController->commands(), &CCommands::newOutputString, this, &CFileModel::onNewOutputString);
    connect(m_pController->commands(), &CCommands::newOutputStringList, this, &CFileModel::onNewOutputStringList);
    connect(m_pController->commands(), &CCommands::newOutputListOfCRepoFile, this, &CFileModel::onNewOutputListOfCRepoFile);
    connect(m_pController->commands(), &CCommands::newOutputListOfCLogLine, this, &CFileModel::onNewOutputListOfCLogLine);
}

//-------------------------------------------------------------------------------------------------

CFileModel::~CFileModel()
{
    qDeleteAll(m_vRepoFiles);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::setCurrentBranch(QString sValue)
{
    if (m_sCurrentBranch != sValue)
    {
        m_pController->commands()->setCurrentBranch(m_pController->repositoryPath(), sValue);
    }
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

    m_pController->commands()->allFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::checkRepositoryStatus(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->repositoryStatus(sPath);
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
    checkRepositoryStatus();
    checkAllFileStatus();
    getBranches();
    getLog();
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

void CFileModel::commit(const QString& sMessage, bool bAmend)
{
    if (m_eRepositoryStatus == NoMerge && bAmend == false)
    {
        m_pController->commands()->commit(m_pController->repositoryPath(), sMessage);
    }
    else
    {
        m_pController->commands()->amend(m_pController->repositoryPath());
    }

    checkAllFileStatus();
    getLog();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::push()
{
    m_pController->commands()->push(m_pController->repositoryPath());

    getLog();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::pull()
{
    m_pController->commands()->pull(m_pController->repositoryPath());

    checkAllFileStatus();
    getLog();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::getBranches(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    m_pController->commands()->branches(sPath);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::getLog(QString sPath)
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

void CFileModel::onRootPathChanged(const QString& sNewPath)
{
    Q_UNUSED(sNewPath);

    refresh();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onNewOutputString(CProcessCommand::EProcessCommand eCommand, QString sOutput)
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
        break;
    }

    case CProcessCommand::eSetCurrentBranch:
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

void CFileModel::onNewOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue)
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

    case CProcessCommand::eFileLog:
    {
        m_pFileLogModel->setStringList(lValue);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onNewOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles)
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

void CFileModel::onNewOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> lNewGraphLines)
{
    switch (eCommand)
    {

    case CProcessCommand::eBranchLog:
    {
        m_pLogModel->setGraphLines(lNewGraphLines);
        break;
    }

    default:
    {
        break;
    }

    }
}
