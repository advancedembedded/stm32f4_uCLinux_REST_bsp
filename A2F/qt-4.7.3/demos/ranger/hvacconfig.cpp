#include "hvacconfig.h"

HvacConfig::HvacConfig()
{
    QString hvacType;
    QString backupHeatingType;

    int coolingStages;
    int heatingStages;

}
void HvacConfig::setHvacType(QString s) {
    hvacType = s;
}
QString HvacConfig::getHvacType() {
    return hvacType;
}
void HvacConfig::setBackupHeatingType(QString s) {
    backupHeatingType = s;
}
QString HvacConfig::getBackupHeatingType() {
    return backupHeatingType;
}
void HvacConfig::setCoolingStages(int i) {
    coolingStages = i;
}
int HvacConfig::getCoolingStages() {
    return coolingStages;
}
void HvacConfig::setHeatingStages(int i) {
    heatingStages = i;
}
int HvacConfig::getHeatingStages() {
    return heatingStages;
}
void HvacConfig::fromJson(QJsonObject &json)
{
    hvacType = json["hvacType"].toString();
    backupHeatingType = json["backupHeatingType"].toString();
    coolingStages = (int)json["coolingStages"].toDouble();
    heatingStages = (int)json["heatingStages"].toDouble();
}
void HvacConfig::toJson(QJsonObject &json)
{
    json["hvacType"] = hvacType;
    json["backupHeatingType"] = backupHeatingType;
    json["coolingStages"] = coolingStages;
    json["heatingStages"] = heatingStages;
}


