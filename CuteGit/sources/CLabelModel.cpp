
// Qt
#include <QDebug>

// Application
#include "CLabelModel.h"
#include "CRepository.h"

//-------------------------------------------------------------------------------------------------

CLabelModel::CLabelModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

//-------------------------------------------------------------------------------------------------

CLabelModel::~CLabelModel()
{
}

//-------------------------------------------------------------------------------------------------

QHash<int, QByteArray> CLabelModel::roleNames() const
{
    QHash<int, QByteArray> hRoleNames;
    hRoleNames[eNameRole] = "name";
    hRoleNames[eTypeRole] = "type";
    return hRoleNames;
}

//-------------------------------------------------------------------------------------------------

int CLabelModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return m_lLabels.count();
}

//-------------------------------------------------------------------------------------------------

QVariant CLabelModel::data(const QModelIndex& qIndex, int iRole) const
{
    if (qIndex.isValid())
    {
        switch (iRole)
        {
        case eNameRole:
            return m_lLabels[qIndex.row()]->name();

        case eTypeRole:
            return m_lLabels[qIndex.row()]->type();
        }
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

void CLabelModel::setLabelList(QList<CLabel*> lNewList)
{
    beginResetModel();

    qDeleteAll(m_lLabels);
    m_lLabels.clear();

    for (CLabel* pLabel : lNewList)
        m_lLabels << pLabel;

    endResetModel();
}
