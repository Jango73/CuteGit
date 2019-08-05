
#pragma once

// Qt
#include <QObject>

// Application
#include "CGitCommands.h"

//-------------------------------------------------------------------------------------------------

class CGerritCommands : public CGitCommands
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void push(const QString& sPath) override;

    //-------------------------------------------------------------------------------------------------
    // Protected constants
    //-------------------------------------------------------------------------------------------------

protected:

    // Command strings

    static const QString sCommandPushGerrit;
};
