
#pragma once

// Qt
#include <QObject>
#include <QDateTime>

// qt-plus
#include "Macros.h"

// Application
#include "CEnums.h"

//-------------------------------------------------------------------------------------------------

class CLabel : public QObject
{
    Q_OBJECT

    //-------------------------------------------------------------------------------------------------
    // QML properties
    //-------------------------------------------------------------------------------------------------

    Q_FAST_PROPERTY(QString, s, name, Name)
    Q_FAST_PROPERTY(CEnums::ELabelType, e, type, Type)

public:

    //-------------------------------------------------------------------------------------------------
    // Constructors
    //-------------------------------------------------------------------------------------------------

    CLabel(QObject* parent = nullptr)
        : QObject(parent)
        , m_eType(CEnums::UnknownLabel)
    {}

    CLabel(const CLabel& target)
        : m_sName(target.m_sName)
        , m_eType(target.m_eType)
    {
    }
};
