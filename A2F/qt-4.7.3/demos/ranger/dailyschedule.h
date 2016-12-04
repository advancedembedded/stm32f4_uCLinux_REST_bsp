#ifndef DAILYSCHEDULE_H
#define DAILYSCHEDULE_H

#include <QString>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QJsonObject>
#else
#include "qjson/qjson_p.h"
#endif
class DailySchedule
{
    QString period;
    QString startTime;
    QString stopTime;
    QString timeRange;
    int heatTo;
    int coolTo;
    int index;

public:
    DailySchedule();
    QString getPeriod();
    QString getStartTime();
    QString getStopTime();
    QString getTimeRange();
    int getHeatTo();
    int getCoolTo();
    int getIndex();

    void setPeriod(QString s);
    void setStartTime(QString s);
    void setStopTime(QString s);
    void setTimeRange(QString s);
    void setHeatTo(int t);
    void setCoolTo(int t);
    void setIndex(int t);

    void fromJson(QJsonObject &json);
    void toJson(QJsonObject &json, DailySchedule);
};

#endif // DAILYSCHEDULE_H
