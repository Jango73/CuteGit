
#pragma once

// Qt
#include <QObject>

// Application
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

class CGitCommands : public CCommands
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Enumerators
    //-------------------------------------------------------------------------------------------------

    enum ERebaseType
    {
        eRTReword,
        eRTEdit,
        eRTSquash
    };

    enum ERebaseStep
    {
        eRSChangeCommitEditSequence,
        eRSChangeCommitEditMessage,
        eRSSquashCommitEditSequence,
        eRSSquashCommitEditMessage
    };

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CGitCommands();

    //! Destructor
    virtual ~CGitCommands() override;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    virtual bool can(CEnums::ECapability eWhat) const override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void cloneRepository(const QString& sRepositoryURL, const QString& sRepositoryPath) override;

    //!
    virtual void repositoryStatus(const QString& sPath) override;

    //!
    virtual void allFileStatus(const QString& sPath) override;

    //!
    virtual void fileStatus(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void branches(const QString& sPath) override;

    //!
    virtual void branchHeadCommits(const QString& sPath, QStringList lBranches) override;

    //!
    virtual void tags(const QString& sPath) override;

    //!
    virtual void tagCommits(const QString& sPath, QStringList lTags) override;

    //!
    virtual void graph(const QString& sPath) override;

    //!
    virtual void branchLog(const QString& sPath, const QDateTime& from, const QDateTime& to) override;

    //!
    virtual void fileLog(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void toggleStaged(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void stageFile(const QString& sPath, const QString& sFullName, bool bStage) override;

    //!
    virtual void stageAll(const QString& sPath, bool bStage) override;

    //!
    virtual void revertFile(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void commit(const QString& sPath, const QString& sMessage) override;

    //!
    virtual void amend(const QString& sPath) override;

    //!
    virtual void push(const QString& sPath) override;

    //!
    virtual void pull(const QString& sPath) override;

    //!
    virtual void fetch(const QString& sPath) override;

    //!
    virtual void stashSave(const QString& sPath) override;

    //!
    virtual void stashPop(const QString& sPath) override;

    //!
    virtual void unstagedFileDiff(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void setCurrentBranch(const QString& sPath, const QString& sBranch) override;

    //!
    virtual void commitReset(const QString& sPath, const QString& sCommitId) override;

    //!
    virtual void commitRebase(const QString& sPath, const QString& sCommitId) override;

    //!
    virtual void commitSquash(const QString& sPath, const QString& sCommitId) override;

    //!
    virtual void commitBranchFrom(const QString& sPath, const QString& sCommitId, const QString& sBranchName) override;

    //!
    virtual void changeCommitMessage(const QString& sPath, const QString& sCommitId, const QString& sMessage) override;

    //!
    virtual void continueRebase(const QString& sPath) override;

    //!
    virtual void abortRebase(const QString& sPath) override;

    //!
    virtual void deleteBranch(const QString& sPath, const QString& sBranchName) override;

    //!
    virtual void editSequenceFile(const QString& sFileName) override;

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    CRepoFile* repoFileForLine(const QString& sPath, QString sLine);

    //-------------------------------------------------------------------------------------------------
    // Protected slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //!
    void onExecFinished(QString sPath, CEnums::EProcessCommand eCommand, QString sValue, QString sUserData);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    ERebaseType     m_eRebaseType;
    ERebaseStep     m_eRebaseStep;
    QString         m_sCommitId;
    QString         m_sCommitMessage;
};
