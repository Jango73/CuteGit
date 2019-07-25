
#pragma once

// Qt
#include <QStringListModel>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

// Application
#include "CTreeFileModel.h"
#include "CTreeFileModelProxy.h"
#include "CFlatFileModel.h"
#include "CRepoFile.h"
#include "CLogModel.h"
#include "CDiffModel.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CRepository : public QObject
{
    Q_OBJECT

public:

    enum ERepositoryStatus
    {
        NoMerge,
        Merge,
        Rebase,
        InteractiveRebase
    };

    Q_ENUMS(ERepositoryStatus)

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_PROPERTY(QString repositoryPath READ repositoryPath WRITE setRepositoryPath NOTIFY repositoryPathChanged)

    Q_FAST_PROPERTY(ERepositoryStatus, e, repositoryStatus, RepositoryStatus)
    Q_FAST_PROPERTY(CController*, p, controller, Controller)
    Q_FAST_PROPERTY(CTreeFileModel*, p, treeFileModel, TreeFileModel)
    Q_FAST_PROPERTY(CTreeFileModelProxy*, p, treeFileModelProxy, TreeFileModelProxy)
    Q_FAST_PROPERTY(CFlatFileModel*, p, flatFileModel, FlatFileModel)
    Q_FAST_PROPERTY(QStringListModel*, p, branchModel, BranchModel)
    Q_FAST_PROPERTY(CLogModel*, p, logModel, LogModel)
    Q_FAST_PROPERTY(CDiffModel*, p, diffModel, DiffModel)
    Q_FAST_PROPERTY(CLogModel*, p, fileLogModel, FileLogModel)
    Q_FAST_PROPERTY_NO_SET_IMPL(QString, s, currentBranch, CurrentBranch)
    Q_FAST_PROPERTY(QList<CRepoFile*>, l, repoFiles, RepoFiles)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CRepository(CController* pController, QObject *parent = nullptr);

    //! Destructor
    virtual ~CRepository() override;

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Sets current repository path
    void setRepositoryPath(QString sPath);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Returns current repository path
    QString repositoryPath() const;

    //!
    CRepoFile* fileByFullName(const QList<CRepoFile*>& vFiles, const QString& sFullName) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

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
    Q_INVOKABLE void commitReset(const QString& sCommitId);

    //!
    Q_INVOKABLE void commitRebase(const QString& sCommitId);

    //!
    Q_INVOKABLE void changeCommitMessage(const QString& sCommitId, const QString& sMessage);

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    void getBranches(QString sPath = "");

    //!
    void getLog(QString sPath = "");

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void repositoryPathChanged();

    //!
    void currentFileFullName(QString sFileFullName);

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //!
    void onCurrentFileFullName(QString sFileFullName);

    //!
    void onNewOutput(QString sOutput);

    //!
    void onNewOutputString(CProcessCommand::EProcessCommand eCommand, QString sValue);

    //!
    void onNewOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue);

    //!
    void onNewOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles);

    //!
    void onNewOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> lNewLines);

    //!
    void onNewOutputListOfCDiffLine(CProcessCommand::EProcessCommand eCommand, QList<CDiffLine*> lNewLines);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    QString     m_sRepositoryPath;
};
