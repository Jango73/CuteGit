
#pragma once

// Qt
#include <QStringListModel>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

// Application
#include "CEnums.h"
#include "CBranchModel.h"
#include "CTreeFileModel.h"
#include "CTreeFileModelProxy.h"
#include "CFlatFileModel.h"
#include "CFlatFileModelProxy.h"
#include "CRepoFile.h"
#include "CLogModel.h"
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

    Q_FAST_PROPERTY(CEnums::ERepositoryType, e, repositoryType, RepositoryType)
    Q_FAST_PROPERTY(CEnums::ERepositoryStatus, e, repositoryStatus, RepositoryStatus)
    Q_FAST_PROPERTY(QString, s, repositoryPath, RepositoryPath)
    Q_FAST_PROPERTY(CCommands*, p, commands, Commands)
    Q_FAST_PROPERTY(CTreeFileModel*, p, treeFileModel, TreeFileModel)
    Q_FAST_PROPERTY(CTreeFileModelProxy*, p, treeFileModelProxy, TreeFileModelProxy)
    Q_FAST_PROPERTY(CFlatFileModel*, p, flatFileModel, FlatFileModel)
    Q_FAST_PROPERTY(CFlatFileModelProxy*, p, flatFileModelProxy, FlatFileModelProxy)
    Q_FAST_PROPERTY(CBranchModel*, p, branchModel, BranchModel)
    Q_FAST_PROPERTY(CBranchModel*, p, tagModel, TagModel)
    Q_FAST_PROPERTY(CLogModel*, p, logModel, LogModel)
    Q_FAST_PROPERTY(CDiffModel*, p, fileDiffModel, FileDiffModel)
    Q_FAST_PROPERTY(CLogModel*, p, fileLogModel, FileLogModel)
    Q_FAST_PROPERTY(CGraphModel*, p, graphModel, GraphModel)
    Q_FAST_PROPERTY_NO_SET_IMPL(QString, s, currentBranch, CurrentBranch)
    Q_FAST_PROPERTY(QList<CRepoFile*>, l, repoFiles, RepoFiles)

    Q_FAST_PROPERTY(bool, b, hasCommitableFiles, HasCommitableFiles)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CRepository(const QString& sPath, CController* pController, QObject *parent = nullptr);

    //! Destructor
    virtual ~CRepository() override;

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    CRepoFile* fileByFullName(const QList<CRepoFile*>& vFiles, const QString& sFullName) const;

    //!
    QStringList labelsForCommit(const QString& sCommitId) const;

    //-------------------------------------------------------------------------------------------------
    // Invokable control methods
    //-------------------------------------------------------------------------------------------------

    //!
    Q_INVOKABLE bool can(CEnums::ECapability eWhat);

    //!
    Q_INVOKABLE void checkAllFileStatus(QString sPath = "");

    //!
    Q_INVOKABLE void checkRepositoryStatus(QString sPath = "");

    //!
    Q_INVOKABLE void refresh();

    //!
    Q_INVOKABLE void toggleStaged(QString sFullName);

    //!
    Q_INVOKABLE void stageSelection(QStringList lFileFullNames);

    //!
    Q_INVOKABLE void unstageSelection(QStringList lFileFullNames);

    //!
    Q_INVOKABLE void stageAll();

    //!
    Q_INVOKABLE void unstageAll();

    //!
    Q_INVOKABLE void revertSelection(QStringList lFileFullNames);

    //!
    Q_INVOKABLE void commit(const QString& sMessage, bool bAmend);

    //!
    Q_INVOKABLE void continueRebase();

    //!
    Q_INVOKABLE void abortRebase();

    //!
    Q_INVOKABLE void push();

    //!
    Q_INVOKABLE void pull();

    //!
    Q_INVOKABLE void fetch();

    //!
    Q_INVOKABLE void stashSave();

    //!
    Q_INVOKABLE void stashPop();

    //!
    Q_INVOKABLE void commitReset(const QString& sCommitId);

    //!
    Q_INVOKABLE void commitRebase(const QString& sCommitId);

    //!
    Q_INVOKABLE void commitSquash(const QString& sCommitId);

    //!
    Q_INVOKABLE void commitBranchFrom(const QString& sCommitId, const QString& sBranchName);

    //!
    Q_INVOKABLE void changeCommitMessage(const QString& sCommitId, const QString& sMessage);

    //!
    Q_INVOKABLE void deleteBranch(const QString& sName);

    //-------------------------------------------------------------------------------------------------
    // Static control methods
    //-------------------------------------------------------------------------------------------------

    //!
    static CEnums::ERepositoryType getRepositoryTypeFromFolder(const QString& sPath);

    //! Returns the type of repository using URL
    static CEnums::ERepositoryType getRepositoryTypeFromURL(const QString& sRepositoryURL);

    //!
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
    void getTags(QString sPath = "");

    //!
    void getTagCommits(QString sPath = "");

    //!
    void getGraph(QString sPath = "");

    //!
    void getLog(QString sPath = "");

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void currentFileFullName(QString sFileFullName);

    //!
    void newOutput(QString sOutput);

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //!
    void onCurrentFileFullName(QString sFileFullName);

    //!
    void onNewOutput(QString sOutput);

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
    void onNewOutputListOfCLogLine(CEnums::EProcessCommand eCommand, QList<CLogLine*> lNewLines);

    //!
    void onNewOutputListOfCDiffLine(CEnums::EProcessCommand eCommand, QList<CDiffLine*> lNewLines);

    //!
    void onNewOutputListOfCGraphLine(CEnums::EProcessCommand eCommand, QList<CGraphLine*> lNewLines);
};
