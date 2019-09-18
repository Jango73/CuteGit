
#pragma once

// Qt
#include <QObject>

// Application
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

class CSvnCommands : public CCommands
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CSvnCommands(CController* pController);

    //! Destructor
    virtual ~CSvnCommands() override;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    virtual bool can(CEnums::ECapability eWhat) const override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void repositoryStatus(const QString& sPath) override;

    //!
    virtual void allFileStatus(const QString& sPath) override;

    //!
    virtual void changedFileStatus(const QString& sPath) override;

    //!
    virtual void branches(const QString& sPath) override;

    //!
    virtual void graph(const QString& sPath) override;

    //!
    virtual void branchLog(const QString& sPath, int iFrom = 0, int iCount = LOG_COUNT_DEFAULT) override;

    //!
    virtual void fileLog(const QString& sPath, const QString& sFullName, int iFrom = 0, int iCount = LOG_COUNT_DEFAULT) override;

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
    virtual void pull(const QString& sPath) override;

    //!
    virtual void unstagedFileDiff(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void setCurrentBranch(const QString& sPath, const QString& sBranch) override;

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
};
