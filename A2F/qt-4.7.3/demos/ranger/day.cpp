#include "day.h"

Day::Day()
{
    QString label;
    QList<Period> periods;
}
QString Day::getLabel() {
    return label;
}

void Day::setLabel(QString s) {
    label = s;
}

QList<Period> Day::getPeriods() {
    return periods;
}

void Day::setPeriods(QList<Period> p) {
    periods = p;
}
void Day::fromJson(QJsonObject &json)
{
    QJsonArray periodArray;
    label = json["label"].toString().toUpper();
    QJsonArray periodsArray = json["periods"].toArray();
    for (int i = 0; i < periodsArray.size(); ++i) {
        QJsonObject po = periodsArray[i].toObject();
        Period p;
        p.fromJson(po);
        periods<<p;
        periodArray.append(po);
    }
    json["periods"] = periodArray;
}
void Day::toJson(QJsonObject &json)
{
    json["label"] = label.toLower();
    QJsonArray periodArray;
    foreach (Period p, periods) {
        QJsonObject po;
        p.toJson(po);
        periodArray.append(po);
    }
    json["periods"] = periodArray;
}

