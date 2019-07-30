
// Application
#include "CGraphModel.h"

//-------------------------------------------------------------------------------------------------

CGraphModel::CGraphModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

CGraphModel::~CGraphModel()
{
    qDeleteAll(m_lLines);
}

//-------------------------------------------------------------------------------------------------

void CGraphModel::setLines(QList<CGraphLine*> lNewLines)
{
    beginResetModel();

    qDeleteAll(m_lLines);
    m_lLines.clear();

    for (CGraphLine* pLine : lNewLines)
        m_lLines << pLine;

    endResetModel();
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CGraphModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eCommitIdRole] = "commitId";
    hRoleNames[eDateRole] = "date";
    hRoleNames[eAuthorRole] = "author";
    hRoleNames[eMessageRole] = "message";
    hRoleNames[eGraphSymbolRole] = "graphSymbol";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CGraphModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lLines.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CGraphModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if (!index.isValid())
        return QVariant();

    if ((row < 0) || (row > (rowCount() - 1)))
        return QVariant();

    switch (role)
    {
    case eCommitIdRole:
        return m_lLines[row]->commitId();

    case eDateRole:
        return m_lLines[row]->date();

    case eAuthorRole:
        return m_lLines[row]->author();

    case eMessageRole:
        return m_lLines[row]->message();

    case eGraphSymbolRole:
        return m_lLines[row]->graphSymbol();
    }

    return QVariant();
}