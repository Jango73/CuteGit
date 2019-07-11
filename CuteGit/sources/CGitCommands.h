
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
};
