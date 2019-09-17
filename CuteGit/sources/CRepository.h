
#pragma once

// Qt
#include <QStringListModel>
#include <QHash>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

// Application
#include "CEnums.h"
#include "CLabel.h"
#include "CBranchModel.h"
#include "CFlatFileModel.h"
#include "CFlatFileModelProxy.h"
#include "CStagedFileModelProxy.h"
#include "CRepoFile.h"
#include "CLogModel.h"
#include "CLogModelProxy.h"
#include "CDiffModel.h"
#include "CGraphModel.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CRepository : public QObject
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    // Informations on the repo
    Q_FAST_PROPERTY_NO_SET_IMPL(CEnums::ERepositoryType, e, repositoryType, RepositoryType)
    Q_FAST_PROPERTY(CEnums::ERepositoryStatus, e, repositoryStatus, RepositoryStatus)
    Q_FAST_PROPERTY(QString, s, repositoryPath, RepositoryPath)
    Q_FAST_PROPERTY(QString, s, repositoryName, RepositoryName)

    // From and to commit Ids
    Q_FAST_PROPERTY(QString, s, diffFromCommitId, DiffFromCommitId)
    Q_FAST_PROPERTY(QString, s, diffToCommitId, DiffToCommitId)

    // Main controller
    Q_FAST_PROPERTY(CController*, p, controller, Controller)

    // Versioning system interface
	Q_FAST_PROPERTY(CCommands*, p, commands, Commands)

    // Models for files
    Q_FAST_PROPERTY(CFlatFileModel*, p, flatFileModel, FlatFileModel)
    Q_FAST_PROPERTY(CFlatFileModelProxy*, p, flatFileModelProxy, FlatFileModelProxy)
    Q_FAST_PROPERTY(CStagedFileModelProxy*, p, stagedFileModelProxy, StagedFileModelProxy)

    // Models for branchs and tags
    Q_FAST_PROPERTY(CBranchModel*, p, branchModel, BranchModel)
    Q_FAST_PROPERTY(CBranchModel*, p, tagModel, TagModel)

    // Models for various logs
    Q_FAST_PROPERTY(CLogModel*, p, branchLogModel, BranchLogModel)
    Q_FAST_PROPERTY(CLogModelProxy*, p, branchLogModelProxy, BranchLogModelProxy)
    Q_FAST_PROPERTY(CLogModel*, p, fileLogModel, FileLogModel)
    Q_FAST_PROPERTY(CGraphModel*, p, graphModel, GraphModel)

    // Model for the diff view
    Q_FAST_PROPERTY(CDiffModel*, p, fileDiffModel, FileDiffModel)

    // Output of commands
    Q_FAST_PROPERTY(QStringListModel*, p, commandOutputModel, CommandOutputModel)

    // Name of the current branch
    Q_FAST_PROPERTY_NO_SET_IMPL(QString, s, currentBranch, CurrentBranch)

    // List of files in the repo
    Q_FAST_PROPERTY(QList<CRepoFile*>, l, repoFiles, RepoFiles)
    Q_FAST_PROPERTY(CHashOfRepoFile, h, hashRepoFiles, HashRepoFiles)

    // Number of commits ahead and behind
    Q_FAST_PROPERTY(int, i, commitCountAhead, CommitCountAhead)
    Q_FAST_PROPERTY(int, i, commitCountBehind, CommitCountBehind)

    // Values that drive visibility of actions
    Q_FAST_PROPERTY(bool, b, hasModifiedFiles, HasModifiedFiles)
    Q_FAST_PROPERTY(bool, b, hasCommitableFiles, HasCommitableFiles)
    Q_FAST_PROPERTY(bool, b, hasPushableCommits, HasPushableCommits)
    Q_FAST_PROPERTY(bool, b, hasPullableCommits, HasPullableCommits)
    Q_FAST_PROPERTY(bool, b, hasStashableFiles, HasStashableFiles)

    Q_PROPERTY(QString repositoryTypeString READ repositoryTypeString NOTIFY repositoryTypeStringChanged)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CRepository(const QString& sPath, CController* pController, QObject *parent = nullptr, CEnums::ERepositoryType eType = CEnums::UnknownRepositoryType);

    //! Destructor
    virtual ~CRepository() override;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    CRepoFile* fileByFullName(const QString& sFullName) const;

    //!
    CRepoFile* fileByFullName(QList<CRepoFile*> lRepoFiles, const QString& sFullName) const;

    //!
    QList<CLabel*> labelsForCommit(const QString& sCommitId) const;

    //!
    QString repositoryTypeString() const;

    //-------------------------------------------------------------------------------------------------
    // Invokable control methods
    //-------------------------------------------------------------------------------------------------

    //! Clears the list of output lines
    Q_INVOKABLE void clearOutput();

	//! Copies a string to clipboard
	Q_INVOKABLE void copy(const QString& sText);

    //! Opens a file for edit
    Q_INVOKABLE void openFile(const QString& sFullName);

    //!
    Q_INVOKABLE bool can(CEnums::ECapability eWhat);

    //! Checks the status of the repo at sPath
    Q_INVOKABLE void checkRepositoryStatus(QString sPath = "");

    //! Creates a list of CRepoFile from the repo at sPath
    Q_INVOKABLE void checkAllFileStatus(QString sPath = "");

    //! Creates a list of CRepoFile from the repo at sPath
    Q_INVOKABLE void checkChangedFileStatus(QString sPath = "");

    //! Creates a list of CRepoFile from the repo at sPath
    Q_INVOKABLE void checkFileStatus(const QString& sFileFullName);

    //!
    Q_INVOKABLE void refresh();

    //! Toggles the 'staged' flag of a file
    Q_INVOKABLE void toggleStaged(QString sFullName);

    //! Deletes a file
    Q_INVOKABLE void deleteFile(QString sFullName);

    //!
    Q_INVOKABLE void stageSelection(QStringList lFileFullNames);

    //!
    Q_INVOKABLE void unstageSelection(QStringList lFileFullNames);

    //! Stages all modified files
    Q_INVOKABLE void stageAll();

    //! Unstages all staged files
    Q_INVOKABLE void unstageAll();

    //! Does a revert on all selected files
    Q_INVOKABLE void revertSelection(QStringList lFileFullNames);

    //! Commits (or amends) all staged files
    Q_INVOKABLE void commit(const QString& sMessage, bool bAmend);

    //! Continues any ongoing rebase
    Q_INVOKABLE void continueRebase();

    //! Aborts any ongoing rebase
    Q_INVOKABLE void abortRebase();

    //! Pushes local commits to remote
    Q_INVOKABLE void push();

    //! Pulls remote commits
    Q_INVOKABLE void pull();

    //! Fetches changes from remote
    Q_INVOKABLE void fetch();

    //! Saves stash
    Q_INVOKABLE void stashSave();

    //! Pops stash
    Q_INVOKABLE void stashPop();

    //! Resets current branch to a commit
    Q_INVOKABLE void commitReset(const QString& sCommitId);

    //! Rebases current branch on a commit
    Q_INVOKABLE void commitRebase(const QString& sCommitId);

    //! Squashes a commit
    Q_INVOKABLE void commitSquash(const QString& sCommitId);

    //! Branches from a commit
    Q_INVOKABLE void commitBranchFrom(const QString& sCommitId, const QString& sBranchName);

    //! Changes the message of a commit
    Q_INVOKABLE void changeCommitMessage(const QString& sCommitId, const QString& sMessage);

    //! Merges a branch
    Q_INVOKABLE void mergeBranch(const QString& sName);

    //! Deletes a branch
    Q_INVOKABLE void deleteBranch(const QString& sName);

    //! Creates a tag
    Q_INVOKABLE void createTagOnCommit(const QString& sCommitId, const QString& sTagName, const QString& sMessage);

    //! View diff with previous commit
    Q_INVOKABLE void commitDiffPrevious(const QString& sCommitId);

    //! Sets the file filter
    Q_INVOKABLE void setFileFilter(const QString& sText);

    //! Sets the branch log filter
    Q_INVOKABLE void setBranchLogFilter(const QString& sText);

    //-------------------------------------------------------------------------------------------------
    // Static control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns the type of repository from a folder
    static CEnums::ERepositoryType getRepositoryTypeFromFolder(const QString& sPath);

    //! Returns the type of repository from an URL
    static CEnums::ERepositoryType getRepositoryTypeFromURL(const QString& sRepositoryURL);

    //! Returns a command interface for a given repository type
    static CCommands* getCommandsForRepositoryType(CEnums::ERepositoryType eType);

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    void getBranches(QString sPath = "");

    //!
    void getBranchHeadCommits(QString sPath = "");

    //!
    void getBranchesCommitCountAheadBehind(QString sPath = "");

    //!
    void getTags(QString sPath = "");

    //!
    void getTagCommits(QString sPath = "");

    //!
    void getGraph(QString sPath = "");

    //!
    void getBranchLog(QString sPath = "");

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void repositoryTypeStringChanged();

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //! Called when a file has been highlighted
    void onCurrentFileFullName(QString sFileFullName);

    //!
    void onNewOutput(QString sOutput, bool bSeparation);

    //!
    void onNewOutputString(CEnums::EProcessCommand eCommand, QString sValue);

    //!
    void onNewOutputKeyValue(CEnums::EProcessCommand eCommand, QString sKey, QString sValue);

    //!
    void onNewOutputStringList(CEnums::EProcessCommand eCommand, QStringList lValue);

    //!
    void onNewOutputListOfCBranch(CEnums::EProcessCommand eCommand, QList<CBranch*> lNewBranches);

    //!
    void onNewOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles);

    //!
    void onNewOutputCLogLineCollection(CEnums::EProcessCommand eCommand, CLogLineCollection lNewLines);

    //!
    void onNewOutputListOfCDiffLine(CEnums::EProcessCommand eCommand, QList<CDiffLine*> lNewLines);

    //!
    void onNewOutputListOfCGraphLine(CEnums::EProcessCommand eCommand, QList<CGraphLine*> lNewLines);

    //!
    void onDiffCommitIdChanged();

    //!
    void onShouldRefreshFileStatus();

    //!
    void onRequestBranchLogData(int iStartIndex, int iCount);

    //!
    void onRequestFileLogData(int iStartIndex, int iCount);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    QString m_sCurrentFileFullName;
};
