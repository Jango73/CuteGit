
// Qt
#include <QDebug>

// Application
#include "CFlatFileModel.h"
#include "CController.h"

//-------------------------------------------------------------------------------------------------

CFlatFileModel::CFlatFileModel(CController* pController, QObject* parent)
    : QAbstractListModel(parent)
    , m_pController(pController)
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

    return m_pController->repository()->repoFiles().count();
}

//-------------------------------------------------------------------------------------------------

QVariant CFlatFileModel::data(const QModelIndex& qIndex, int iRole) const
{
    if (qIndex.isValid())
    {
        switch (iRole)
        {
        case eFullNameRole:
            return m_pController->repository()->repoFiles()[qIndex.row()]->fullName();

        case eFileNameRole:
            return m_pController->repository()->repoFiles()[qIndex.row()]->fileName();

        case eRelativeNameRole:
            return m_pController->repository()->repoFiles()[qIndex.row()]->relativeName();

        case eSizeRole:
            return 0;

        case eStatusRole:
            return m_pController->repository()->repoFiles()[qIndex.row()]->statusToString();

        case eStagedRole:
            return m_pController->repository()->repoFiles()[qIndex.row()]->stagedToString();
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
        emit currentFileFullName(m_pController->repository()->repoFiles()[qIndex.row()]->fullName());
    }
}
