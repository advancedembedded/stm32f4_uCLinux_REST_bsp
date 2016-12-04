#include "schedule.h"

Schedule::Schedule()
{
   QString label;
   bool systemDefined;
   QList<Day> days;
}
QString Schedule::getLabel() {
     return label;
 }

void Schedule::setLabel(QString s) {
     label = s;
 }

bool Schedule::getSystemDefined() {
     return systemDefined;
 }

void Schedule::setSystemDefined(bool b) {
     systemDefined = b;
 }

QList<Day> Schedule::getDays() {
     return days;
 }

void Schedule::setDays(QList<Day> d) {
     days = d;
 }
void Schedule::fromJson(QJsonObject &json)
{
    QJsonArray dayArray;
    //qDebug() << "SC" << json["label"].toString();
    label = json["label"].toString().toUpper();
    QJsonArray daysArray = json["days"].toArray();
    for (int i = 0; i < daysArray.size(); ++i) {
        QJsonObject dayObject = daysArray[i].toObject();
        //qDebug() << "DR" << dayObject["label"].toString();
        Day day;
        day.fromJson(dayObject);
        days<<day;
        dayArray.append(dayObject);
    }
    json["days"] = dayArray;
}
void Schedule::toJson(QJsonObject &json)
{
    //qDebug() << "SC" << label;
    json["label"] = label.toLower();
    json["systemDefined"] = systemDefined;
    QJsonArray dayArray;
    foreach (Day day, days) {
        QJsonObject dayo;
        day.toJson(dayo);
        //qDebug() << "DO" << day.getLabel();
        dayArray.append(dayo);
    }
    json["days"] = dayArray;
}
