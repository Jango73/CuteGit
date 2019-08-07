
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"

//-------------------------------------------------------------------------------------------------

class CBranch : public QObject
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, name, Name)
    Q_FAST_PROPERTY(QString, s, commitId, CommitId)
    Q_FAST_PROPERTY(int, i, commitCountAhead, CommitCountAhead)
    Q_FAST_PROPERTY(int, i, commitCountBehind, CommitCountBehind)
};
