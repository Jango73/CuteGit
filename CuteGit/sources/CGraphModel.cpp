
// Application
#include "CGraphModel.h"
#include "CRepository.h"

//-------------------------------------------------------------------------------------------------

CGraphModel::CGraphModel(CRepository* pRepository, QObject* parent)
    : QAbstractListModel(parent)
    , m_pRepository(pRepository)
{
}

//-------------------------------------------------------------------------------------------------

CGraphModel::~CGraphModel()
{
    clear();
}

//-------------------------------------------------------------------------------------------------

void CGraphModel::clear()
{
    qDeleteAll(m_lLines);
    m_lLines.clear();
    qDeleteAll(m_mLabels);
    m_mLabels.clear();
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
    hRoleNames[eLabelsRole] = "labels";
    hRoleNames[eGraphSymbolRole] = "graphSymbol";
    hRoleNames[eMessageIsCompleteRole] = "messageIsComplete";
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

    case eLabelsRole:
    {
        return QVariant();
    }

    case eGraphSymbolRole:
        return m_lLines[row]->graphSymbol();

    case eMessageIsCompleteRole:
        return m_lLines[row]->messageIsComplete();
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

void CGraphModel::setCommitMessage(const QString& sCommitId, const QString& sMessage)
{
    for (int iLineIndex = 0; iLineIndex < m_lLines.count(); iLineIndex++)
    {
        CLogLine* pLine = m_lLines[iLineIndex];

        if (pLine->commitId() == sCommitId)
        {
            pLine->setMessage(sMessage);
            pLine->setMessageIsComplete(true);
            emit dataChanged(index(iLineIndex), index(iLineIndex));
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CGraphModel::commitChanged(const QString& sCommitId)
{
    for (int iLineIndex = 0; iLineIndex < m_lLines.count(); iLineIndex++)
    {
        CGraphLine* pLine = m_lLines[iLineIndex];

        if (pLine->commitId() == sCommitId)
        {
            emit dataChanged(index(iLineIndex), index(iLineIndex));
            break;
        }
    }
}
