#ifndef DAY_H
#define DAY_H

#include <QSet>
#include "period.h"

class Day
{
    QString label;
    QList<Period> periods;

public:
    Day();
    QString getLabel();
    QList<Period> getPeriods();

    void setLabel(QString s);
    void setPeriods(QList<Period> s);
    void toJson(QJsonObject &json);
    void fromJson(QJsonObject &json);
};

#endif // DAY_H
