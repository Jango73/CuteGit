
// Application
#include "CFileModel.h"

//-------------------------------------------------------------------------------------------------

CFileModel::CFileModel(QObject* parent)
    : QFileSystemModel(parent)
{
    setRootPath(QDir::homePath());
    setResolveSymlinks(true);
}

//-------------------------------------------------------------------------------------------------

CFileModel::~CFileModel()
{
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames = QFileSystemModel::roleNames();
    hRoleNames[eSizeRole] = "size";
    hRoleNames[eStateRole] = "state";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

QVariant CFileModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    if ((row < 0) || (row > (rowCount() - 1)))
        return QVariant();

    switch (role)
    {
    case eSizeRole:
        return 0;

    case eStateRole:
        return 0;
    }

    return QVariant();
}
