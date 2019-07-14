
#pragma once

// Qt
#include <QObject>

// Application
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

class CGitCommands : public CCommands
{
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
    virtual QVector<CRepoFile*> getAllFileStatus(const QString& sPath) override;

    //!
    virtual QStringList getGraph(const QString& sPath, const QDateTime& from, const QDateTime& to) override;

    //!
    virtual QString stageFile(const QString& sPath, const QString& sFullName, bool bStage) override;

    //!
    virtual QString commit(const QString& sPath, const QString& sMessage) override;
};
