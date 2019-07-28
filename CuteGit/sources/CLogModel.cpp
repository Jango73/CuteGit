
// Application
#include "CLogModel.h"
#include "CRepository.h"

//-------------------------------------------------------------------------------------------------

CLogModel::CLogModel(CRepository *pRepository, QObject* parent)
    : QAbstractListModel(parent)
    , m_pRepository(pRepository)
{
}

//-------------------------------------------------------------------------------------------------

CLogModel::~CLogModel()
{
    qDeleteAll(m_lLines);
}

//-------------------------------------------------------------------------------------------------

void CLogModel::setLines(QList<CLogLine*> lNewLines)
{
    beginResetModel();

    qDeleteAll(m_lLines);
    m_lLines.clear();

    for (CLogLine* pLine : lNewLines)
        m_lLines << pLine;

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
    hRoleNames[eLabelsRole] = "labels";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CLogModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lLines.count();
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
        return m_lLines[row]->commitId();

    case eDateRole:
        return m_lLines[row]->date();

    case eAuthorRole:
        return m_lLines[row]->author();

    case eMessageRole:
        return m_lLines[row]->message();

    case eLabelsRole:
        return m_pRepository->labelsForCommit(m_lLines[row]->commitId());
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

void CLogModel::commitChanged(const QString& sCommitId)
{
    for (int iLineIndex = 0; iLineIndex < m_lLines.count(); iLineIndex++)
    {
        CLogLine* pLine = m_lLines[iLineIndex];

        if (pLine->commitId() == sCommitId)
        {
            emit dataChanged(index(iLineIndex), index(iLineIndex));
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CLogModel::invalidate()
{
    beginResetModel();
    endResetModel();
}
