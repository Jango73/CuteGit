
#pragma once

// Qt
#include <QObject>
#include <QDateTime>
#include <QMap>
#include <QThread>
#include <QMutex>

// Application
#include "CRepoFile.h"
#include "CLogLine.h"

//-------------------------------------------------------------------------------------------------

class CProcessCommand : public QObject
{
    Q_OBJECT

public:

    enum EProcessCommand
    {
        eNotification,
        eAllFileStatus,
        eRepositoryStatus,
        eBranches,
        eBranchLog,
        eCurrentBranch,
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
        eCommitRebase,
        eChangeCommitMessage,
        eContinueRebase
    };

    Q_ENUMS(EProcessCommand)

    CProcessCommand(EProcessCommand eCommand, QString sWorkPath, QString sCommand, QMap<QString, QString> mEnvironment = QMap<QString, QString>())
        : m_eCommand(eCommand)
        , m_sWorkPath(sWorkPath)
        , m_sCommand(sCommand)
        , m_mEnvironment(mEnvironment)
    {
    }

    EProcessCommand         m_eCommand;
    QString                 m_sWorkPath;
    QString                 m_sCommand;
    QMap<QString, QString>  m_mEnvironment;
};

//-------------------------------------------------------------------------------------------------

class CCommands : public QThread
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CCommands();

    //! Destructor
    virtual ~CCommands() override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void allFileStatus(const QString& sPath);

    //!
    virtual void repositoryStatus(const QString& sPath);

    //!
    virtual void branches(const QString& sPath);

    //!
    virtual void branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to);

    //!
    virtual void fileLog(const QString& sPath, const QString& sFullName);

    //!
    virtual void stageFile(const QString& sPath, const QString& sFullName, bool bStage);

    //!
    virtual void stageAll(const QString& sPath, bool bStage);

    //!
    virtual void revertFile(const QString& sPath, const QString& sFullName);

    //!
    virtual void commit(const QString& sPath, const QString& sMessage);

    //!
    virtual void amend(const QString& sPath);

    //!
    virtual void push(const QString& sPath);

    //!
    virtual void pull(const QString& sPath);

    //!
    virtual void unstagedFileDiff(const QString& sPath, const QString& sFullName);

    //!
    virtual void setCurrentBranch(const QString& sPath, const QString& sBranch);

    //!
    virtual void commitRebase(const QString& sPath, const QString& sCommitId);

    //!
    virtual void changeCommitMessage(const QString& sPath, const QString& sCommitId, const QString& sMessage);

    //!
    virtual void continueRebase(const QString& sPath);

    //!
    virtual void editSequenceFile(const QString& sFileName);

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    void exec(CProcessCommand* pCommand);

    //!
    QString execNow(QString m_sWorkPath, QString m_sCommand, QMap<QString, QString> m_mEnvironment = QMap<QString, QString>());

    //-------------------------------------------------------------------------------------------------
    // Private control methods
    //-------------------------------------------------------------------------------------------------

private:

    //!
    virtual void run() override;

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void execFinished(QString sPath, CProcessCommand::EProcessCommand eCommand, QString sValue);

    //!
    void newOutputString(CProcessCommand::EProcessCommand eCommand, QString sValue);

    //!
    void newOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue);

    //!
    void newOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> vNewRepoFiles);

    //!
    void newOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> vNewGraphLines);

private:

    bool                    m_bStop;
    QMutex                  m_mMutex;
    QList<CProcessCommand*> m_lCommandStack;
};
