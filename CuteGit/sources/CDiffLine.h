
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"
#include "CXMLNode.h"

//-------------------------------------------------------------------------------------------------

class CDiffLine : public QObject
{
    Q_OBJECT

public:

    enum EDiffOperation
    {
        None,
        Add,
        Delete
    };

    Q_ENUMS(EDiffOperation)

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, text, Text)
    Q_FAST_PROPERTY(EDiffOperation, e, operation, Operation)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructor & destructor
    //-------------------------------------------------------------------------------------------------

    CDiffLine() : m_eOperation(None) {}
};
