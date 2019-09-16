
#pragma once

// Qt
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

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
    CuteGit(bool bMasterMode = true, const QString& sSequenceFileName = "");

    //! Destructor
    virtual ~CuteGit();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    void setLanguage(const QString& sLang);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    //! Controller
    CController* m_pController;

    //! View
    QQmlApplicationEngine* m_pEngine;

    //!
    QTranslator* m_pTranslator;
};
