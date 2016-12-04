#include "thermostat.h"

Thermostat::Thermostat()
{
    QString name;
    QString room;
}
void Thermostat::setName(QString s) {
    name = s;
}

QString Thermostat::getName() {
    return name;
}
void Thermostat::setRoom(QString s) {
    room = s;
}

QString Thermostat::getRoom() {
    return room;
}
