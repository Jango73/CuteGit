
// Application
#include "CFileModel.h"
#include "CController.h"

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

CFileModel::CFileModel(CController* pController, QObject* parent)
    : QFileSystemModel(parent)
    , m_pController(pController)
    , m_pGraphModel(new QStringListModel())
{
    setRootPath(QDir::homePath());
    setResolveSymlinks(true);

    connect(this, &QFileSystemModel::rootPathChanged, this, &CFileModel::onRootPathChanged);
}

//-------------------------------------------------------------------------------------------------

CFileModel::~CFileModel()
{
    qDeleteAll(m_vRepoFiles);
}

//-------------------------------------------------------------------------------------------------

QModelIndex CFileModel::rootPathIndex() const
{
    return index(rootPath());
}

//-------------------------------------------------------------------------------------------------

CRepoFile* CFileModel::fileByFullName(const QString& sFullName) const
{
    for (CRepoFile* pFile : m_vRepoFiles)
    {
        if (pFile->fullName() == sFullName)
            return pFile;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames = QFileSystemModel::roleNames();
    hRoleNames[eSizeRole] = "size";
    hRoleNames[eStatusRole] = "status";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

QVariant CFileModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role >= eSizeRole)
    {
        switch (role)
        {
        case eSizeRole:
        {
            return QVariant(sizeString(fileInfo(index)));
        }
        case eStatusRole:
        {
            QString sFileFullName = fileInfo(index).absoluteFilePath();
            CRepoFile* pFile = fileByFullName(sFileFullName);
            if (pFile != nullptr)
                return pFile->statusToString();
            return "";
        }
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

//-------------------------------------------------------------------------------------------------

void CFileModel::onRootPathChanged(const QString& newPath)
{
    QVector<CRepoFile*> repoFiles = m_pController->commands()->getAllFileStatus(newPath);

    qDeleteAll(m_vRepoFiles);
    m_vRepoFiles.clear();

    for (CRepoFile* pFile : repoFiles)
    {
        m_vRepoFiles << pFile;
    }

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pGraphModel->setStringList(m_pController->commands()->getGraph(newPath, dFrom, dTo));
}
