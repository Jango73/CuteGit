
#pragma once

// Qt
#include <QObject>
#include <QDateTime>
#include <QMap>
#include <QThread>
#include <QMutex>

// Application
#include "CEnums.h"
#include "CBranch.h"
#include "CRepoFile.h"
#include "CLogLine.h"
#include "CDiffLine.h"
#include "CGraphLine.h"

//-------------------------------------------------------------------------------------------------

class CProcessCommand : public QObject
{
    Q_OBJECT

public:

    CProcessCommand(CEnums::EProcessCommand eCommand, QString sWorkPath, QString sCommand, bool bAllowStack = false, QMap<QString, QString> mEnvironment = QMap<QString, QString>(), QString sUserData = "")
        : m_eCommand(eCommand)
        , m_bAllowStack(bAllowStack)
        , m_sWorkPath(sWorkPath)
        , m_sCommand(sCommand)
        , m_mEnvironment(mEnvironment)
        , m_sUserData(sUserData)
    {
    }

    CEnums::EProcessCommand m_eCommand;
    bool                    m_bAllowStack;
    QString                 m_sWorkPath;
    QString                 m_sCommand;
    QMap<QString, QString>  m_mEnvironment;
    QString                 m_sUserData;
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
    // Getters
    //-------------------------------------------------------------------------------------------------

    virtual bool can(CEnums::ECapability eWhat) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Creates a list of CRepoFile from the repo at sPath
    virtual void allFileStatus(const QString& sPath);

    //! Checks the status of the repo at sPath
    virtual void repositoryStatus(const QString& sPath);

    //! Creates a list of branches of the repo at sPath
    virtual void branches(const QString& sPath);

    //! Creates a list of head commits for branches of the repo at sPath
    virtual void branchHeadCommits(const QString& sPath, QStringList lBranches);

    //! Creates a list of CGraphLine from the repo at sPath
    virtual void graph(const QString& sPath);

    //! Creates a list of CLogLine from the repo at sPath, for the current branch
    virtual void branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to);

    //! Creates a list of CLogLine from the repo at sPath, for sFullName
    virtual void fileLog(const QString& sPath, const QString& sFullName);

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
    virtual void unstagedFileDiff(const QString& sPath, const QString& sFullName);

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
    void execFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue, QString sUserData);

    //!
    void newOutputString(CEnums::EProcessCommand eCommand, QString sValue);

    //!
    void newOutputKeyValue(CEnums::EProcessCommand eCommand, QString sKey, QString sValue);

    //!
    void newOutputStringList(CEnums::EProcessCommand eCommand, QStringList lValue);

    //!
    void newOutputListOfCBranch(CEnums::EProcessCommand eCommand, QList<CBranch*> lNewBranches);

    //!
    void newOutputListOfCRepoFile(CEnums::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles);

    //!
    void newOutputListOfCLogLine(CEnums::EProcessCommand eCommand, QList<CLogLine*> lNewLines);

    //!
    void newOutputListOfCDiffLine(CEnums::EProcessCommand eCommand, QList<CDiffLine*> lNewLines);

    //!
    void newOutputListOfCGraphLine(CEnums::EProcessCommand eCommand, QList<CGraphLine*> lNewLines);

private:

    bool                    m_bStop;
    QMutex                  m_mMutex;
    QList<CProcessCommand*> m_lCommandStack;
};
