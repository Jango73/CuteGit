
#pragma once

// Qt
#include <QFileSystemModel>

// qt-plus
#include "CXMLNode.h"

//-------------------------------------------------------------------------------------------------

class CFileModel : public QFileSystemModel
{
    Q_OBJECT

public:
    enum ERoles
    {
        eSizeRole = Qt::UserRole + 1,
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
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Returns role names
    virtual QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    //! Returns data
    virtual QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
};
