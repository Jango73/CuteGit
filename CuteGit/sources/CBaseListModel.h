
#pragma once

// Qt
#include <QObject>
#include <QVariant>
#include <QVector>
#include <QAbstractListModel>

//-------------------------------------------------------------------------------------------------

class CBaseListModel : public QAbstractListModel
{
public:

    // Returns number of items in the list
    Q_INVOKABLE virtual int count() = 0;

    // Returns an item of the list
    Q_INVOKABLE virtual QObject* get(int index) = 0;

    // Appends an item to the list
    Q_INVOKABLE virtual void append(QObject* pObject) = 0;

    // Removes an item from the list
    Q_INVOKABLE virtual void remove(int index, int count = 1) = 0;

    // Moves some items in the list
    Q_INVOKABLE virtual void move(int fromIndex, int toIndex, int count = 1) = 0;
};
