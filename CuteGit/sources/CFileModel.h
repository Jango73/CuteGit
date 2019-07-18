
#pragma once

// Qt
#include <QFileSystemModel>
#include <QStringListModel>

// qt-plus
#include <Macros.h>
#include "CXMLNode.h"

// Application
#include "CRepoFile.h"

//-------------------------------------------------------------------------------------------------
// Forward declarations

class CController;

//-------------------------------------------------------------------------------------------------

class CFileModel : public QFileSystemModel
{
    Q_OBJECT

public:
    enum ERoles
    {
        eSizeRole = Qt::UserRole + 10,
        eStatusRole,
        eStagedRole
    };

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_PROPERTY(QModelIndex rootPathIndex READ rootPathIndex NOTIFY rootPathIndexChanged)

    Q_FAST_PROPERTY(CController*, p, controller, Controller)
    Q_FAST_PROPERTY(QStringListModel*, p, graphModel, GraphModel)
    Q_FAST_PROPERTY(QStringListModel*, p, diffModel, DiffModel)

//    Q_FAST_PROPERTY_NO_SET_IMPL(QString, s, rootPath, RootPath)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CFileModel(CController* pController, QObject *parent = nullptr);

    //! Destructor
    virtual ~CFileModel() Q_DECL_OVERRIDE;

    //-------------------------------------------------------------------------------------------------
    // Setters
    //-------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //!
    QModelIndex rootPathIndex() const;

    //!
    CRepoFile* fileByFullName(const QVector<CRepoFile*>& vFiles, const QString& sFullName) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

    //!
    void checkAllFileStatus(QString sPath = "");

    //!
    void handleCurrentIndex(QModelIndex qIndex);

    //!
    void refresh();

    //!
    void stageSelection(QModelIndexList lIndices);

    //!
    void unstageSelection(QModelIndexList lIndices);

    //!
    void stageAll();

    //!
    void revertSelection(QModelIndexList lIndices);

    //!
    void commit(const QString& sMessage);

    //!
    void push();

    //!
    void pull();

    //-------------------------------------------------------------------------------------------------
    // Protected control methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    void getGraph(QString sPath = "");

    //!

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void rootPathIndexChanged();
    void newOutput(QString sText);

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

protected slots:

    void onRootPathChanged(const QString& sNewPath);

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    QVector<CRepoFile*>     m_vRepoFiles;
};
