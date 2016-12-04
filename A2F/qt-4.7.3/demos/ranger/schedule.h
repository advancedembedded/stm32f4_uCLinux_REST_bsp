#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "day.h"

class Schedule
{
    QString label;
    bool systemDefined;
    QList<Day> days;

public:
    Schedule();
    QString getLabel();
    bool getSystemDefined();
    QList<Day> getDays();

    void setLabel(QString s);
    void setSystemDefined(bool b);
    void setDays(QList<Day> d);
    void toJson(QJsonObject &json);
    void fromJson(QJsonObject &json);
};

#endif // SCHEDULE_H
