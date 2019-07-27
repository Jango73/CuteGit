
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

// Application
#include "CEnums.h"

//-------------------------------------------------------------------------------------------------

class CDiffLine : public QObject
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, text, Text)
    Q_FAST_PROPERTY(CEnums::EDiffOperation, e, operation, Operation)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    CDiffLine() : m_eOperation(CEnums::None) {}
};
