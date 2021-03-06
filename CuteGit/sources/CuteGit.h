
#pragma once

// Qt
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// Application
#include "CController.h"

//-------------------------------------------------------------------------------------------------

class CuteGit : public QObject
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CuteGit(bool bMasterMode = true, const QString& sStubFileName = "", const QString& sSequenceFileName = "");

    //! Destructor
    virtual ~CuteGit();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    //! Controller
    CController* m_pController;

    //! View
    QQmlApplicationEngine* m_pEngine;
};
