
// Qt
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>

// Application
#include "CUtils.h"
#include "CController.h"
#include "CGitCommands.h"
#include "CGerritCommands.h"
#include "CSvnCommands.h"
#include "CHgCommands.h"

//-------------------------------------------------------------------------------------------------

/*!
    \class CRepository
    \inmodule CuteGit
    \section1 How this class works
    Whenever the user triggers a repository command from the UI:
    \list
    \li A method from this class is called.
    \li The method in turn calls one or more methods in CCommands.
    \li CCommands executes some process and emits a result.
    \li The result is caught in this class' slots, like onNewOutputString.
    \li The data is put into models which in turn notify the UI.

        ___
     __/_  `.  .-"""-.
     \_,` | \-'  /   )`-')
      "") `"`    \  ((`"`
     ___Y  ,    .'7 /|
    (_,___/...-` (_/_/ sk
*/

//-------------------------------------------------------------------------------------------------

CRepository::CRepository(const QString& sPath, CController* pController, QObject* parent, CEnums::ERepositoryType eType)
    : QObject(parent)
    , m_eRepositoryType(CEnums::UnknownRepositoryType)
    , m_eRepositoryStatus(CEnums::NoMerge)
    , m_sRepositoryPath(sPath)
    , m_pController(pController)
    , m_pCommands(nullptr)
    , m_pFlatFileModel(new CFlatFileModel(this, this))
    , m_pFlatFileModelProxy(new CFlatFileModelProxy(pController, this))
    , m_pStagedFileModelProxy(new CStagedFileModelProxy(pController, this))
    , m_pBranchModel(new CBranchModel(this))
    , m_pTagModel(new CBranchModel(this))
    , m_pLogModel(new CLogModel(this, this))
    , m_pFileDiffModel(new CDiffModel(this))
    , m_pFileLogModel(new CLogModel(this, this))
    , m_pGraphModel(new CGraphModel(this, this))
    , m_pCommandOutputModel(new QStringListModel(this))
    , m_iCommitCountAhead(0)
    , m_iCommitCountBehind(0)
    , m_bHasModifiedFiles(false)
    , m_bHasCommitableFiles(false)
    , m_bHasPushableCommits(false)
    , m_bHasPullableCommits(false)
    , m_bHasStashableFiles(false)
{
    if (eType == CEnums::UnknownRepositoryType)
    {
        eType = getRepositoryTypeFromFolder(sPath);
    }

    setRepositoryType(eType);

    m_sRepositoryName = sPath.split(PATH_SEP).last();

    // Command return values
    connect(m_pCommands, &CCommands::newOutputString, this, &CRepository::onNewOutputString);
    connect(m_pCommands, &CCommands::newOutputKeyValue, this, &CRepository::onNewOutputKeyValue);
    connect(m_pCommands, &CCommands::newOutputStringList, this, &CRepository::onNewOutputStringList);
    connect(m_pCommands, &CCommands::newOutputListOfCBranch, this, &CRepository::onNewOutputListOfCBranch);
    connect(m_pCommands, &CCommands::newOutputListOfCRepoFile, this, &CRepository::onNewOutputListOfCRepoFile);
    connect(m_pCommands, &CCommands::newOutputListOfCLogLine, this, &CRepository::onNewOutputListOfCLogLine);
    connect(m_pCommands, &CCommands::newOutputListOfCDiffLine, this, &CRepository::onNewOutputListOfCDiffLine);
    connect(m_pCommands, &CCommands::newOutputListOfCGraphLine, this, &CRepository::onNewOutputListOfCGraphLine);

    m_pBranchModel->setBranchList(QList<CBranch*>());
    m_pTagModel->setBranchList(QList<CBranch*>());
    m_pLogModel->setLines(QList<CLogLine*>());
    m_pFileDiffModel->setLines(QList<CDiffLine*>());
    m_pFileLogModel->setLines(QList<CLogLine*>());
    m_pGraphModel->setLines(QList<CGraphLine*>());

    connect(m_pFlatFileModel, &CFlatFileModel::currentFileFullName, this, &CRepository::onCurrentFileFullName);

    m_pFlatFileModelProxy->setSourceModel(m_pFlatFileModel);
    m_pStagedFileModelProxy->setSourceModel(m_pFlatFileModel);

    connect(this, &CRepository::diffToCommitIdChanged, this, &CRepository::onDiffCommitIdChanged);
    connect(this, &CRepository::diffFromCommitIdChanged, this, &CRepository::onDiffCommitIdChanged);

    checkAllFileStatus();
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

void CRepository::setRepositoryType(CEnums::ERepositoryType eType)
{
    if (m_eRepositoryType != eType)
    {
        m_eRepositoryType = eType;

        if (m_pCommands != nullptr)
            delete m_pCommands;

        // Create the command interface
        m_pCommands = getCommandsForRepositoryType(m_eRepositoryType);

        emit repositoryTypeChanged();
        emit repositoryTypeStringChanged();
    }
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

CRepoFile* CRepository::fileByFullName(const QString& sFullName) const
{
    auto itr = std::find_if(m_lRepoFiles.begin(), m_lRepoFiles.end(), [sFullName](CRepoFile* pFile) { return pFile->fullName() == sFullName; });

    if (itr != m_lRepoFiles.end())
        return *itr;

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CRepository::fileByFullName(QList<CRepoFile*> lRepoFiles, const QString& sFullName) const
{
    auto itr = std::find_if(lRepoFiles.begin(), lRepoFiles.end(), [sFullName](CRepoFile* pFile) { return pFile->fullName() == sFullName; });

    if (itr != lRepoFiles.end())
        return *itr;

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

QStringList CRepository::labelsForCommit(const QString& sCommitId) const
{
    QStringList lReturnValue;
    lReturnValue << m_pBranchModel->labelsForCommit(sCommitId);

    QStringList lTags = m_pTagModel->labelsForCommit(sCommitId);
    for (QString sTag : lTags)
        lReturnValue << "Tag: " + sTag;

    return lReturnValue;
}

//-------------------------------------------------------------------------------------------------

QString CRepository::repositoryTypeString() const
{
    switch(m_eRepositoryType)
    {
    case CEnums::UnknownRepositoryType: return "?";
    case CEnums::Git: return "Git";
    case CEnums::Gerrit: return "Gerrit";
    case CEnums::CVS: return "CVS";
    case CEnums::SVN: return "SVN";
    case CEnums::HG: return "Mercurial";
    }

    return "";
}

//-------------------------------------------------------------------------------------------------

void CRepository::clearOutput()
{
    m_pCommandOutputModel->setStringList(QStringList());
}

//-------------------------------------------------------------------------------------------------

void CRepository::copy(const QString& sText)
{
    QClipboard* pClipboard = QGuiApplication::clipboard();
    pClipboard->setText(sText);
}


//-------------------------------------------------------------------------------------------------

void CRepository::openFile(const QString& sFullName)
{
    QDesktopServices::openUrl(QUrl(sFullName));
}

//-------------------------------------------------------------------------------------------------

bool CRepository::can(CEnums::ECapability eWhat)
{
    return m_pCommands->can(eWhat);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkRepositoryStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->repositoryStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkAllFileStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->allFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkChangedFileStatus(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->changedFileStatus(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::checkFileStatus(const QString& sFileFullName)
{
    m_pCommands->fileStatus(m_sRepositoryPath, sFileFullName);
}

//-------------------------------------------------------------------------------------------------

void CRepository::refresh()
{
    checkRepositoryStatus();
    checkChangedFileStatus();
    getBranches();
    getTags();
    getBranchLog();
    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CRepository::toggleStaged(QString sFullName)
{
    if (not sFullName.isEmpty())
    {
        m_pCommands->toggleStaged(m_sRepositoryPath, sFullName);
    }
    checkChangedFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::stageSelection(QStringList lFileFullNames)
{
    for (QString sFullName : lFileFullNames)
    {
        if (not sFullName.isEmpty())
        {
            m_pCommands->stageFile(m_sRepositoryPath, sFullName, true);
        }
    }
    checkChangedFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::unstageSelection(QStringList lFileFullNames)
{
    for (QString sFullName : lFileFullNames)
    {
        if (not sFullName.isEmpty())
        {
            m_pCommands->stageFile(m_sRepositoryPath, sFullName, false);
        }
    }
    checkChangedFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::stageAll()
{
    m_pCommands->stageAll(m_sRepositoryPath, true);
    checkChangedFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::unstageAll()
{
    m_pCommands->stageAll(m_sRepositoryPath, false);
    checkChangedFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CRepository::revertSelection(QStringList lFileFullNames)
{
    for (QString sFullName : lFileFullNames)
    {
        m_pCommands->revertFile(m_sRepositoryPath, sFullName);
    }
    checkChangedFileStatus();
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

void CRepository::stashSave()
{
    m_pCommands->stashSave(m_sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::stashPop()
{
    m_pCommands->stashPop(m_sRepositoryPath);
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
    m_pCommands->createBranchOnCommit(m_sRepositoryPath, sCommitId, sBranchName);
}

//-------------------------------------------------------------------------------------------------

void CRepository::changeCommitMessage(const QString& sCommitId, const QString& sMessage)
{
    m_pCommands->changeCommitMessage(m_sRepositoryPath, sCommitId, sMessage);
}

//-------------------------------------------------------------------------------------------------

void CRepository::mergeBranch(const QString& sName)
{
    m_pCommands->mergeBranch(m_sRepositoryPath, sName);
}

//-------------------------------------------------------------------------------------------------

void CRepository::deleteBranch(const QString& sName)
{
    m_pCommands->deleteBranch(m_sRepositoryPath, sName);
}

//-------------------------------------------------------------------------------------------------

void CRepository::createTagOnCommit(const QString& sCommitId, const QString& sTagName, const QString& sMessage)
{
    m_pCommands->createTagOnCommit(m_sRepositoryPath, sCommitId, sTagName, sMessage);
}

//-------------------------------------------------------------------------------------------------

void CRepository::commitDiffPrevious(const QString& sCommitId)
{
    m_pCommands->commitDiffPrevious(m_sRepositoryPath, sCommitId);
}

//-------------------------------------------------------------------------------------------------

CEnums::ERepositoryType CRepository::getRepositoryTypeFromFolder(const QString& sPath)
{
    if (QDir(QString("%1/.git").arg(sPath)).exists())
    {
        QString sCommitMsgFile = QString("%1/.git/hooks/commit-msg").arg(sPath);
        QString sCommitMsgContents = CUtils::getTextFileContents(sCommitMsgFile);

        if (sCommitMsgContents.contains("Gerrit") && sCommitMsgContents.contains("add_ChangeId()"))
            return CEnums::Gerrit;

        return CEnums::Git;
    }

    if (QDir(QString("%1/.svn").arg(sPath)).exists())
    {
        return CEnums::SVN;
    }

    if (QDir(QString("%1/.hg").arg(sPath)).exists())
    {
        return CEnums::HG;
    }

    return CEnums::UnknownRepositoryType;
}

//-------------------------------------------------------------------------------------------------

CEnums::ERepositoryType CRepository::getRepositoryTypeFromURL(const QString& sRepositoryURL)
{
    if (sRepositoryURL.contains(".git"))
        return CEnums::Git;

    if (sRepositoryURL.contains("gerrit"))
        return CEnums::Gerrit;

    if (sRepositoryURL.contains("/svn"))
        return CEnums::SVN;

    if (sRepositoryURL.contains("/hg"))
        return CEnums::HG;

    return CEnums::UnknownRepositoryType;
}

//-------------------------------------------------------------------------------------------------

CCommands* CRepository::getCommandsForRepositoryType(CEnums::ERepositoryType eType)
{
    switch (eType)
    {

    case CEnums::Git:
        return new CGitCommands();

    case CEnums::Gerrit:
        return new CGerritCommands();

    case CEnums::SVN:
        return new CSvnCommands();

    case CEnums::HG:
        return new CHgCommands();

    default:
        return new CCommands();

    }
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

void CRepository::getBranchesCommitCountAheadBehind(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->branchCommitCountAheadBehind(sPath, m_pBranchModel->branchNames());
}

//-------------------------------------------------------------------------------------------------

void CRepository::getTags(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->tags(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::getTagCommits(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->tagCommits(sPath, m_pTagModel->branchNames());
}

//-------------------------------------------------------------------------------------------------

void CRepository::getGraph(QString sPath)
{
    if (sPath.isEmpty())
        sPath = m_sRepositoryPath;

    m_pCommands->graph(sPath);
}

//-------------------------------------------------------------------------------------------------

void CRepository::getBranchLog(QString sPath)
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
    QStringList lNewList = sOutput.split(NEW_LINE);
    QStringList lData = m_pCommandOutputModel->stringList();
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

        m_pCommandOutputModel->setStringList(lData);
    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputString(CEnums::EProcessCommand eCommand, QString sOutput)
{
    switch (eCommand)
    {

    case CEnums::eCloneRepository:
    {
        onNewOutput(sOutput);
        break;
    }

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

    case CEnums::eStashSave:
    case CEnums::eStashPop:
    {
        onNewOutput(sOutput);

        checkChangedFileStatus();
        break;
    }

    case CEnums::eCreateTagOnCommit:
    {
        onNewOutput(sOutput);

        getTags();
        break;
    }

    case CEnums::eCommit:
    case CEnums::eAmend:
    case CEnums::ePush:
    case CEnums::ePull:
    case CEnums::eFetch:
    {
        onNewOutput(sOutput);

        refresh();
        break;
    }

    case CEnums::eSetCurrentBranch:
    case CEnums::eResetToCommit:
    case CEnums::eRebaseOnCommit:
    case CEnums::eSquashCommit:
    case CEnums::eCreateBranchOnCommit:
    case CEnums::eMergeBranch:
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
        m_pGraphModel->commitChanged(sValue);
        break;
    }

    case CEnums::eTagCommit:
    {
        m_pTagModel->setBranchHeadCommit(sKey, sValue);
        m_pLogModel->commitChanged(sValue);
        m_pGraphModel->commitChanged(sValue);
        break;
    }

    case CEnums::eBranchCommitCountAhead:
    {
        if (sKey == m_sCurrentBranch)
        {
            setCommitCountAhead(sValue.toInt());
            setHasPushableCommits(m_iCommitCountAhead > 0);
        }
        break;
    }

    case CEnums::eBranchCommitCountBehind:
    {
        if (sKey == m_sCurrentBranch)
        {
            setCommitCountBehind(sValue.toInt());
            setHasPullableCommits(m_iCommitCountBehind > 0);
        }
        break;
    }

    case CEnums::eCommitMessage:
    {
        m_pLogModel->setCommitMessage(sKey, sValue);
        m_pGraphModel->setCommitMessage(sKey, sValue);
        m_pFileLogModel->setCommitMessage(sKey, sValue);
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
        getBranchesCommitCountAheadBehind();
        break;
    }

    case CEnums::eTags:
    {
        m_pTagModel->setBranchList(lNewBranches);
        getTagCommits();
        break;
    }

    default:
    {
        break;
    }

    }
}

//-------------------------------------------------------------------------------------------------

void CRepository::onNewOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, QList<CRepoFile*> lNewFiles)
{
    switch (eCommand)
    {

    case CEnums::eAllFileStatus:
    case CEnums::eChangedFileStatus:
    {
        if (not lNewFiles.isEmpty())
        {
            QHash<QString, CRepoFile*> hNewFiles;

            for (CRepoFile* pFile : lNewFiles)
            {
                hNewFiles[pFile->fullName()] = pFile;
            }

            bool bHasModifiedFiles = false;
            bool bHasCommitableFiles = false;

            for (CRepoFile* pNewFile : lNewFiles)
            {
                QString sNewKey = pNewFile->fullName();

                if (pNewFile->status() != CEnums::eIgnored && pNewFile->status() != CEnums::eClean)
                {
                    bHasModifiedFiles = true;

                    if (pNewFile->staged())
                        bHasCommitableFiles = true;
                }

                if (m_hHashRepoFiles.contains(sNewKey))
                {
                    CRepoFile* pExistingFile = m_hHashRepoFiles[sNewKey];
                    m_lRepoFiles.removeAll(pExistingFile);
                    delete pExistingFile;
                }

                m_lRepoFiles << pNewFile;
                m_hHashRepoFiles[sNewKey] = pNewFile;
            }

            for (CRepoFile* pExistingFile : m_lRepoFiles)
            {
                QString sExistingKey = pExistingFile->fullName();

                if (not hNewFiles.contains(sExistingKey))
                {
                    pExistingFile->setStaged(false);
                    pExistingFile->setStatus(CEnums::eClean);
                }
            }

            setHasModifiedFiles(bHasModifiedFiles);
            setHasCommitableFiles(bHasCommitableFiles);

            std::sort(m_lRepoFiles.begin(), m_lRepoFiles.end(), [] (CRepoFile* left, CRepoFile* right) {
                return left->fullName() < right->fullName();
            });

            // Update the models
            m_pFlatFileModel->handleRepoFilesChanged();
            m_pFlatFileModelProxy->filterChanged();
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
    case CEnums::eTwoCommitDiff:
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

//-------------------------------------------------------------------------------------------------

void CRepository::onDiffCommitIdChanged()
{
    QString sDiffFromCommitId = m_sDiffFromCommitId;
    QString sDiffFromToId = m_sDiffToCommitId;

    if (not m_sDiffFromCommitId.isEmpty() && not m_sDiffToCommitId.isEmpty())
    {
        m_pCommands->twoCommitDiff(m_sRepositoryPath, m_sDiffFromCommitId, m_sDiffToCommitId);

        setDiffFromCommitId("");
        setDiffToCommitId("");
    }

    m_pLogModel->commitChanged(sDiffFromCommitId);
    m_pLogModel->commitChanged(sDiffFromToId);
    m_pGraphModel->commitChanged(sDiffFromCommitId);
    m_pGraphModel->commitChanged(sDiffFromToId);
}

//-------------------------------------------------------------------------------------------------

void CRepository::onShouldRefreshFileStatus()
{
    checkRepositoryStatus();
    checkChangedFileStatus();
}
