#include "period.h"

Period::Period()
{
    QString label;
    QString startTime;
    QString stopTime;
    int startTimeUtc;
    int coolTemp;
    int heatTemp;
}

QString Period::getLabel() {
    return label;
}

void Period::setLabel(QString s) {
    label = s;
}

QString Period::getStartTime() {
    return startTime;
}

void Period::setStartTime(QString s) {
    startTime = s;
}

QString Period::getStopTime() {
    return stopTime;
}

void Period::setStopTime(QString s) {
    stopTime = s;
}

int Period::getStartTimeUtc() {
    return startTimeUtc;
}

void Period::setStartTimeUtc(int i) {
    startTimeUtc = i;
}

int Period::getCoolTemp() {
    return coolTemp;
}

void Period::setCoolTemp(int i) {
    coolTemp = i;
}
int Period::getHeatTemp() {
    return heatTemp;
}

void Period::setHeatTemp(int i) {
    heatTemp = i;
}
void Period::fromJson(QJsonObject &json)
{
    label = json["label"].toString();
    startTime = json["startTime"].toString();
    stopTime = json["stopTime"].toString();
    startTimeUtc = (int)json["startTimeUtc"].toDouble();
    coolTemp = (int)json["coolTemp"].toDouble();
    heatTemp = (int)json["heatTemp"].toDouble();
}
void Period::toJson(QJsonObject &json)
{
    json["label"] = label;
    json["startTime"] = startTime;
    json["stopTime"] = stopTime;
    json["startTimeUtc"] = startTimeUtc;
    json["coolTemp"] = coolTemp;
    json["heatTemp"] = heatTemp;
}
