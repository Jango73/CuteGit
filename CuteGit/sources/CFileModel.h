
#pragma once

// Qt
#include <QFileSystemModel>

// qt-plus
#include "CXMLNode.h"

//-------------------------------------------------------------------------------------------------

class CFileModel : public QFileSystemModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex rootPathIndex READ rootPathIndex NOTIFY rootPathIndexChanged)

public:
    enum ERoles
    {
        eSizeRole = Qt::UserRole + 10,
        eStateRole
    };

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    //! Default constructor
    CFileModel(QObject *parent = nullptr);

    //! Destructor
    virtual ~CFileModel() Q_DECL_OVERRIDE;

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    QModelIndex rootPathIndex() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

    //!
    void stageSelection();

    //!
    void unstageSelection();

    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

signals:

    void rootPathIndexChanged();
};
