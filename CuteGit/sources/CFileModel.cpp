
// Application
#include "CFileModel.h"

//-------------------------------------------------------------------------------------------------

static inline QString sizeString(const QFileInfo &fi)
{
    if (!fi.isFile())
        return QString();
    const qint64 size = fi.size();
    if (size > 1024 * 1024 * 10)
        return QString::number(size / (1024 * 1024)) + QLatin1Char('M');
    if (size > 1024 * 10)
        return QString::number(size / 1024) + QLatin1Char('K');
    return QString::number(size);
}

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

QModelIndex CFileModel::rootPathIndex() const
{
    return index(rootPath());
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
    if (index.isValid() && role >= eSizeRole) {
        switch (role) {
        case eSizeRole:
            return QVariant(sizeString(fileInfo(index)));
        case eStateRole:
            return "";
        default:
            break;
        }
    }

    return QFileSystemModel::data(index, role);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::stageSelection()
{
}

//-------------------------------------------------------------------------------------------------

void CFileModel::unstageSelection()
{
}
