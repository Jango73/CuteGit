
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

    return m_pRepository->repoFiles().count();
}

//-------------------------------------------------------------------------------------------------

QVariant CFlatFileModel::data(const QModelIndex& qIndex, int iRole) const
{
    if (qIndex.isValid())
    {
        switch (iRole)
        {
        case eFullNameRole:
            return m_pRepository->repoFiles()[qIndex.row()]->fullName();

        case eFileNameRole:
            return m_pRepository->repoFiles()[qIndex.row()]->fileName();

        case eRelativeNameRole:
            return m_pRepository->repoFiles()[qIndex.row()]->relativeName();

        case eSizeRole:
            return 0;

        case eStatusRole:
            return m_pRepository->repoFiles()[qIndex.row()]->statusToString();

        case eStagedRole:
            return m_pRepository->repoFiles()[qIndex.row()]->stagedToString();
        }
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::handleRepoFilesChanged()
{
    beginResetModel();
    endResetModel();
}

//-------------------------------------------------------------------------------------------------

void CFlatFileModel::handleCurrentIndex(QModelIndex qIndex)
{
    if (qIndex.isValid())
    {
        emit currentFileFullName(m_pRepository->repoFiles()[qIndex.row()]->fullName());
    }
}

//-------------------------------------------------------------------------------------------------

QStringList CFlatFileModel::selectionToFullNameList(QModelIndexList lIndices)
{
    QStringList lFullNames;

    for (QModelIndex qIndex : lIndices)
    {
        lFullNames << m_pRepository->repoFiles()[qIndex.row()]->fullName();
    }

    return lFullNames;
}
