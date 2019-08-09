
// Qt
#include <QDebug>

// Application
#include "CFlatFileModel.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CFlatFileModel::CFlatFileModel(CRepository* pRepository, QObject* parent)
    : QAbstractListModel(parent)
    , m_pRepository(pRepository)
{
}

//-------------------------------------------------------------------------------------------------

CFlatFileModel::~CFlatFileModel()
{
    qDeleteAll(m_lRepoFiles);
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CFlatFileModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eFullNameRole] = "fullName";
    hRoleNames[eFileNameRole] = "fileName";
    hRoleNames[eRelativeNameRole] = "relativeName";
    hRoleNames[eSizeRole] = "size";
    hRoleNames[eStatusRole] = "status";
    hRoleNames[eStagedRole] = "staged";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CFlatFileModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lRepoFiles.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CFlatFileModel::data(const QModelIndex& qIndex, int iRole) const
{
    if (qIndex.isValid())
    {
        switch (iRole)
        {
        case eFullNameRole:
            return m_lRepoFiles[qIndex.row()]->fullName();

        case eFileNameRole:
            return m_lRepoFiles[qIndex.row()]->fileName();

        case eRelativeNameRole:
            return m_lRepoFiles[qIndex.row()]->relativeName();

        case eSizeRole:
            return 0;

        case eStatusRole:
            return m_lRepoFiles[qIndex.row()]->statusToString();

        case eStagedRole:
            return m_lRepoFiles[qIndex.row()]->stagedToString();
        }
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

bool CFlatFileModel::setData(const QModelIndex& qIndex, const QVariant& vValue, int iRole)
{
    if (qIndex.isValid())
    {
        QVector<int> roles;

        switch (iRole)
        {
        case eStatusRole:
            m_lRepoFiles[qIndex.row()]->setStatus(static_cast<CEnums::ERepoFileStatus>(vValue.toInt()));
            roles << iRole;
            emit dataChanged(qIndex, qIndex, roles);
            break;

        case eStagedRole:
            m_lRepoFiles[qIndex.row()]->setStaged(vValue.toBool());
            roles << iRole;
            emit dataChanged(qIndex, qIndex, roles);
            break;
        }

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::handleRepoFilesChanged()
{
    QList<CRepoFile*> lNewFiles = m_pRepository->repoFiles();

    for (int iNewFileIndex = 0; iNewFileIndex < lNewFiles.count(); iNewFileIndex++)
    {
        CRepoFile* pNewFile = lNewFiles[iNewFileIndex];
        CRepoFile* pExistingFile = m_pRepository->fileByFullName(m_lRepoFiles, pNewFile->fullName());

        if (pExistingFile != nullptr)
        {
            if (pExistingFile->staged() != pNewFile->staged() || pExistingFile->status() != pNewFile->status())
            {
                QModelIndex qIndex = createIndex(iNewFileIndex, 0);

                pExistingFile->setStaged(pNewFile->staged());
                pExistingFile->setStatus(pNewFile->status());

                emit dataChanged(qIndex, qIndex);
            }
        }
        else
        {
            beginInsertRows(QModelIndex(), iNewFileIndex, iNewFileIndex);
            m_lRepoFiles.insert(iNewFileIndex, new CRepoFile(*pNewFile, this));
            endInsertRows();
        }
    }

    for (int iExistingFileIndex = 0; iExistingFileIndex < m_lRepoFiles.count(); iExistingFileIndex++)
    {
        CRepoFile* pExistingFile = m_lRepoFiles[iExistingFileIndex];
        CRepoFile* pNewFile = m_pRepository->fileByFullName(lNewFiles, pExistingFile->fullName());

        if (pNewFile == nullptr)
        {
            beginRemoveRows(QModelIndex(), iExistingFileIndex, iExistingFileIndex);
            delete m_lRepoFiles[iExistingFileIndex];
            m_lRepoFiles.removeAt(iExistingFileIndex);
            endRemoveRows();

            iExistingFileIndex--;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::handleCurrentIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        emit currentFileFullName(m_lRepoFiles[qIndex.row()]->fullName());
    }
}

//-------------------------------------------------------------------------------------------------

QStringList CFlatFileModel::selectionToFullNameList(QModelIndexList lIndices)
{
    QStringList lFullNames;

    for (QModelIndex qIndex : lIndices)
    {
        lFullNames << m_lRepoFiles[qIndex.row()]->fullName();
    }

    return lFullNames;
}
