
// Qt
#include <QDebug>

// Application
#include "CBranchModel.h"
#include "CRepository.h"

//-------------------------------------------------------------------------------------------------

CBranchModel::CBranchModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

CBranchModel::~CBranchModel()
{
}

//-------------------------------------------------------------------------------------------------

QStringList CBranchModel::labelsForCommit(const QString& sCommitId) const
{
    QStringList lReturnValue;

    for (CBranch* pBranch : m_lBranches)
    {
        if (pBranch->commitId() == sCommitId)
            lReturnValue << pBranch->name();
    }

    return lReturnValue;
}

//-------------------------------------------------------------------------------------------------

QStringList CBranchModel::branchNames() const
{
    QStringList lReturnValue;

    for (CBranch* pBranch : m_lBranches)
        lReturnValue << pBranch->name();

    return lReturnValue;
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CBranchModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eNameRole] = "name";
    hRoleNames[eCommitIdRole]= "commitId";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CBranchModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lBranches.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CBranchModel::data(const QModelIndex& qIndex, int iRole) const
{
    if (qIndex.isValid())
    {
        switch (iRole)
        {
        case eNameRole:
            return m_lBranches[qIndex.row()]->name();

        case eCommitIdRole:
            return m_lBranches[qIndex.row()]->commitId();
        }
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

void CBranchModel::setBranchList(QList<CBranch*> lNewList)
{
    beginResetModel();

    qDeleteAll(m_lBranches);
    m_lBranches.clear();

    for (CBranch* pBranch : lNewList)
        m_lBranches << pBranch;

    endResetModel();
}

//-------------------------------------------------------------------------------------------------

void CBranchModel::setBranchHeadCommit(const QString& sBranch, const QString& sCommitId)
{
    for (int iBranchIndex = 0; iBranchIndex < m_lBranches.count(); iBranchIndex++)
    {
        CBranch* pBranch = m_lBranches[iBranchIndex];

        if (pBranch->name() == sBranch)
        {
            pBranch->setCommitId(sCommitId);
            emit dataChanged(index(iBranchIndex), index(iBranchIndex));
            break;
        }
    }
}
