
DEPENDPATH += .
INCLUDEPATH += .

QT       = core gui widgets network

CONFIG -= moc app_bundle

CONFIG += static
DEFINES += STATIC

TRANSLATIONS  = ranger_es.ts
RESOURCES = ranger.qrc

TARGET = ranger
TEMPLATE = app

!greaterThan(QT_MAJOR_VERSION, 4): {
    SOURCES += \
        qjson/qjson.cpp \
        qjson/qjsonarray.cpp \
        qjson/qjsondocument.cpp \
        qjson/qjsonobject.cpp \
        qjson/qjsonparser.cpp \
        qjson/qjsonvalue.cpp \
        qjson/qjsonwriter.cpp

    HEADERS += \
        qjson/qjson_p.h \
        qjson/qjsonarray.h \
        qjson/qjsondocument.h \
        qjson/qjsonobject.h \
        qjson/qjsonparser_p.h \
        qjson/qjsonvalue.h \
        qjson/qjsonwriter_p.h
}


SOURCES += ranger.cpp \
    dailyschedule.cpp \
    thermostat.cpp \
    rangertheme.cpp \
    roundedpolygon.cpp \
    rangercommon.cpp \
    hvacconfig.cpp \
    day.cpp \
    period.cpp \
    schedule.cpp

HEADERS  += ranger.h \
    dailyschedule.h \
    thermostat.h \
    rangertheme.h \
    roundedpolygon.h \
    rangercommon.h \
    hvacconfig.h \
    day.h \
    period.h \
    schedule.h

