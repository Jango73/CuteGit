
// Qt
#include <QDebug>

// Application
#include "CTreeFileModel.h"
#include "CTreeFileModelProxy.h"
#include "CController.h"
#include "CGitCommands.h"
#include "CSvnCommands.h"

//-------------------------------------------------------------------------------------------------

CRepository::CRepository(const QString& sPath, CController* pController, QObject* parent)
    : QObject(parent)
    , m_eRepositoryType(CEnums::UnknownRepositoryType)
    , m_eRepositoryStatus(CEnums::NoMerge)
    , m_sRepositoryPath(sPath)
    , m_pController(pController)
    , m_pCommands(new CCommands())
    , m_pTreeFileModel(nullptr)
    , m_pTreeFileModelProxy(new CTreeFileModelProxy(pController, this))
    , m_pFlatFileModel(nullptr)
    , m_pFlatFileModelProxy(new CFlatFileModelProxy(pController, this))
    , m_pBranchModel(new CBranchModel(this))
    , m_pLogModel(new CLogModel(this))
    , m_pFileDiffModel(new CDiffModel(this))
    , m_pFileLogModel(new CLogModel(this))
    , m_pGraphModel(new CGraphModel(this))
    , m_bHasCommitableFiles(false)
{
    setRepositoryType(getRepositoryType(sPath));

    // Delete any existing command interface
    if (m_pCommands != nullptr)
        m_pCommands->deleteLater();

    // Create the command interface
    switch (m_eRepositoryType)
    {

    case CEnums::GIT:
        setCommands(new CGitCommands());
        break;

    case CEnums::SVN:
        setCommands(new CSvnCommands());
        break;

    default:
        setCommands(new CCommands());
        break;

    }

    // Command return values
    connect(m_pCommands, &CCommands::newOutputString, this, &CRepository::onNewOutputString);
    connect(m_pCommands, &CCommands::newOutputKeyValue, this, &CRepository::onNewOutputKeyValue);
    connect(m_pCommands, &CCommands::newOutputStringList, this, &CRepository::onNewOutputStringList);
    connect(m_pCommands, &CCommands::newOutputListOfCBranch, this, &CRepository::onNewOutputListOfCBranch);
    connect(m_pCommands, &CCommands::newOutputListOfCRepoFile, this, &CRepository::onNewOutputListOfCRepoFile);
    connect(m_pCommands, &CCommands::newOutputListOfCLogLine, this, &CRepository::onNewOutputListOfCLogLine);
    connect(m_pCommands, &CCommands::newOutputListOfCDiffLine, this, &CRepository::onNewOutputListOfCDiffLine);
    connect(m_pCommands, &CCommands::newOutputListOfCGraphLine, this, &CRepository::onNewOutputListOfCGraphLine);

    m_pFileDiffModel->setLines(QList<CDiffLine*>());
    m_pFileLogModel->setLines(QList<CLogLine*>());

    // Create a file model
    m_pTreeFileModel = new CTreeFileModel(this, this);
    m_pFlatFileModel = new CFlatFileModel(this, this);

    connect(m_pTreeFileModel, &CTreeFileModel::currentFileFullName, this, &CRepository::onCurrentFileFullName);
    connect(m_pFlatFileModel, &CFlatFileModel::currentFileFullName, this, &CRepository::onCurrentFileFullName);

    m_pFlatFileModelProxy->setSourceModel(m_pFlatFileModel);

    m_pTreeFileModelProxy->setSourceModel(m_pTreeFileModel);
    m_pTreeFileModel->setRootPath(sPath);

    refresh();
}

//-------------------------------------------------------------------------------------------------

CRepository::~CRepository()
{
    qDeleteAll(m_lRepoFiles);

    if (m_pCommands != nullptr)
        delete m_pCommands;
}

//-------------------------------------------------------------------------------------------------

void CRepository::setCurrentBranch(QString sValue)
{
    if (m_sCurrentBranch != sValue)
    {
        m_pCommands->setCurrentBranch(m_sRepositoryPath, sValue);
    }
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

QStringList CRepository::labelsForCommit(const QString& sCommitId) const
{
    return m_pBranchModel->labelsForCommit(sCommitId);
}

//-------------------------------------------------------------------------------------------------

bool CRepository::can(CEnums::ECapability eWhat)
{
    return m_pCommands->can(eWhat);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkAllFileStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->allFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkRepositoryStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->repositoryStatus(sPath);
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
        m_pCommands->toggleStaged(m_sRepositoryPath, sFullName);
        checkAllFileStatus();
    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::stageSelection(QStringList lFileFullNames)
{
    for (QString sFullName : lFileFullNames)
    {
        if (not sFullName.isEmpty())
            m_pCommands->stageFile(m_sRepositoryPath, sFullName, true);
    }
    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::unstageSelection(QStringList lFileFullNames)
{
    for (QString sFullName : lFileFullNames)
    {
        if (not sFullName.isEmpty())
            m_pCommands->stageFile(m_sRepositoryPath, sFullName, false);
    }
    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::stageAll()
{
    m_pCommands->stageAll(m_sRepositoryPath, true);
    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::unstageAll()
{
    m_pCommands->stageAll(m_sRepositoryPath, false);
    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::revertSelection(QStringList lFileFullNames)
{
    for (QString sFullName : lFileFullNames)
    {
        m_pCommands->revertFile(m_sRepositoryPath, sFullName);
    }
    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::commit(const QString& sMessage, bool bAmend)
{
    if (m_eRepositoryStatus == CEnums::NoMerge && bAmend == false)
    {
        m_pCommands->commit(m_sRepositoryPath, sMessage);
    }
    else
    {
        m_pCommands->amend(m_sRepositoryPath);
    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::continueRebase()
{
    m_pCommands->continueRebase(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::abortRebase()
{
    m_pCommands->abortRebase(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::push()
{
    m_pCommands->push(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::pull()
{
    m_pCommands->pull(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::fetch()
{
    m_pCommands->fetch(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitReset(const QString& sCommitId)
{
    m_pCommands->commitReset(m_sRepositoryPath, sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitRebase(const QString& sCommitId)
{
    m_pCommands->commitRebase(m_sRepositoryPath, sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitSquash(const QString& sCommitId)
{
    m_pCommands->commitSquash(m_sRepositoryPath, sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitBranchFrom(const QString& sCommitId, const QString& sBranchName)
{
    m_pCommands->commitBranchFrom(m_sRepositoryPath, sCommitId, sBranchName);
}

//-------------------------------------------------------------------------------------------------

void CRepository::changeCommitMessage(const QString& sCommitId, const QString& sMessage)
{
    m_pCommands->changeCommitMessage(m_sRepositoryPath, sCommitId, sMessage);
}

//-------------------------------------------------------------------------------------------------

void CRepository::deleteBranch(const QString& sName)
{
    m_pCommands->deleteBranch(m_sRepositoryPath, sName);
}

//-------------------------------------------------------------------------------------------------

CEnums::ERepositoryType CRepository::getRepositoryType(const QString& sPath)
{
    if (QDir(QString("%1/.git").arg(sPath)).exists())
    {
        return CEnums::GIT;
    }

    if (QDir(QString("%1/.svn").arg(sPath)).exists())
    {
        return CEnums::SVN;
    }

    return CEnums::UnknownRepositoryType;
}

//-------------------------------------------------------------------------------------------------

void CRepository::getBranches(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->branches(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::getBranchHeadCommits(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->branchHeadCommits(sPath, m_pBranchModel->branchNames());
}

//-------------------------------------------------------------------------------------------------

void CRepository::getGraph(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->graph(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::getLog(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pCommands->branchLog(sPath, dFrom, dTo);
}

//-------------------------------------------------------------------------------------------------

void CRepository::onCurrentFileFullName(QString sFileFullName)
{
    m_pCommands->unstagedFileDiff(m_sRepositoryPath, sFileFullName);
    m_pCommands->fileLog(m_sRepositoryPath, sFileFullName);
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

        while (lData.count() > 50)
            lData.removeFirst();

        m_pController->commandOutputModel()->setStringList(lData);
    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputString(CEnums::EProcessCommand eCommand, QString sOutput)
{
    switch (eCommand)
    {

    case CEnums::eRepositoryStatus:
    {
        if (sOutput == CRepoFile::sRepositoryStatusClean)
            setRepositoryStatus(CEnums::NoMerge);
        else if (sOutput == CRepoFile::sRepositoryStatusMerge)
            setRepositoryStatus(CEnums::Merge);
        else if (sOutput == CRepoFile::sRepositoryStatusRebase)
            setRepositoryStatus(CEnums::Rebase);
        else if (sOutput == CRepoFile::sRepositoryStatusInteractiveRebase)
            setRepositoryStatus(CEnums::InteractiveRebase);
        break;
    }

    case CEnums::eNotification:
    case CEnums::eStageFile:
    case CEnums::eStageAll:
    case CEnums::eRevertFile:
    {
        onNewOutput(sOutput);
        break;
    }

    case CEnums::eCommit:
    case CEnums::eAmend:
    case CEnums::ePush:
    case CEnums::ePull:
    {
        onNewOutput(sOutput);
        getBranchHeadCommits();
        checkAllFileStatus();
        getLog();
        break;
    }

    case CEnums::eSetCurrentBranch:
    case CEnums::eResetToCommit:
    case CEnums::eRebaseOnCommit:
    case CEnums::eSquashCommit:
    case CEnums::eBranchFromCommit:
    case CEnums::eDeleteBranch:
    case CEnums::eChangeCommitMessage:
    case CEnums::eContinueRebase:
    case CEnums::eAbortRebase:
    {
        onNewOutput(sOutput);
        refresh();
        break;
    }

    case CEnums::eCurrentBranch:
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

void CRepository::onNewOutputKeyValue(CEnums::EProcessCommand eCommand, QString sKey, QString sValue)
{
    switch (eCommand)
    {

    case CEnums::eBranchHeadCommit:
    {
        m_pBranchModel->setBranchHeadCommit(sKey, sValue);
        m_pLogModel->commitChanged(sValue);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputStringList(CEnums::EProcessCommand eCommand, QStringList lValue)
{
    Q_UNUSED(eCommand);
    Q_UNUSED(lValue);
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCBranch(CEnums::EProcessCommand eCommand, QList<CBranch*> lNewBranches)
{
    switch (eCommand)
    {

    case CEnums::eBranches:
    {
        m_pBranchModel->setBranchList(lNewBranches);
        getBranchHeadCommits();
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles)
{
    switch (eCommand)
    {

    case CEnums::eAllFileStatus:
    {
        // TODO
//        QStringList changedFiles;

//        for (CRepoFile* pExistingFile : m_lRepoFiles)
//        {
//            CRepoFile* pNewFile = fileByFullName(lNewRepoFiles, pExistingFile->fullName());

//            if (pNewFile == nullptr)
//            {
//                changedFiles << pNewFile->fullName();
//            }
//        }

//        for (CRepoFile* pFile : lNewRepoFiles)
//        {
//            CRepoFile* pExistingFile = fileByFullName(m_lRepoFiles, pFile->fullName());

//            if (pExistingFile != nullptr)
//            {
//                if (pExistingFile->status() != pFile->status() || pExistingFile->staged() != pFile->staged())
//                {
//                    changedFiles << pExistingFile->fullName();
//                }
//            }
//        }

        qDeleteAll(m_lRepoFiles);
        m_lRepoFiles.clear();

        bool bHasCommitableFiles = false;

        for (CRepoFile* pFile : lNewRepoFiles)
        {
            if (pFile->status() != CEnums::eIgnored)
            {
                if (pFile->staged())
                    bHasCommitableFiles = true;

                m_lRepoFiles << pFile;
            }
        }

        setHasCommitableFiles(bHasCommitableFiles);

        m_pTreeFileModel->handleRepoFilesChanged();
        m_pFlatFileModel->handleRepoFilesChanged();
        m_pTreeFileModelProxy->filterChanged();
        m_pFlatFileModelProxy->filterChanged();

        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCLogLine(CEnums::EProcessCommand eCommand, QList<CLogLine*> lNewLines)
{
    switch (eCommand)
    {

    case CEnums::eBranchLog:
    {
        m_pLogModel->setLines(lNewLines);
        break;
    }

    case CEnums::eFileLog:
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

void CRepository::onNewOutputListOfCDiffLine(CEnums::EProcessCommand eCommand, QList<CDiffLine*> lNewLines)
{
    switch (eCommand)
    {

    case CEnums::eUnstagedFileDiff:
    {
        m_pFileDiffModel->setLines(lNewLines);
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCGraphLine(CEnums::EProcessCommand eCommand, QList<CGraphLine*> lNewLines)
{
    switch (eCommand)
    {

    case CEnums::eGraph:
    {
        m_pGraphModel->setLines(lNewLines);
        break;
    }

    default:
    {
        break;
    }

    }
}
