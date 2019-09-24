
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

CCommands::CCommands(CController* pController)
    : m_pController(pController)
{
}

//-------------------------------------------------------------------------------------------------

CCommands::~CCommands()
{
}

//-------------------------------------------------------------------------------------------------

bool CCommands::can(CEnums::ECapability eWhat) const
{
    Q_UNUSED(eWhat);
    return false;
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

void CCommands::branchLog(const QString& sPath, int iFrom, int iCount)
{
    Q_UNUSED(sPath);
    Q_UNUSED(iFrom);
    Q_UNUSED(iCount);
}

//-------------------------------------------------------------------------------------------------

void CCommands::fileLog(const QString& sPath, const QString& sFullName, int iFrom, int iCount)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
    Q_UNUSED(iFrom);
    Q_UNUSED(iCount);
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

void CCommands::patchApply(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::unstagedFileDiff(const QString& sPath, const QString& sFullName)
{
    Q_UNUSED(sPath);
    Q_UNUSED(sFullName);
}

//-------------------------------------------------------------------------------------------------

void CCommands::blame(const QString& sPath, const QString& sFullName)
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
