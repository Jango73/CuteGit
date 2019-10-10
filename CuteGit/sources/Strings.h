
// Qt
#include <QObject>
#include <QString>

#pragma once

//-------------------------------------------------------------------------------------------------

class Strings : public QObject
{
    Q_OBJECT

public:

    static const QString s_sSomeObjectNameOpened;
    static const QString s_sSomeObjectNameIsNotARepository;
    static const QString s_sSomeObjectDoesNotExist;

    static const QString s_sPushingToGerrit;
    static const QString s_sUnableToGetBranch;
};
