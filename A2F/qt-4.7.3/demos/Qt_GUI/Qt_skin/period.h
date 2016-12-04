#ifndef PERIOD_H
#define PERIOD_H

#include <QString>
#include <QDebug>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QJsonObject>
#include <QJsonArray>
#else
#include "qjson/qjson_p.h"
#endif

class Period
{
    QString label;
    QString startTime;
    QString stopTime;
    int startTimeUtc;
    int coolTemp;
    int heatTemp;

public:
    Period();

    QString getLabel();
    QString getStartTime();
    QString getStopTime();
    int getStartTimeUtc();
    int getCoolTemp();
    int getHeatTemp();

    void setLabel(QString s);
    void setStartTime(QString s);
    void setStopTime(QString s);
    void setStartTimeUtc(int i);
    void setCoolTemp(int i);
    void setHeatTemp(int i);
    void toJson(QJsonObject &json);
    void fromJson(QJsonObject &json);
};

#endif // PERIOD_H
