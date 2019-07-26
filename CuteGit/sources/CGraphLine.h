
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"

// Application
#include "CLogLine.h"

//-------------------------------------------------------------------------------------------------

class CGraphLine : public CLogLine
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, graphSymbol, GraphSymbol)
};
