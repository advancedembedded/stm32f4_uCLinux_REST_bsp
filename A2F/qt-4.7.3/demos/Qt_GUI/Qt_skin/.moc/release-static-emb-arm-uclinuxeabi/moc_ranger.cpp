/****************************************************************************
** Meta object code from reading C++ file 'ranger.h'
**
** Created: Tue Nov 15 22:46:26 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ranger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ranger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Ranger[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      42,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    8,    7,    7, 0x0a,
      48,   41,    7,    7, 0x0a,
      85,   41,    7,    7, 0x0a,
     122,   41,    7,    7, 0x0a,
     158,   41,    7,    7, 0x0a,
     196,   41,    7,    7, 0x0a,
     232,   41,    7,    7, 0x0a,
     273,   41,    7,    7, 0x0a,
     314,   41,    7,    7, 0x0a,
     359,   41,    7,    7, 0x0a,
     400,   41,    7,    7, 0x0a,
     435,   41,    7,    7, 0x0a,
     471,    7,    7,    7, 0x0a,
     497,    7,    7,    7, 0x0a,
     508,    7,    7,    7, 0x0a,
     524,    7,    7,    7, 0x0a,
     538,    7,    7,    7, 0x0a,
     551,    7,    7,    7, 0x0a,
     565,    7,    7,    7, 0x0a,
     580,    7,    7,    7, 0x0a,
     592,    7,    7,    7, 0x0a,
     605,    7,    7,    7, 0x0a,
     619,    7,    7,    7, 0x0a,
     634,    7,    7,    7, 0x0a,
     649,    7,    7,    7, 0x0a,
     665,    7,    7,    7, 0x0a,
     682,    7,    7,    7, 0x0a,
     700,    7,    7,    7, 0x0a,
     715,    7,    7,    7, 0x0a,
     730,    7,    7,    7, 0x0a,
     747,    7,    7,    7, 0x0a,
     763,    7,    7,    7, 0x0a,
     781,    7,    7,    7, 0x0a,
     797,    7,    7,    7, 0x0a,
     815,    7,    7,    7, 0x0a,
     832,    7,    7,    7, 0x0a,
     847,    7,    7,    7, 0x0a,
     865,    7,    7,    7, 0x0a,
     880,    7,    7,    7, 0x0a,
     895,    7,    7,    7, 0x0a,
     914,    7,    7,    7, 0x0a,
     933,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Ranger[] = {
    "Ranger\0\0reply\0loadConfig(QNetworkReply*)\0"
    "button\0mainButtonReleased(QAbstractButton*)\0"
    "modeButtonReleased(QAbstractButton*)\0"
    "fanButtonReleased(QAbstractButton*)\0"
    "colorButtonReleased(QAbstractButton*)\0"
    "keyButtonReleased(QAbstractButton*)\0"
    "scheduleButtonReleased(QAbstractButton*)\0"
    "locationButtonReleased(QAbstractButton*)\0"
    "editScheduleButtonReleased(QAbstractButton*)\0"
    "settingsButtonReleased(QAbstractButton*)\0"
    "dayButtonPressed(QAbstractButton*)\0"
    "dayButtonReleased(QAbstractButton*)\0"
    "statsFileChanged(QString)\0showTime()\0"
    "returnPressed()\0screenSaver()\0"
    "changeSize()\0idleClicked()\0restartTimer()\0"
    "upPressed()\0upReleased()\0downPressed()\0"
    "downReleased()\0upNumPressed()\0"
    "upNumReleased()\0downNumPressed()\0"
    "downNumReleased()\0backReleased()\0"
    "nextReleased()\0buttonReleased()\0"
    "prefsReleased()\0profileReleased()\0"
    "setupReleased()\0keyBackReleased()\0"
    "cancelReleased()\0editReleased()\0"
    "calendarChanged()\0doneReleased()\0"
    "saveReleased()\0dateChanged(QDate)\0"
    "timeChanged(QTime)\0ssBoxChanged(QString)\0"
};

const QMetaObject Ranger::staticMetaObject = {
    { &RangerCommon::staticMetaObject, qt_meta_stringdata_Ranger,
      qt_meta_data_Ranger, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Ranger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Ranger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Ranger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Ranger))
        return static_cast<void*>(const_cast< Ranger*>(this));
    return RangerCommon::qt_metacast(_clname);
}

int Ranger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = RangerCommon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loadConfig((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 1: mainButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 2: modeButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 3: fanButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 4: colorButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 5: keyButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 6: scheduleButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 7: locationButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 8: editScheduleButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 9: settingsButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 10: dayButtonPressed((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 11: dayButtonReleased((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 12: statsFileChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: showTime(); break;
        case 14: returnPressed(); break;
        case 15: screenSaver(); break;
        case 16: changeSize(); break;
        case 17: idleClicked(); break;
        case 18: restartTimer(); break;
        case 19: upPressed(); break;
        case 20: upReleased(); break;
        case 21: downPressed(); break;
        case 22: downReleased(); break;
        case 23: upNumPressed(); break;
        case 24: upNumReleased(); break;
        case 25: downNumPressed(); break;
        case 26: downNumReleased(); break;
        case 27: backReleased(); break;
        case 28: nextReleased(); break;
        case 29: buttonReleased(); break;
        case 30: prefsReleased(); break;
        case 31: profileReleased(); break;
        case 32: setupReleased(); break;
        case 33: keyBackReleased(); break;
        case 34: cancelReleased(); break;
        case 35: editReleased(); break;
        case 36: calendarChanged(); break;
        case 37: doneReleased(); break;
        case 38: saveReleased(); break;
        case 39: dateChanged((*reinterpret_cast< QDate(*)>(_a[1]))); break;
        case 40: timeChanged((*reinterpret_cast< QTime(*)>(_a[1]))); break;
        case 41: ssBoxChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 42;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
