
#pragma once

// Qt
#include <QObject>

//-------------------------------------------------------------------------------------------------

class CEnums : public QObject
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Enumerators exposed to QML
    //-------------------------------------------------------------------------------------------------

    // Capabilities of a versioning system
    enum ECapability
    {
        Clone,
        ShowGlobalGraph,
        ShowBranchLog,
        ShowFileLog,
        Commit,
        Amend,
        Push,
        Pull,
        SquashCommit,
        ChangeCommitMessage,
        ResetToCommit,
        RebaseOnCommit,
        ContinueRebase,
        AbortRebase,
    };

    // Type of versioning system
    enum ERepositoryType
    {
        UnknownRepositoryType,
        GIT,
        CVS,
        SVN
    };

    // Status of a repository
    enum ERepositoryStatus
    {
        NoMerge,
        Merge,
        Rebase,
        InteractiveRebase
    };

    // Diff operations
    enum EDiffOperation
    {
        None,
        Add,
        Delete
    };

    Q_ENUMS(ECapability)
    Q_ENUMS(ERepositoryType)
    Q_ENUMS(ERepositoryStatus)
    Q_ENUMS(EDiffOperation)

    //-------------------------------------------------------------------------------------------------
    // Internal enumerators
    //-------------------------------------------------------------------------------------------------

    // Command identifiers
    enum EProcessCommand
    {
        eNotification,
        eAllFileStatus,
        eRepositoryStatus,
        eBranches,
        eBranchHeadCommits,
        eCurrentBranch,
        eGraph,
        eBranchLog,
        eFileLog,
        eStageFile,
        eStageAll,
        eRevertFile,
        eCommit,
        eAmend,
        ePush,
        ePull,
        eUnstagedFileDiff,
        eSetCurrentBranch,
        eCommitReset,
        eCommitRebase,
        eCommitSquash,
        eChangeCommitMessage,
        eContinueRebase,
        eAbortRebase
    };

    // Status of a file in repository
    enum ERepoFileStatus
    {
        eClean,
        eAdded,
        eModified,
        eRenamed,
        eDeleted,
        eUntracked,
        eIgnored
    };
};
