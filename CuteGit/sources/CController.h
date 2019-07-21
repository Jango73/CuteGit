
#pragma once

// Qt
#include <QObject>
#include <QTimer>
#include <QFileInfo>
#include <QStringListModel>
#include <QSharedMemory>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

// Application
#include "CFileModel.h"
#include "CFileModelProxy.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------

#define CONFIG_FILE_NAME        "configuration.xml"
#define SHARED_MEMORY_MAX       65536
#define SHARED_MEMORY_TEXT_MAX  60000

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

    enum ESharedOperation
    {
        eSONone = 0,
        eSOSlaveRequestEdit,
        eSOMasterFinishedEdit
    };

    typedef struct
    {
        qint32              iMagic;
        ESharedOperation    eOperation;
        char                sText[SHARED_MEMORY_TEXT_MAX];
    } SMemoryStruct;

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CController(QObject *parent = nullptr);

    //! Slave constructor
    CController(bool bDummy, QObject *parent = nullptr);

    //! Destructor
    virtual ~CController();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Sets current repository path
    void setRepositoryPath(QString sPath);

    //!
    void setSharedOperation(ESharedOperation iOperation);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Returns current repository path
    QString repositoryPath() const;

    //!
    ESharedOperation sharedOperation();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Saves configuration
    void saveConfiguration();

    //! Loads configuration
    void loadConfiguration();

    //!
    void clearSharedMemory();

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

    void onSharedTimerTick();

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    bool                    m_bMasterMode;
    QSharedMemory           m_tShared;
    QTimer                  m_tSharedTimer;

    static const QString    m_sSharedKey;
};
