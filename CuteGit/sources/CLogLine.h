
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

//-------------------------------------------------------------------------------------------------

class CLogLine : public QObject
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, commitId, CommitId)
    Q_FAST_PROPERTY(QDateTime, d, date, Date)
    Q_FAST_PROPERTY(QString, s, author, Author)
    Q_FAST_PROPERTY(QString, s, message, Message)
    Q_FAST_PROPERTY(bool, b, markedAsDiffFrom, MarkedAsDiffFrom)
    Q_FAST_PROPERTY(bool, b, markedAsDiffTo, MarkedAsDiffTo)
};
