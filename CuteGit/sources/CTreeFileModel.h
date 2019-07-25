
#pragma once

// Qt
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QStringListModel>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

// Application
#include "CRepoFile.h"
#include "CLogModel.h"
#include "CDiffModel.h"
#include "CCommands.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CTreeFileModel : public QFileSystemModel
{
    Q_OBJECT

public:
    enum ERoles
    {
        eSizeRole = Qt::UserRole + 10,
        eStatusRole,
        eStagedRole
    };

    enum ERepositoryStatus
    {
        NoMerge,
        Merge,
        Rebase,
        InteractiveRebase
    };

    Q_ENUMS(ERepositoryStatus)

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_PROPERTY(QModelIndex rootPathIndex READ rootPathIndex NOTIFY rootPathIndexChanged)

    Q_FAST_PROPERTY(CController*, p, controller, Controller)
    Q_FAST_PROPERTY(QFileSystemWatcher*, p, fileSystemWatcher, FileSystemWatcher)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CTreeFileModel(CController* pController, QObject *parent = nullptr);

    //! Destructor
    virtual ~CTreeFileModel() override;

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QModelIndex rootPathIndex() const;

    //!
    CRepoFile* fileByFullName(const QList<CRepoFile*>& vFiles, const QString& sFullName) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const override;

    //! Returns data
    virtual QVariant data(const QModelIndex& qIndex, int iRole) const override;

    //!
    void handleRepoFilesChanged();

    //!
    void handleCurrentIndex(QModelIndex qIndex);

    //!
    QStringList selectionToFullNameList(QModelIndexList lIndices);

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    //!
    void rootPathIndexChanged();

    //!
    void currentFileFullName(QString sFileFullName);

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    //!
    void onRootPathChanged(const QString& sNewPath);

    //!
    void onFileChanged(const QString& path);
};
