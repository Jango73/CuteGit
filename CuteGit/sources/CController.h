
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

    Q_PROPERTY(QString repositoryPath READ repositoryPath WRITE setRepositoryPath NOTIFY repositoryPathChanged)

    Q_FAST_PROPERTY(CCommands*, p, commands, Commands)
    Q_FAST_PROPERTY(CFileModel*, p, fileModel, FileModel)
    Q_FAST_PROPERTY(CFileModelProxy*, p, fileModelProxy, FileModelProxy)
    Q_FAST_PROPERTY(QStringListModel*, p, repositoryModel, RepositoryModel)
    Q_FAST_PROPERTY(QStringListModel*, p, commandOutputModel, CommandOutputModel)

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

    //! Sets current repository path
    void setRepositoryPath(QString sPath);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Returns current repository path
    QString repositoryPath() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Saves configuration
    void saveConfiguration();

    //! Loads configuration
    void loadConfiguration();

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

    //!
    Q_INVOKABLE void quit();

    //!
    Q_INVOKABLE void clearOutput();

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void repositoryPathChanged();

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    void onNewOutput(QString sOutput);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:
};
