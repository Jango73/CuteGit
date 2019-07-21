
#pragma once

// Qt
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Application
#include "CController.h"

//-------------------------------------------------------------------------------------------------

class CuteGit
{
public:

    //! Constructor
    CuteGit(bool bMasterMode = true);

    //! Destructor
    virtual ~CuteGit();

private:

    //! Controller
    CController* m_pController;

    //! View
    QQmlApplicationEngine* m_pEngine;
};
