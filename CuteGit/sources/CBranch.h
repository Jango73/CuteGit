
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"

// Application
#include "CLabel.h"

//-------------------------------------------------------------------------------------------------

class CBranch : public CLabel
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, commitId, CommitId)
    Q_FAST_PROPERTY(int, i, commitCountAhead, CommitCountAhead)
    Q_FAST_PROPERTY(int, i, commitCountBehind, CommitCountBehind)

public:

    CBranch(QObject* parent = nullptr)
        : CLabel(parent)
        , m_iCommitCountAhead(0)
        , m_iCommitCountBehind(0)
    {
        setType(CEnums::LocalBranchLabel);
    }

    CBranch(const CBranch& target)
        : CLabel(target)
        , m_iCommitCountAhead(target.m_iCommitCountAhead)
        , m_iCommitCountBehind(target.m_iCommitCountBehind)
    {
    }
};
