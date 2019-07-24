
// Qt
#include <QDebug>
#include <QUrl>

// Application
#include "CTreeFileModel.h"
#include "CTreeFileModelProxy.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CRepository::CRepository(CController* pController, QObject* parent)
    : QObject(parent)
    , m_pController(pController)
    , m_pTreeFileModel(nullptr)
    , m_pTreeFileModelProxy(new CTreeFileModelProxy(pController, this))
    , m_pFlatFileModel(nullptr)
    , m_pBranchModel(new QStringListModel(this))
    , m_pLogModel(new CLogModel(this))
    , m_pDiffModel(new CDiffModel(this))
    , m_pFileLogModel(new CLogModel(this))
{
    // Command return values
    connect(m_pController->commands(), &CCommands::newOutputString, this, &CRepository::onNewOutputString);
    connect(m_pController->commands(), &CCommands::newOutputStringList, this, &CRepository::onNewOutputStringList);
    connect(m_pController->commands(), &CCommands::newOutputListOfCRepoFile, this, &CRepository::onNewOutputListOfCRepoFile);
    connect(m_pController->commands(), &CCommands::newOutputListOfCLogLine, this, &CRepository::onNewOutputListOfCLogLine);
    connect(m_pController->commands(), &CCommands::newOutputListOfCDiffLine, this, &CRepository::onNewOutputListOfCDiffLine);
}

//-------------------------------------------------------------------------------------------------

CRepository::~CRepository()
{
    qDeleteAll(m_lRepoFiles);
}

//-------------------------------------------------------------------------------------------------

void CRepository::setRepositoryPath(QString sPath)
{
    if (sPath.startsWith("file:"))
        sPath = QUrl(sPath).toLocalFile();

    // If repo path valid and different from current
    if (m_pTreeFileModel == nullptr || m_sRepositoryPath != sPath)
    {
        // IF repo is a GIT repo
        if (QDir(QString("%1/.git").arg(sPath)).exists())
        {
            m_sRepositoryPath = sPath;

            // Delete any existing file model
            if (m_pTreeFileModel != nullptr)
                m_pTreeFileModel->deleteLater();

            if (m_pFlatFileModel != nullptr)
                m_pFlatFileModel->deleteLater();

            // Create a file model
            m_pTreeFileModel = new CTreeFileModel(m_pController, this);
            m_pFlatFileModel = new CFlatFileModel(m_pController, this);

            m_pTreeFileModelProxy->setSourceModel(m_pTreeFileModel);
            m_pTreeFileModel->setRootPath(sPath);

            emit repositoryPathChanged();
            emit treeFileModelChanged();
            emit treeFileModelProxyChanged();
            emit flatFileModelChanged();

            connect(m_pTreeFileModel, &CTreeFileModel::currentFileFullName, this, &CRepository::onCurrentFileFullName);
            connect(m_pFlatFileModel, &CFlatFileModel::currentFileFullName, this, &CRepository::onCurrentFileFullName);

            // Add this path to repository model
            QStringList lRepositoryPaths = m_pController->repositoryModel()->stringList();

            if (lRepositoryPaths.contains(sPath) == false)
                lRepositoryPaths << sPath;

            m_pController->repositoryModel()->setStringList(lRepositoryPaths);

            // Clear command output
            m_pController->commandOutputModel()->setStringList(QStringList());

            refresh();
        }
        else
        {
            onNewOutput(QString(tr("%1 is not a GIT repository.\nPlease select a folder containing a GIT repository.")).arg(sPath));
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::setCurrentBranch(QString sValue)
{
    if (m_sCurrentBranch != sValue)
    {
        m_pController->commands()->setCurrentBranch(m_sRepositoryPath, sValue);
    }
}

//-------------------------------------------------------------------------------------------------

QString CRepository::repositoryPath() const
{
    return m_sRepositoryPath;
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CRepository::fileByFullName(const QList<CRepoFile*>& vFiles, const QString& sFullName) const
{
    for (CRepoFile* pFile : vFiles)
    {
        if (pFile->fullName() == sFullName)
            return pFile;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkAllFileStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pController->commands()->allFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkRepositoryStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pController->commands()->repositoryStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::refresh()
{
    checkRepositoryStatus();
    checkAllFileStatus();
    getBranches();
    getLog();
}

//-------------------------------------------------------------------------------------------------

void CRepository::toggleStaged(QString sFullName)
{
    if (not sFullName.isEmpty())
    {
        m_pController->commands()->toggleStaged(m_sRepositoryPath, sFullName);

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::stageSelection()
{
//    for (QModelIndex qIndex : lIndices)
//    {
//        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
//        m_pController->commands()->stageFile(m_sRepositoryPath, sFileFullName, true);
//    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::unstageSelection()
{
//    for (QModelIndex qIndex : lIndices)
//    {
//        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
//        m_pController->commands()->stageFile(m_sRepositoryPath, sFileFullName, false);
//    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::stageAll()
{
    m_pController->commands()->stageAll(m_sRepositoryPath, true);
}

//-------------------------------------------------------------------------------------------------

void CRepository::unstageAll()
{
    m_pController->commands()->stageAll(m_sRepositoryPath, false);
}

//-------------------------------------------------------------------------------------------------

void CRepository::revertSelection()
{
//    for (QModelIndex qIndex : lIndices)
//    {
//        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
//        m_pController->commands()->revertFile(m_sPath, sFileFullName);
//    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::commit(const QString& sMessage, bool bAmend)
{
    if (m_eRepositoryStatus == NoMerge && bAmend == false)
    {
        m_pController->commands()->commit(m_sRepositoryPath, sMessage);
    }
    else
    {
        m_pController->commands()->amend(m_sRepositoryPath);
    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::continueRebase()
{
    m_pController->commands()->continueRebase(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::abortRebase()
{
    m_pController->commands()->abortRebase(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::push()
{
    m_pController->commands()->push(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::pull()
{
    m_pController->commands()->pull(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitReset(const QString& sCommitId)
{
    m_pController->commands()->commitReset(m_sRepositoryPath, sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitRebase(const QString& sCommitId)
{
    m_pController->commands()->commitRebase(m_sRepositoryPath, sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CRepository::changeCommitMessage(const QString& sCommitId, const QString& sMessage)
{
    m_pController->commands()->changeCommitMessage(m_sRepositoryPath, sCommitId, sMessage);
}

//-------------------------------------------------------------------------------------------------

void CRepository::getBranches(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pController->commands()->branches(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::getLog(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pController->commands()->branchLog(sPath, dFrom, dTo);
}

//-------------------------------------------------------------------------------------------------

void CRepository::onCurrentFileFullName(QString sFileFullName)
{
    m_pController->commands()->unstagedFileDiff(m_sRepositoryPath, sFileFullName);
    m_pController->commands()->fileLog(m_sRepositoryPath, sFileFullName);
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutput(QString sOutput)
{
    QStringList lNewList = sOutput.split("\n");
    QStringList lData = m_pController->commandOutputModel()->stringList();
    bool bHasNewLine = false;

    for (QString sLine : lNewList)
    {
        sLine = sLine.trimmed();

        if (sLine.isEmpty() == false)
        {
            bHasNewLine = true;
            lData << sLine;
        }
    }

    if (bHasNewLine)
    {
        lData << "----------------------------------------------------------------------------------------------------";
    }

    while (lData.count() > 50)
        lData.removeFirst();

    m_pController->commandOutputModel()->setStringList(lData);
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputString(CProcessCommand::EProcessCommand eCommand, QString sOutput)
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
        onNewOutput(sOutput);
        checkAllFileStatus();
        getLog();
        break;
    }

    case CProcessCommand::eSetCurrentBranch:
    case CProcessCommand::eCommitReset:
    case CProcessCommand::eCommitRebase:
    case CProcessCommand::eChangeCommitMessage:
    case CProcessCommand::eContinueRebase:
    case CProcessCommand::eAbortRebase:
    {
        onNewOutput(sOutput);
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

void CRepository::onNewOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue)
{
    switch (eCommand)
    {

    case CProcessCommand::eBranches:
    {
        m_pBranchModel->setStringList(lValue);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles)
{
    switch (eCommand)
    {

    case CProcessCommand::eAllFileStatus:
    {
        qDeleteAll(m_lRepoFiles);
        m_lRepoFiles.clear();

        for (CRepoFile* pFile : lNewRepoFiles)
        {
            if (pFile->status() != CRepoFile::eIgnored)
                m_lRepoFiles << pFile;
        }

//        m_pTreeFileModel->handleRepoFilesChanged();
        m_pFlatFileModel->handleRepoFilesChanged();
        m_pTreeFileModelProxy->filterChanged();

        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> lNewLines)
{
    switch (eCommand)
    {

    case CProcessCommand::eBranchLog:
    {
        m_pLogModel->setLines(lNewLines);
        break;
    }

    case CProcessCommand::eFileLog:
    {
        m_pFileLogModel->setLines(lNewLines);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCDiffLine(CProcessCommand::EProcessCommand eCommand, QList<CDiffLine*> lNewLines)
{
    switch (eCommand)
    {

    case CProcessCommand::eUnstagedFileDiff:
    {
        m_pDiffModel->setLines(lNewLines);
        break;
    }

    default:
    {
        break;
    }

    }
}
