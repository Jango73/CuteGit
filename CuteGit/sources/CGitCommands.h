
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
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CGitCommands();

    //! Destructor
    virtual ~CGitCommands() override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void getAllFileStatus(const QString& sPath) override;

    //!
    virtual void getBranches(const QString& sPath) override;

    //!
    virtual void getGraph(const QString& sPath, const QDateTime& from, const QDateTime& to) override;

    //!
    virtual void fileLog(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void stageFile(const QString& sPath, const QString& sFullName, bool bStage) override;

    //!
    virtual void stageAll(const QString& sPath, bool bStage) override;

    //!
    virtual void revertFile(const QString& sPath, const QString& sFullName) override;

    //!
    virtual void commit(const QString& sPath, const QString& sMessage) override;

    //!
    virtual void push(const QString& sPath) override;

    //!
    virtual void pull(const QString& sPath) override;

    //!
    virtual void unstagedFileDiff(const QString& sPath, const QString& sFullName) override;

protected slots:

    //!
    void onExecFinished(QString sPath, CProcessCommand::EProcessCommand eCommand, QString sValue);
};
