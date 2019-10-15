
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
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CGerritCommands(CController* pController);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    virtual bool can(CEnums::ECapability eWhat) const override;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    virtual void push(const QString& sPath) override;

    //!
    virtual void pushAsWIP(const QString& sPath) override;

    //-------------------------------------------------------------------------------------------------
    // Protected constants
    //-------------------------------------------------------------------------------------------------

protected:

    // Command strings

    static const QString sCommandPushGerritReady;
    static const QString sCommandPushGerritWIP;
};
