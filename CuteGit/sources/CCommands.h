
#pragma once

// Qt
#include <QObject>

// Application
#include "CEnums.h"
#include "CExecution.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CCommands : public CExecution
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CCommands(CController* pController);

    //! Destructor
    virtual ~CCommands() override;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    virtual bool can(CEnums::ECapability eWhat) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Clones a repository
    virtual void cloneRepository(const QString& sRepositoryURL, const QString& sRepositoryPath);

    //! Checks the status of the repo at sPath
    virtual void repositoryStatus(const QString& sPath);

    //! Creates a list of CRepoFile from the repo at sPath
    virtual void allFileStatus(const QString& sPath);

    //! Creates a list of CRepoFile from the repo at sPath
    virtual void changedFileStatus(const QString& sPath);

    //! Creates a list of CRepoFile from the repo at sPath
    virtual void fileStatus(const QString& sPath, const QString& sFullName);

    //! Creates a list of branches of the repo at sPath
    virtual void branches(const QString& sPath);

    //! Creates a list of head commits for branches of the repo at sPath
    virtual void branchHeadCommits(const QString& sPath, QStringList lBranches);

    //! Creates a list of commit ahead counts for branches of the repo at sPath
    virtual void branchCommitCountAheadBehind(const QString& sPath, QStringList lBranches);

    //! Creates a list of tags of the repo at sPath
    virtual void tags(const QString& sPath);

    //! Creates a list of head commits for tags of the repo at sPath
    virtual void tagCommits(const QString& sPath, QStringList lTags);

    //! Creates a list of CGraphLine from the repo at sPath
    virtual void graph(const QString& sPath);

    //! Creates a list of CLogLine from the repo at sPath, for the current branch
    virtual void branchLog(const QString& sPath, int iFrom = 0, int iCount = LOG_COUNT_DEFAULT);

    //! Creates a list of CLogLine from the repo at sPath, for sFullName
    virtual void fileLog(const QString& sPath, const QString& sFullName, int iFrom = 0, int iCount = LOG_COUNT_DEFAULT);

    //! Creates a list of CLogLine from the repo at sPath, for the ref log
    virtual void refLog(const QString& sPath, int iFrom = 0, int iCount = REF_LOG_COUNT_DEFAULT);

    //! Toggles the 'staged' state of sFullName in the repo at sPath
    virtual void toggleStaged(const QString& sPath, const QString& sFullName);

    //! Stages or unstages sFullName in the repo at sPath
    virtual void stageFile(const QString& sPath, const QString& sFullName, bool bStage);

    //! Stages or unstages all files in the repo at sPath
    virtual void stageAll(const QString& sPath, bool bStage);

    //! Reverts all modifications of sFullName in the repo at sPath
    //! Use with caution
    virtual void revertFile(const QString& sPath, const QString& sFullName);

    //! Commits all staged files in the repo at sPath, using sMessage
    virtual void commit(const QString& sPath, const QString& sMessage);

    //! Amends all staged files in the repo at sPath
    virtual void amend(const QString& sPath);

    //!
    virtual void push(const QString& sPath);

    //!
    virtual void pull(const QString& sPath);

    //!
    virtual void fetch(const QString& sPath);

    //!
    virtual void stashSave(const QString& sPath);

    //!
    virtual void stashPop(const QString& sPath);

    //!
    virtual void patchApply(const QString& sPath, const QString& sFullName);

    //!
    virtual void unstagedFileDiff(const QString& sPath, const QString& sFullName);

    //!
    virtual void blame(const QString& sPath, const QString& sFullName);

    //!
    virtual void commitDiffPrevious(const QString& sPath, const QString& sCommitId);

    //!
    virtual void twoCommitDiff(const QString& sPath, const QString& sFromCommitId, const QString& sToCommitId);

    //!
    virtual void setCurrentBranch(const QString& sPath, const QString& sBranch);

    //!
    virtual void commitReset(const QString& sPath, const QString& sCommitId);

    //!
    virtual void commitRebase(const QString& sPath, const QString& sCommitId);

    //!
    virtual void commitSquash(const QString& sPath, const QString& sCommitId);

    //!
    virtual void changeCommitMessage(const QString& sPath, const QString& sCommitId, const QString& sMessage);

    //!
    virtual void continueRebase(const QString& sPath);

    //!
    virtual void abortRebase(const QString& sPath);

    //!
    virtual void createBranchOnCommit(const QString& sPath, const QString& sCommitId, const QString& sBranchName);

    //!
    virtual void mergeBranch(const QString& sPath, const QString& sBranchName);

    //!
    virtual void deleteBranch(const QString& sPath, const QString& sBranchName);

    //!
    virtual void createTagOnCommit(const QString& sPath, const QString& sCommitId, const QString& sTagName, const QString& sMessage);

    //!
    virtual void editSequenceFile(const QString& sFileName);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    CController*    m_pController;
};
