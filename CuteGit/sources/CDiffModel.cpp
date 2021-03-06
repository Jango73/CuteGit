
// Application
#include "CDiffModel.h"

//-------------------------------------------------------------------------------------------------

CDiffModel::CDiffModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

CDiffModel::~CDiffModel()
{
    qDeleteAll(m_lLines);
}

//-------------------------------------------------------------------------------------------------

void CDiffModel::setLines(QList<CDiffLine*> lNewLines)
{
    beginResetModel();

    qDeleteAll(m_lLines);
    m_lLines.clear();

    for (CDiffLine* pLine : lNewLines)
        m_lLines << pLine;

    endResetModel();
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CDiffModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eOperationRole] = "operation";
    hRoleNames[eTextRole] = "text";
    hRoleNames[eCommitIdRole] = "commitId";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CDiffModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return m_lLines.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CDiffModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    if ((row < 0) || (row > (rowCount() - 1)))
        return QVariant();

    switch (role)
    {
    case eOperationRole:
        return m_lLines[row]->operation();

    case eTextRole:
        return m_lLines[row]->text();

    case eCommitIdRole:
        return m_lLines[row]->commitId();
    }

    return QVariant();
}
