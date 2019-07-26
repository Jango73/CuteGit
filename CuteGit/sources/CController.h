
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
#include "CRepository.h"

//-------------------------------------------------------------------------------------------------

#define CONFIG_FILE_NAME            "configuration.xml"
#define SHARED_MEMORY_MAX           65536
#define SHARED_MEMORY_TEXT_MAX      60000
#define SHARED_MEMORY_FILE_NAME_MAX 2000

//-------------------------------------------------------------------------------------------------

class CController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repositoryPath READ repositoryPath WRITE setRepositoryPath NOTIFY repositoryPathChanged)

    Q_FAST_PROPERTY(QStringListModel*, p, repositoryModel, RepositoryModel)
    Q_FAST_PROPERTY(QStringListModel*, p, commandOutputModel, CommandOutputModel)
    Q_FAST_PROPERTY(CRepository*, p, repository, Repository)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showClean, ShowClean)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showAdded, ShowAdded)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showModified, ShowModified)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showDeleted, ShowDeleted)
    Q_FAST_PROPERTY_NO_SET_IMPL(bool, b, showUntracked, ShowUntracked)

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
        char                sSequenceFileName[SHARED_MEMORY_FILE_NAME_MAX];
        char                sText[SHARED_MEMORY_TEXT_MAX];
    } SMemoryStruct;

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Master constructor
    CController(QObject* parent = nullptr);

    //! Slave constructor
    CController(QString sSequenceFileName, QObject* parent = nullptr);

    //! Destructor
    virtual ~CController();

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //! Sets current repository path
    void setRepositoryPath(QString sPath);

    //!
    void setSharedOperation(ESharedOperation iOperation);

    //!
    void setSequenceFileName(const QString& sSequenceFileName);

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QString repositoryPath() const;

    //!
    ESharedOperation sharedOperation();

    //!
    QString sequenceFileName();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Saves configuration
    void saveConfiguration();

    //! Loads configuration
    void loadConfiguration();

    //! Clears the shared memory data
    void clearSharedMemory();

    //-------------------------------------------------------------------------------------------------
    // Invokables
    //-------------------------------------------------------------------------------------------------

    //! Quits the application
    Q_INVOKABLE void quit();

    //! Clears the list of output lines
    Q_INVOKABLE void clearOutput();

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void repositoryPathChanged();

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //! Triggered for shared memory operation sync
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
