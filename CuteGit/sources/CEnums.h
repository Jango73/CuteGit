
#pragma once

// Qt
#include <QObject>
#include <QtGlobal>

//-------------------------------------------------------------------------------------------------

// Character constants
#define PATH_SEP    "/"
#define NEW_LINE	"\n"

// Qt features
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
#else
#define HAVE_QSTRING_BACK
#endif

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
        Stash,
        Commit,
        Amend,
        Push,
        Pull,
        Fetch,
        SquashCommit,
        ChangeCommitMessage,
        ResetToCommit,
        RebaseOnCommit,
        BranchFromCommit,
        MergeBranch,
        DeleteBranch,
        ContinueRebase,
        AbortRebase,
    };

    // Type of versioning system
    enum ERepositoryType
    {
        UnknownRepositoryType,
        Git,
        Gerrit,
        CVS,
        SVN,
        HG,     // Mercurial
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
        eNothing,
        eNotification,
        eCloneRepository,
        eCloneRepositoryFinished,
        eAllFileStatus,
        eRepositoryStatus,
        eBranches,
        eBranchHeadCommit,
        eBranchCommitCountAhead,
        eBranchCommitCountBehind,
        eTags,
        eTagCommit,
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
        eFetch,
        eStashSave,
        eStashPop,
        eUnstagedFileDiff,
        eTwoCommitDiff,
        eSetCurrentBranch,
        eResetToCommit,
        eRebaseOnCommit,
        eSquashCommit,
        eChangeCommitMessage,
        eBranchFromCommit,
        eMergeBranch,
        eDeleteBranch,
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
        eMissing,
        eUntracked,
        eIgnored
    };
};
