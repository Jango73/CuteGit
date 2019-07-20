
// Application
#include "CLogModel.h"

//-------------------------------------------------------------------------------------------------

CLogModel::CLogModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

CLogModel::~CLogModel()
{
    qDeleteAll(m_lGraphLines);
}

//-------------------------------------------------------------------------------------------------

void CLogModel::setGraphLines(QList<CLogLine*> lNewLines)
{
    beginResetModel();

    qDeleteAll(m_lGraphLines);
    m_lGraphLines.clear();

    for (CLogLine* pLine : lNewLines)
        m_lGraphLines << pLine;

    endResetModel();
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CLogModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eCommitIdRole] = "commitId";
    hRoleNames[eDateRole] = "date";
    hRoleNames[eAuthorRole] = "author";
    hRoleNames[eMessageRole] = "message";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CLogModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lGraphLines.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CLogModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    if ((row < 0) || (row > (rowCount() - 1)))
        return QVariant();

    switch (role)
    {
    case eCommitIdRole:
        return m_lGraphLines[row]->commitId();

    case eDateRole:
        return m_lGraphLines[row]->date();

    case eAuthorRole:
        return m_lGraphLines[row]->author();

    case eMessageRole:
        return m_lGraphLines[row]->message();
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

//void CGraphModel::addItem(...)
//{
//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//    m_lGraphLines << item;
//    endInsertRows();
//}

//-------------------------------------------------------------------------------------------------

bool CLogModel::isEmpty() const
{
    return m_lGraphLines.count() == 0;
}
