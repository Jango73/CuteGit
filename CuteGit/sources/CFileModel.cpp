
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
    hRoleNames[eStagedRole] = "staged";
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
        case eStagedRole:
        {
            QString sFileFullName = fileInfo(index).absoluteFilePath();
            CRepoFile* pFile = fileByFullName(sFileFullName);

            if (pFile != nullptr)
                return pFile->stagedToString();

            return "";
        }
        default:
            break;
        }
    }

    return QFileSystemModel::data(index, role);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::checkAllFileStatus(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    QVector<CRepoFile*> repoFiles = m_pController->commands()->getAllFileStatus(sPath);
    QVector<QModelIndex> changedIndices;

    for (CRepoFile* pFile : repoFiles)
    {
        CRepoFile* pExistingFile = fileByFullName(pFile->fullName());

        if (pExistingFile != nullptr)
        {
            if (pExistingFile->status() != pFile->status() || pExistingFile->staged() != pFile->staged())
            {
                QModelIndex qIndex = index(pExistingFile->fullName());
                changedIndices << qIndex;
            }
        }
    }

    qDeleteAll(m_vRepoFiles);
    m_vRepoFiles.clear();

    for (CRepoFile* pFile : repoFiles)
    {
        m_vRepoFiles << pFile;
    }

    for (QModelIndex qIndex : changedIndices)
    {
        emit dataChanged(qIndex, qIndex);
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModel::stageSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        QString sOutput = m_pController->commands()->stageFile(m_pController->repositoryPath(), sFileFullName, true);

        emit newOutput(sOutput);
    }

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::unstageSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        QString sOutput = m_pController->commands()->stageFile(m_pController->repositoryPath(), sFileFullName, false);

        emit newOutput(sOutput);
    }

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onRootPathChanged(const QString& sNewPath)
{
    checkAllFileStatus();

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pGraphModel->setStringList(m_pController->commands()->getGraph(sNewPath, dFrom, dTo));
}
