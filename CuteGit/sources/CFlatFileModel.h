
#pragma once

// Qt
#include <QAbstractListModel>

// Application
#include "CRepoFile.h"
#include "CLogLine.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CFlatFileModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ERoles
    {
        eFileNameRole = Qt::UserRole + 1,
        eSizeRole,
        eStatusRole,
        eStagedRole
    };

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(CController*, p, controller, Controller)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CFlatFileModel(CController* pController, QObject *parent = nullptr);

    //! Destructor
    virtual ~CFlatFileModel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Sets all lines
    void setRepoFiles(QList<CRepoFile*> lNewFiles);

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const;

    //! Returns row count
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const;

    //! Model empty?
    bool isEmpty() const;

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //!
    void onNewOutputString(CProcessCommand::EProcessCommand eCommand, QString sValue);

    //!
    void onNewOutputStringList(CProcessCommand::EProcessCommand eCommand, QStringList lValue);

    //!
    void onNewOutputListOfCRepoFile(CProcessCommand::EProcessCommand eCommand, QList<CRepoFile*> lNewRepoFiles);

    //!
    void onNewOutputListOfCLogLine(CProcessCommand::EProcessCommand eCommand, QList<CLogLine*> lNewGraphLines);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    QList<CRepoFile*>   m_vRepoFiles;
};
