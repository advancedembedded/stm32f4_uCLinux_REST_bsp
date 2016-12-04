#ifndef RANGERCOMMON_H
#define RANGERCOMMON_H

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include "roundedpolygon.h"
#include "rangertheme.h"
#include "hvacconfig.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#else
#include <QtGui>
#include "qjson/qjson_p.h"
#endif

class RangerCommon : public QMainWindow
{
    Q_OBJECT

public:
    explicit RangerCommon(QWidget *parent = 0);
    void buildThemes();
    void initialize();
    void buildButtonIcons();

    int screenSaverCount;
    int windowXpos;
    int windowYpos;
    int windowWidth;
    int windowHeight;
    float windowSize;
    bool shiftOn;
    int firstTime;
    bool screenLocked;

    int screenSaverTime;
    int statControl;
    int prefsPage;

    QTimer *screenTimer;
    QString selectedMode;
    QString selectedModeTemp;
    QString selectedFanMode;
    QString selectedFanModeTemp;
    QString selectedFanControlTemp;
    bool holdMode;
    QString lockCode;
    QString serialNumber;
    QString dateTime;
    QString configVersion;

    QJsonDocument configJson;
    QJsonDocument statJson;
    QJsonObject configObject;
    QJsonObject statsObject;

    QDateTime currentDateTime;
    QDate currentDate;
    QTime currentTime;

    int currentMonth;
    int currentDay;
    int currentYear;
    int currentHour;
    int currentMinute;
    QString currentAmPm;

    QStringList months;

    int coolToDegrees;
    int coolToHour;
    int coolToMinute;
    QString coolToAmPm;
    int heatToDegrees;
    int heatToHour;
    int heatToMinute;
    QString heatToAmPm;

    int coolToDegreesTemp;
    int heatToDegreesTemp;

    int heatToHourTemp;
    int heatToMinuteTemp;
    QString heatToAmPmTemp;

    int coolToHourTemp;
    int coolToMinuteTemp;
    QString coolToAmPmTemp;

    QStringList periodList;
    QStringList scheduleList;

    int periodCounter;
    int lockCodes[4];

    QCheckBox *coolingBox;
    QCheckBox *heatingBox;
    QString heatToTimeText;
    QString selectedSchedule;
    QString coolToTimeText;
    QString selectedDayTemp;
    QString selectedDay;
    QString selectedRoom;
    QString degreeCode;

    QString currFwVersion;
    bool enableSchedule;
    int filterChangeDate;
    int filterLifeInDays;
    int localHumidity;
    int localTemp;
    QString nextFwVersion;
    QString securityMode;
    int tempSetPoint;
    int timeZoneOffset;
    bool unitLocked;
    bool resetUnit;


    QString thermostatMAC;
    QString thermostatIp;
    QString thermostatStatus;
    QString thermostatCRC;
    QString wifiNetwork;

    int schedulePeriods;
    QString keyboardLockout;
    QString tempuratureLimits;
    QString systemsChangeOver;
    bool dst;
    int clockFormat;
    bool tempuratureScale;
    QString schedulingOption;
    HvacConfig hvacConfig;

    QString hvacType;
    int thermostatControls;
    QString fanControl;
    QString backupHeat;
    int coolingStages;
    int heatingStages;
    QString language;
    QString changeOverValve;
    QAbstractButton *saveShift;
    QMap<QString, RangerTheme> rangerThemes;

    QString currentScheme;
    QString currentSchemeTemp;
    QString rangerStyleSheet;

    QString lightBlueStyle;
    QString blueStyle;
    QString pinkStyle;
    QString greenStyle;
    QString grayStyle;
    QString yellowStyle;
    QString redStyle;
    QString plainGrayStyle;
    QString grayBarStyle;
    QMap<QString, QString> buttonStyles;
    QString offButton;

    QIcon *upIcon;
    QIcon *dnIcon;
    QIcon *upIconPressed;
    QIcon *dnIconPressed;

    QIcon *upNumIcon;
    QIcon *dnNumIcon;
    QIcon *upNumIconPressed;
    QIcon *dnNumIconPressed;

    QIcon *leftIcon;
    QIcon *rightIcon;
    QIcon *leftIconPressed;
    QIcon *rightIconPressed;

    RoundedPolygon trianglePoly;
    RoundedPolygon dnPoly;
    RoundedPolygon leftPoly;
    RoundedPolygon rightPoly;

    RoundedPolygon upNumPoly;
    RoundedPolygon dnNumPoly;

    QFont labelFont;
    QFont headerFont;

    QGroupBox *backgroundBox();
    QGroupBox *headerBox;

    QLabel *addHeaderLabel(QString lbl, QWidget *parent);
    QLabel *addHeaderLabel(QString lbl, int x, int y, QWidget *parent);
    QLabel *addLabel(QString lbl, int x, int y, QWidget *parent);
    QLabel *addLabels(QString lbl, int x, int y, QString s, QWidget *parent);
    QLineEdit *addText(QString lbl, int x, int y, QString s, QWidget *parent);

    QPushButton *addButton(QString lbl, int x, int y, QWidget *parent);
    QCheckBox *addCheckBox(QString lbl, int x, int y, QWidget *parent);
    QPushButton *addImageButton(QString image, int x, int y, float sc,QWidget *parent);
    QPushButton *addBaseButton(QString lbl, int x, int y, QWidget *parent);
    QPushButton *addUpButton(int x, int y, QString outline, QWidget *parent);
    QPushButton *addDownButton(int x, int y, QString outline, QWidget *parent);
    QPushButton *addUpNumButton(int x, int y, QString outline, QWidget *parent);
    QPushButton *addDownNumButton(int x, int y, QString outline, QWidget *parent);
    QPushButton *addLeftButton(int x, int y, QString outline, QWidget *parent);
    QPushButton *addRightButton(int x, int y, QString outline, QWidget *parent);
    QPushButton *getPolyButton(float rot, float scale, QString color, QWidget *parent);

    QIcon *getButtonIcon(float rot, float scale, QString color);
    QPixmap getPixmap(float rot, float scale, QString color);
    QPushButton *tempuratureScaleButtons;

    QPushButton *fanModeButtons;
    QPushButton *fanControlButtons;
    QPushButton *systemTypeButtons;
    QPushButton *schedulingOptionsButtons;
    QPushButton *backupHeatButtons;
    QPushButton *coolingStagesButtons;
    QPushButton *heatingStagesButtons;
    QPushButton *clockFormatButtons;
    QPushButton *dstButtons;
    QPushButton *systemsChangeOverButtons;
    QPushButton *keyboardLockoutButtons;
    QPushButton *schedulePeriodsButtons;
    QPushButton *languagesButtons;
    QPushButton *scheduleButtons;
    QPushButton *editScheduleButtons;
    QPushButton *colorButtons;
    QPushButton *iconsButtons;
    QPushButton *dailyButtons;
    QPushButton *locationButtons;

public slots:
//    void tempuratureScaleReleased(QAbstractButton* button);
//    void restartTimer();
//    void fanControlReleased(QAbstractButton* button);
//    void systemTypeReleased(QAbstractButton* button);
//    void languagesReleased(QAbstractButton* button);
//    void schedulingOptionsReleased(QAbstractButton* button);
//    void clockFormatReleased(QAbstractButton* button);
//    void dstReleased(QAbstractButton* button);
//    void systemsChangeOverReleased(QAbstractButton* button);
//    void keyboardLockoutReleased(QAbstractButton* button);
//    void schedulePeriodsReleased(QAbstractButton* button);
//    void backupHeatReleased(QAbstractButton* button);
//    void coolingStagesReleased(QAbstractButton* button);
//    void heatingStagesReleased(QAbstractButton* button);
//    void fanButtonReleased(QAbstractButton* button);
//    void colorButtonReleased(QAbstractButton* button);

signals:

};

#endif // RANGERCOMMON_H
