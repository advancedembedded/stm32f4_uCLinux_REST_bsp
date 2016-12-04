#include "dailyschedule.h"

DailySchedule::DailySchedule()
{
     QString period;
     QString startTime;
     QString stopTime;
     QString timeRange;
     int heatTo;
     int coolTo;
     int index;
}
void DailySchedule::setPeriod(QString s) {
    period = s;
}
QString DailySchedule::getPeriod() {
    return period;
}
void DailySchedule::setStartTime(QString s) {
    startTime = s;
}
QString DailySchedule::getStartTime() {
    return startTime;
}
void DailySchedule::setStopTime(QString s) {
    stopTime = s;
}
QString DailySchedule::getStopTime() {
    return stopTime;
}
void DailySchedule::setTimeRange(QString s) {
    timeRange = s;
}
QString DailySchedule::getTimeRange() {
    return timeRange;
}
void DailySchedule::setHeatTo(int t) {
    heatTo = t;
}
int DailySchedule::getHeatTo() {
    return heatTo;
}
void DailySchedule::setCoolTo(int t) {
    coolTo = t;
}
int DailySchedule::getCoolTo() {
    return coolTo;
}
void DailySchedule::setIndex(int t) {
    index = t;
}
int DailySchedule::getIndex() {
    return index;
}
void DailySchedule::read(const QJsonObject &json)
{
    startTime = json["start_time"].toString();
    stopTime = json["stop_time"].toDouble();
}

void DailySchedule::write(QJsonObject &json, DailySchedule ds) const
{
    json["start_time"] = ds.getStartTime();
    json["stop_time"] = ds.getStopTime();
    json["heat_to"] = ds.getHeatTo();
    json["cool_to"] = ds.getCoolTo();
//    json["index"] = QString("0");
}

