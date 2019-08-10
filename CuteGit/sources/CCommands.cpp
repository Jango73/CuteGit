
// Qt
#include <QDebug>
#include <QCoreApplication>
#include <QProcess>
#include <QMutexLocker>
#include <QDir>

// Application
#include "CCommands.h"

/*!
    \class CCommands
    \inmodule CuteGit
    \section1 General
    This is the base class for all repository control interfaces.
    It allows execution of a process in background.

    \code

        _.-._         ..-..         _.-._
       (_-.-_)       /|'.'|\       (_'.'_)
     mrf.\-/.        \)\-/(/        ,-.-.
     __/ /-. \__   __/ ' ' \__   __/'-'-'\__
    ( (___/___) ) ( (_/-._\_) ) ( (_/   \_) )
     '.Oo___oO.'   '.Oo___oO.'   '.Oo___oO.'

    \endcode
*/

//-------------------------------------------------------------------------------------------------

CCommands::CCommands()
    : m_tPool(this)
    , m_bStop(false)
    , m_mMutex(QMutex::Recursive)
{
    start();
}

//-------------------------------------------------------------------------------------------------

CCommands::~CCommands()
{
    m_bStop = true;
    wait();
}

//-------------------------------------------------------------------------------------------------

bool CCommands::can(CEnums::ECapability eWhat) const
{
    Q_UNUSED(eWhat);
    return false;
}

//-------------------------------------------------------------------------------------------------

// This method does NOT run in the main thread
// Watch out for data integrity protection
void CCommands::run()
{
    while (not m_bStop)
    {
        bool bStackEmpty = true;
        CProcessCommand* pCommand = nullptr;

        // Open a scope for the mutex locker
        {
            // Lock access to the command stack
            QMutexLocker locker(&m_mMutex);

            if (m_lCommandStack.count() > 0)
            {
                bStackEmpty = false;
                pCommand = m_lCommandStack[0];
                m_lCommandStack.removeAt(0);
            }
        }

        if (pCommand != nullptr)
        {
            if (pCommand->m_eEndSignal == CEnums::eNothing)
            {
                QString sOutput = execNow(
                            pCommand->m_sWorkPath,
                            pCommand->m_sCommand,
                            pCommand->m_mEnvironment
                            );

                emit execFinished(pCommand->m_sWorkPath, pCommand->m_eCommand, sOutput, pCommand->m_sUserData);
            }
            else
            {
                QString sOutput = execNowLiveFeed(
                            pCommand->m_eCommand,
                            pCommand->m_sWorkPath,
                            pCommand->m_sCommand,
                            pCommand->m_mEnvironment
                            );

                emit execFinished(pCommand->m_sWorkPath, pCommand->m_eEndSignal, sOutput, pCommand->m_sUserData);
            }

            delete pCommand;
        }

        msleep(bStackEmpty ? 100 : 10);
    }
}

//-------------------------------------------------------------------------------------------------

void CCommands::exec(CProcessCommand* pCommand)
{
    // Lock access to the command stack
    QMutexLocker locker(&m_mMutex);

    // Leave only one command of a given type in the stack
    // if not allowed to stack the type
    if (not pCommand->m_bAllowStack)
    {
        for (int index = 0; index < m_lCommandStack.count(); index++)
        {
            if (m_lCommandStack[index]->m_eCommand == pCommand->m_eCommand)
            {
                delete m_lCommandStack[index];
                m_lCommandStack.removeAt(index);
                index--;
            }
        }
    }

    m_lCommandStack << pCommand;
}

//-------------------------------------------------------------------------------------------------

QString CCommands::execNow(QString sWorkPath, QString sCommand, QMap<QString, QString> mEnvironment)
{
    QProcess process;

    if (not sWorkPath.isEmpty())
        process.setWorkingDirectory(sWorkPath);

    if (not mEnvironment.isEmpty())
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        for (QString sKey : mEnvironment.keys())
            env.insert(sKey, mEnvironment[sKey]);

        process.setProcessEnvironment(env);
    }

    process.start(sCommand);
    process.waitForFinished();

    QString sOutput = process.readAllStandardOutput();
    sOutput += process.readAllStandardError();

    return sOutput;
}

//-------------------------------------------------------------------------------------------------

QString CCommands::execNowLiveFeed(CEnums::EProcessCommand eCommand, QString sWorkPath, QString sCommand, QMap<QString, QString> mEnvironment)
{
    QProcess process;

    if (not sWorkPath.isEmpty())
        process.setWorkingDirectory(sWorkPath);

    if (not mEnvironment.isEmpty())
    {
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        for (QString sKey : mEnvironment.keys())
            env.insert(sKey, mEnvironment[sKey]);

        process.setProcessEnvironment(env);
    }

    process.start(sCommand);
    msleep(1000);

    while (process.state() == QProcess::Running)
    {
        QCoreApplication::processEvents();

        QString sOutput = process.readAllStandardOutput();
        sOutput += process.readAllStandardError();

        if (not sOutput.isEmpty())
        {
            emit newOutputString(eCommand, sOutput);
        }

        if (m_bStop || QThread::currentThread()->isInterruptionRequested())
            break;

        msleep(20);
    }

    process.waitForFinished();
    QString sOutput = process.readAllStandardOutput();
    sOutput += process.readAllStandardError();

    return sOutput;
}

//-------------------------------------------------------------------------------------------------

void CCommands::cloneRepository(const QString& sRepositoryURL, const QString& sRepositoryPath)
{
    Q_UNUSED(sRepositoryURL);
    Q_UNUSED(sRepositoryPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::repositoryStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::allFileStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::changedFileStatus(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::fileStatus(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::branches(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::branchHeadCommits(const QString& sPath, QStringList lBranches)
{
    Q_UNUSED(sPath);
    Q_UNUSED(lBranches);
}

//-------------------------------------------------------------------------------------------------

void CCommands::branchCommitCountAheadBehind(const QString& sPath, QStringList lBranches)
{
    Q_UNUSED(sPath);
    Q_UNUSED(lBranches);
}

//-------------------------------------------------------------------------------------------------

void CCommands::tags(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::tagCommits(const QString& sPath, QStringList lTags)
{
    Q_UNUSED(sPath);
    Q_UNUSED(lTags);
}

//-------------------------------------------------------------------------------------------------

void CCommands::graph(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to)
{
    Q_UNUSED(sPath);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

//-------------------------------------------------------------------------------------------------

void CCommands::fileLog(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::toggleStaged(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::stageFile(const QString& sPath, const QString& sFullName, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    Q_UNUSED(bStage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::stageAll(const QString& sPath, bool bStage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(bStage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::revertFile(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commit(const QString& sPath, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sMessage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::amend(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::push(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::pull(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::fetch(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::stashSave(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::stashPop(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::unstagedFileDiff(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commitDiffPrevious(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::twoCommitDiff(const QString& sPath, const QString& sFromCommitId, const QString& sToCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFromCommitId);
    Q_UNUSED(sToCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::setCurrentBranch(const QString& sPath, const QString& sBranch)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sBranch);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commitReset(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commitRebase(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::commitSquash(const QString& sPath, const QString& sCommitId)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
}

//-------------------------------------------------------------------------------------------------

void CCommands::changeCommitMessage(const QString& sPath, const QString& sCommitId, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
    Q_UNUSED(sMessage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::continueRebase(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::abortRebase(const QString& sPath)
{
    Q_UNUSED(sPath);
}

//-------------------------------------------------------------------------------------------------

void CCommands::createBranchOnCommit(const QString& sPath, const QString& sCommitId, const QString& sBranchName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
    Q_UNUSED(sBranchName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::mergeBranch(const QString& sPath, const QString& sBranchName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sBranchName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::deleteBranch(const QString& sPath, const QString& sBranchName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sBranchName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::createTagOnCommit(const QString& sPath, const QString& sCommitId, const QString& sTagName, const QString& sMessage)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sCommitId);
    Q_UNUSED(sTagName);
    Q_UNUSED(sMessage);
}

//-------------------------------------------------------------------------------------------------

void CCommands::editSequenceFile(const QString& sFileName)
{
    Q_UNUSED(sFileName);
}

//-------------------------------------------------------------------------------------------------

void CCleanFileLister::run()
{
    QList<CRepoFile*> lFileList;
    getAllFiles(lFileList, m_sRootPath, m_sRootPath);

    emit newOutputListOfCRepoFile(m_eCommand, lFileList);
}

//-------------------------------------------------------------------------------------------------

void CCleanFileLister::getAllFiles(QList<CRepoFile*>& lFileList, const QString& sRootPath, const QString& sCurrentPath)
{
    QStringList slNameFilter;
    slNameFilter << "*.*";

    QDir dRoot(sRootPath);
    QDir dDirectory(sCurrentPath);

    dDirectory.setFilter(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Files);
    QStringList lFiles = dDirectory.entryList(slNameFilter);

    for (QString sFile : lFiles)
    {
        QString sFullName = QString("%1/%2").arg(sCurrentPath).arg(sFile);
        QFileInfo info(sFullName);

        CRepoFile* pNewFile = new CRepoFile();

        pNewFile->setStatus(CEnums::eClean);
        pNewFile->setFullName(sFullName);
        pNewFile->setRelativeName(dRoot.relativeFilePath(sFullName));
        pNewFile->setFileName(info.fileName());

        lFileList << pNewFile;
    }

    dDirectory.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList lDirectories = dDirectory.entryList();

    for (QString sNewDirectory : lDirectories)
    {
        QString sFullName = QString("%1/%2").arg(sCurrentPath).arg(sNewDirectory);

        getAllFiles(lFileList, sRootPath, sFullName);
    }
}
