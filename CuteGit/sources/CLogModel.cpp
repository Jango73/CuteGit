
// Qt
#include <QDebug>

// Application
#include "CLogModel.h"
#include "CRepository.h"

//-------------------------------------------------------------------------------------------------

CLogModel::CLogModel(CRepository *pRepository, QObject* parent)
    : QAbstractListModel(parent)
    , m_pRepository(pRepository)
    , m_iPotentialCount(0)
{
}

//-------------------------------------------------------------------------------------------------

CLogModel::~CLogModel()
{
    clear();
}

//-------------------------------------------------------------------------------------------------

void CLogModel::clear()
{
    qDeleteAll(m_lLines);
    m_lLines.clear();
}

//-------------------------------------------------------------------------------------------------

void CLogModel::setLines(CLogLineCollection tCollection)
{
    setPotentialCount(tCollection.potentialCount());

    for (int iIndex = 0; iIndex < tCollection.lines().count(); iIndex++)
    {
        int iFinalIndex = tCollection.startIndex() + iIndex;

        if (iFinalIndex < m_lLines.count())
        {
            if (m_lLines[iFinalIndex] != nullptr)
                delete m_lLines[iFinalIndex];

            QModelIndex qIndex = createIndex(iFinalIndex, 0);
            m_lLines[iFinalIndex] = tCollection.lines()[iIndex];
            emit dataChanged(qIndex, qIndex);
        }
        else if (iFinalIndex == m_lLines.count())
        {
            beginInsertRows(QModelIndex(), m_lLines.count(), m_lLines.count());
            endInsertRows();

            m_lLines << tCollection.lines()[iIndex];
        }
        else
        {
            qDebug() << "CLogModel::setLines() : log lines must be sequential";
        }
    }
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CLogModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eCommitIdRole] = "commitId";
    hRoleNames[eDateRole] = "date";
    hRoleNames[eAuthorRole] = "author";
    hRoleNames[eMessageRole] = "message";
    hRoleNames[eFullMessageRole] = "fullMessage";
    hRoleNames[eLabelsRole] = "labels";
    hRoleNames[eMessageIsCompleteRole] = "messageIsComplete";
    hRoleNames[eMarkedAsDiffFromRole] = "markedAsDiffFrom";
    hRoleNames[eMarkedAsDiffToRole] = "markedAsDiffTo";
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

    if (row < 0 || row >= m_lLines.count())
    {
        return QVariant();
    }

    switch (role)
    {
    case eCommitIdRole:
        return m_lLines[row]->commitId();

    case eDateRole:
        return m_lLines[row]->date();

    case eAuthorRole:
        return m_lLines[row]->author();

    case eMessageRole:
        return m_lLines[row]->message().split(NEW_LINE).first();

    case eFullMessageRole:
        return m_lLines[row]->message();

    case eLabelsRole:
        return m_pRepository->labelsForCommit(m_lLines[row]->commitId());

    case eMessageIsCompleteRole:
        return m_lLines[row]->messageIsComplete();

    case eMarkedAsDiffFromRole:
        return m_pRepository->diffFromCommitId() == m_lLines[row]->commitId();

    case eMarkedAsDiffToRole:
        return m_pRepository->diffToCommitId() == m_lLines[row]->commitId();

    default:
        return QVariant();
    }
}

//-------------------------------------------------------------------------------------------------

bool CLogModel::canFetchMore(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lLines.count() < m_iPotentialCount;
}

//-------------------------------------------------------------------------------------------------

void CLogModel::fetchMore(const QModelIndex& parent)
{
    Q_UNUSED(parent);

    int iCurrentCount = m_lLines.count();
    int iNewCount = LOG_COUNT_DEFAULT;

    if (iNewCount > m_iPotentialCount - iCurrentCount)
        iNewCount = m_iPotentialCount - iCurrentCount;

    emit requestLogData(iCurrentCount, iNewCount);
}

//-------------------------------------------------------------------------------------------------

void CLogModel::setCommitMessage(const QString& sCommitId, const QString& sMessage)
{
    for (int iLineIndex = 0; iLineIndex < m_lLines.count(); iLineIndex++)
    {
        CLogLine* pLine = m_lLines[iLineIndex];

        if (pLine != nullptr)
        {
            if (pLine->commitId() == sCommitId)
            {
                pLine->setMessage(sMessage);
                pLine->setMessageIsComplete(true);
                emit dataChanged(index(iLineIndex), index(iLineIndex));
                break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CLogModel::commitChanged(const QString& sCommitId)
{
    for (int iLineIndex = 0; iLineIndex < m_lLines.count(); iLineIndex++)
    {
        CLogLine* pLine = m_lLines[iLineIndex];

        if (pLine != nullptr)
        {
            if (pLine->commitId() == sCommitId)
            {
                emit dataChanged(index(iLineIndex), index(iLineIndex));
                break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CLogModel::invalidate()
{
    beginResetModel();
    endResetModel();
}
