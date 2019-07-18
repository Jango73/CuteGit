
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
    , m_pDiffModel(new QStringListModel())
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

CRepoFile* CFileModel::fileByFullName(const QVector<CRepoFile*>& vFiles, const QString& sFullName) const
{
    for (CRepoFile* pFile : vFiles)
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

QVariant CFileModel::data(const QModelIndex& index, int role) const
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
            CRepoFile* pFile = fileByFullName(m_vRepoFiles, sFileFullName);

            if (pFile != nullptr)
                return pFile->statusToString();

            return CRepoFile::sTokenClean;
        }
        case eStagedRole:
        {
            QString sFileFullName = fileInfo(index).absoluteFilePath();
            CRepoFile* pFile = fileByFullName(m_vRepoFiles, sFileFullName);

            if (pFile != nullptr)
                return pFile->stagedToString();

            return CRepoFile::sTokenUnstaged;
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

    QVector<CRepoFile*> vNewRepoFiles = m_pController->commands()->getAllFileStatus(sPath);
    QVector<QModelIndex> changedIndices;

    for (CRepoFile* pExistingFile : m_vRepoFiles)
    {
        CRepoFile* pNewFile = fileByFullName(vNewRepoFiles, pExistingFile->fullName());

        if (pNewFile == nullptr)
        {
            QModelIndex qIndex = index(pExistingFile->fullName());
            changedIndices << qIndex;
        }
    }

    for (CRepoFile* pFile : vNewRepoFiles)
    {
        CRepoFile* pExistingFile = fileByFullName(m_vRepoFiles, pFile->fullName());

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

    for (CRepoFile* pFile : vNewRepoFiles)
    {
        m_vRepoFiles << pFile;
    }

    for (QModelIndex qIndex : changedIndices)
    {
        emit dataChanged(qIndex, qIndex);
    }
}

//-------------------------------------------------------------------------------------------------

void CFileModel::handleCurrentIndex(QModelIndex qIndex)
{
    QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
    QString sOutput = m_pController->commands()->unstagedDiff(m_pController->repositoryPath(), sFileFullName);
    QStringList lNewList = sOutput.split("\n");

    m_pDiffModel->setStringList(lNewList);
}

//-------------------------------------------------------------------------------------------------

void CFileModel::refresh()
{
    checkAllFileStatus();
    getGraph();
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

void CFileModel::stageAll()
{
    QString sOutput = m_pController->commands()->stageAll(m_pController->repositoryPath(), true);

    emit newOutput(sOutput);

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::revertSelection(QModelIndexList lIndices)
{
    for (QModelIndex qIndex : lIndices)
    {
        QString sFileFullName = fileInfo(qIndex).absoluteFilePath();
        QString sOutput = m_pController->commands()->revertFile(m_pController->repositoryPath(), sFileFullName);

        emit newOutput(sOutput);
    }

    checkAllFileStatus();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::commit(const QString& sMessage)
{
    QString sOutput = m_pController->commands()->commit(m_pController->repositoryPath(), sMessage);

    emit newOutput(sOutput);

    checkAllFileStatus();
    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::push()
{
    QString sOutput = m_pController->commands()->push(m_pController->repositoryPath());

    emit newOutput(sOutput);

    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::pull()
{
    QString sOutput = m_pController->commands()->pull(m_pController->repositoryPath());

    emit newOutput(sOutput);

    getGraph();
}

//-------------------------------------------------------------------------------------------------

void CFileModel::getGraph(QString sPath)
{
    if (sPath.isEmpty())
    {
        sPath = m_pController->repositoryPath();
    }

    QDateTime dFrom = QDateTime::currentDateTime().addDays(-2);
    QDateTime dTo = QDateTime::currentDateTime().addDays(2);

    m_pGraphModel->setStringList(m_pController->commands()->getGraph(sPath, dFrom, dTo));
}

//-------------------------------------------------------------------------------------------------

void CFileModel::onRootPathChanged(const QString& sNewPath)
{
    Q_UNUSED(sNewPath);

    checkAllFileStatus();
    getGraph();
}
