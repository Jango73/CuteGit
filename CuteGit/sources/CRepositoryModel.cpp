
// Application
#include "CRepositoryModel.h"

//-------------------------------------------------------------------------------------------------

CRepositoryModel::CRepositoryModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

CRepositoryModel::~CRepositoryModel()
{
    qDeleteAll(m_lRepositories);
}

//-------------------------------------------------------------------------------------------------

QList<CRepository*> CRepositoryModel::repositories() const
{
    return m_lRepositories;
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CRepositoryModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eRepositoryRole] = "repository";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CRepositoryModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lRepositories.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CRepositoryModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        switch (role)
        {
        case eRepositoryRole:
            QVariant vReturnValue;
            vReturnValue.setValue(m_lRepositories[index.row()]);
            return vReturnValue;
        }
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

void CRepositoryModel::addRepository(CRepository* pRepository)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_lRepositories << pRepository;
    endInsertRows();
}

//-------------------------------------------------------------------------------------------------

bool CRepositoryModel::hasRepository(const QString& sPath)
{
    for (CRepository* pRepository : m_lRepositories)
    {
        if (pRepository->repositoryPath() == sPath)
            return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------

void CRepositoryModel::fileFiltersChanged()
{
    for (CRepository* pRepository : m_lRepositories)
    {
        if (pRepository != nullptr)
        {
            pRepository->treeFileModelProxy()->filterChanged();
            pRepository->flatFileModelProxy()->filterChanged();
        }
    }
}
