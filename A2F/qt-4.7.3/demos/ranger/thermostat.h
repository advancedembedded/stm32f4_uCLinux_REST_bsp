#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <QString>

class Thermostat
{
    QString name;
    QString room;

public:
    Thermostat();
    QString getName();
    QString getRoom();

    void setName(QString s);
    void setRoom(QString s);
};

#endif // THERMOSTAT_H
