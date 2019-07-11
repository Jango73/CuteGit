
#pragma once

// Qt
#include <QObject>

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------

class CCommands
{
public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CCommands();

    //! Destructor
    virtual ~CCommands();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    QString exec(const QString& sWorkPath, const QString& sCommand);

    //!
    virtual QVector<CRepoFile*> getAllFileStatus(const QString& sPath);
};
