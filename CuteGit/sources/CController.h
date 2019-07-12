
#pragma once

// Qt
#include <QObject>
#include <QTimer>
#include <QFileInfo>
#include <QStringListModel>

// qt-plus
#include <Macros.h>
#include <CXMLNode.h>

// Application
#include "CFileModel.h"
#include "CFileModelProxy.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

#define CONFIG_FILE_NAME    "configuration.xml"

//-------------------------------------------------------------------------------------------------

class CController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repositoryPath READ repositoryPath NOTIFY repositoryPathChanged)

    Q_FAST_PROPERTY(CCommands*, p, commands, Commands)
    Q_FAST_PROPERTY(CFileModel*, p, fileModel, FileModel)
    Q_FAST_PROPERTY(CFileModelProxy*, p, fileModelProxy, FileModelProxy)
    Q_FAST_PROPERTY(QStringListModel*, p, repositoryModel, RepositoryModel)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CController(QObject *parent = nullptr);

    //! Destructor
    virtual ~CController();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QString repositoryPath() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    void saveConfiguration();

    //!
    void loadConfiguration();

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

    //!
    Q_INVOKABLE void setRepository(QString sPath);

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void repositoryPathChanged();

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

public slots:

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:
};
