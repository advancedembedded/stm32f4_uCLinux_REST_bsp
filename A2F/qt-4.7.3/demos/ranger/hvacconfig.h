#ifndef HVACCONFIG_H
#define HVACCONFIG_H

#include <QString>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QJsonObject>
#else
#include "qjson/qjson_p.h"
#endif

class HvacConfig
{
    QString hvacType;
    QString backupHeatingType;

    int coolingStages;
    int heatingStages;

public:
    HvacConfig();
    QString getHvacType();
    QString getBackupHeatingType();
    int getCoolingStages();
    int getHeatingStages();

    void setHvacType(QString s);
    void setBackupHeatingType(QString s);
    void setCoolingStages(int i);
    void setHeatingStages(int i);
    void toJson(QJsonObject &json);
    void fromJson(QJsonObject &json);

};

#endif // HVACCONFIG_H
