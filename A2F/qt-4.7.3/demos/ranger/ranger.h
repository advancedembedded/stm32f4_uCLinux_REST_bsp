#ifndef RANGER_H
#define RANGER_H

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QtNetwork>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#else
#include <QtGui>
#endif

QT_BEGIN_NAMESPACE
class QCalendarWidget;
class QDate;
class QTime;
class QDateEdit;
class QGroupBox;
class QLabel;
QT_END_NAMESPACE

#include "rangercommon.h"
#include "dailyschedule.h"
#include "thermostat.h"
#include "roundedpolygon.h"
#include "schedule.h"

class Ranger : public RangerCommon
{
    Q_OBJECT

public:
    Ranger(QWidget *parent = 0);
    ~Ranger();
    void showWindow();
    void loadTestData();
    void drawGrid(QWidget *widget);
    DailySchedule currentDailySchedule;
    void setRangerSheet();

private:
    void readConfig(QString);
    void readStats(QString);
    void saveConfig(QString);
    void saveStats(QString);
    void setColorScheme(QString);
    void putConfig();
    void getConfig();
    void updateConfig();
    void setPropertyNames(QString);
    void setEditRoomHeader(QString);
    void thermostatLocations();
    void loadSchedules();
    void loadDailySchedules();

    QGroupBox *createMainPanel();
    QGroupBox *createIdlePanel();
    QGroupBox *createDailySchedulePanel();
    QGroupBox *createSelectModePanel();
    QGroupBox *createFanModePanel();
    QGroupBox *createHeatPanel();
    QGroupBox *createCoolPanel();
    QGroupBox *createEditSchedulePanel();
    QGroupBox *createRegisterPanel();
    QGroupBox *createSettingsPanel();
    QGroupBox *createColorsPanel();
    QGroupBox *createThermostatLocationPanel();
    QGroupBox *createSettingsSchedulePanel();
    QGroupBox *createEditThermoNamePanel();
    QGroupBox *createDeleteThermoPanel();
    QGroupBox *createEditRoomPanel();
    QGroupBox *createLanguagesPanel();
    QGroupBox *createLockPanel();
    QGroupBox *createProfilePanel();
    QGroupBox *createTimeDatePanel();
    QGroupBox *createPreferencesPanel();
    QGroupBox *createSchedulingOptionsPanel();
    QGroupBox *createSetupPanel();
    QGroupBox *createSystemTypePanel();
    QGroupBox *createThermostatControlsPanel();
    QGroupBox *createTempuratureOptionsPanel();
    QGroupBox *createTempuratureScalePanel();
    QGroupBox *createClockFormatPanel();
    QGroupBox *createDstPanel();
    QGroupBox *createTempuratureLimitsPanel();
    QGroupBox *createSchedulePeriodsPanel();
    QGroupBox *createKeyboardLockoutPanel();
    QGroupBox *createSytemsChangeOverPanel();
    QGroupBox *createWifiPanel();
    QGroupBox *createWifiSetupPanel();
    QGroupBox *createWifiPasswordPanel();
    QGroupBox *createConnectionSuccesfulPanel();

    QGroupBox *createFanControlPanel();
    QGroupBox *createBackupHeatPanel();
    QGroupBox *createCoolingStagesPanel();
    QGroupBox *createHeatingStagesPanel();
    QGroupBox *createChangeOverValvePanel();
    QGroupBox *createDisconnectPanel();
    QGroupBox *createLockErrorPanel();

    QLabel *editScehduleHeaderLabel;
    QPushButton *weekButton;

    QPushButton *schedText;

    QNetworkAccessManager *networkManager ;
    QNetworkReply *configReply;

    void createCancelDone(QString prop, QGroupBox *box);
    void createCancelSave(QString prop, QGroupBox *box);

    QGroupBox *createNumericKeyboard();
    QGroupBox *createAlphaKeyboard();

    QList<Schedule> schedules;
    QMap<QString, Thermostat> thermostats;

    QMap<QString, DailySchedule> selectedDaySchedule;
    QMap<QString, QMap<QString, DailySchedule> > dailySchedules;
    QMap<QString, DailySchedule>  currentSchedule;

    bool addNewLocation;
    int insideHumidity;
    int insideTemp;
    int outsideHumidity;
    int outsideTemp;
    int currentIndex;

    QLineEdit *firstNameEdit;
    QString grayButtonStyle;
    QString upButtonGreenStyle;
    QString upButtonGreenPressedStyle;
    QString downButtonGreenPressedStyle;
    QString downButtonGreenStyle;
    QString upButtonGrayStyle;
    QString downButtonGrayStyle;
    QString greenButtonStyle;
    QString redButtonStyle;
    QString headerButtonStyle;
    QString keyboardText;
    QString keyboardEditText;

    QString customerNameText;
    QString firstNameText;
    QString thermostatMACText;
    QString zipCodeText;
    QString wifiPasswordText;
    QString thermostatIpText;
    QString thermostatStatusText;
    QString thermostatCRCText;
    QString wifiNetworkText;

    QString thermoNameText;

    QPushButton *alphaKeyButtons[30];
    QPushButton *numericKeyButtons[30];

    QPushButton *alphaCancel;
    QPushButton *alphaDone;
    QPushButton *alphaBack;
    QPushButton *numericCancel;
    QPushButton *numericDone;
    QPushButton *numericBack;

    QLineEdit *keyboardEdit;
    QLineEdit *thermoName;
    QLineEdit *firstName;
    QString lastName;

    QLabel *registerHeaderLabel;
    QLabel *editRoomHeaderLabel;
    QString configFile;
    QString statsFile;

    //QFileSystemWatcher *watcher;

    QGroupBox *mainPanel;
    QGroupBox *idlePanel;
    QGroupBox *schedulePanel;
    QGroupBox *alphaKeyboardPanel;
    QGroupBox *numericKeyboardPanel;
    QGroupBox *selectModePanel;
    QGroupBox *fanModePanel;
    QGroupBox *heatPanel;
    QGroupBox *coolPanel;
    QGroupBox *editSchedulePanel;
    QGroupBox *registerPanel;
    QGroupBox *settingsPanel;
    QGroupBox *setupPanel;
    QGroupBox *colorsPanel;
    QGroupBox *thermostatLocationPanel;
    QGroupBox *settingsSchedulePanel;
    QGroupBox *editRoomPanel;
    QGroupBox *editThermoNamePanel;
    QGroupBox *deleteThermoPanel;
    QGroupBox *tempuraturePanel;
    QGroupBox *languagesPanel;
    QGroupBox *lockPanel;
    QGroupBox *profilePanel;
    QGroupBox *timeDatePanel;
    QGroupBox *preferencesPanel;
    QGroupBox *dailySchedulePanel;
    QGroupBox *currentPanel;
    QGroupBox *schedulingOptionsPanel;
    QGroupBox *hvacTypePanel;
    QGroupBox *tempuratureScalePanel;
    QGroupBox *systemsChangeOverPanel;
    QGroupBox *clockFormatPanel;
    QGroupBox *dstPanel;
    QGroupBox *tempuratureLimitsPanel;
    QGroupBox *schedulePeriodsPanel;
    QGroupBox *keyboardLockoutPanel;
    QGroupBox *wifiPanel;
    QGroupBox *wifiSetupPanel;
    QGroupBox *wifiPasswordPanel;
    QGroupBox *connectionSuccesfulPanel;
    QGroupBox *disconnectPanel;

    QGroupBox *thermostatControlsPanel;
    QGroupBox *fanControlPanel;
    QGroupBox *backupHeatPanel;
    QGroupBox *coolingStagesPanel;
    QGroupBox *heatingStagesPanel;
    QGroupBox *changeOverValvePanel;
    QGroupBox *lockErrorPanel;

    QLabel *dayLabel;
    QLabel *timeLabel;
    QLabel *outsideLabel;

    QLabel *heatToDegreesLabel;
    QLabel *heatToTimeLabel;
    QLabel *coolToDegreesLabel;
    QLabel *coolToTimeLabel;
    QLabel *coolToAmPmLabel;
    QLabel *heatToAmPmLabel;
    QLabel *tempuratureLabel;
    QLabel *locationLabel;
    QLabel *humidityLabel;
    QLabel *modeLabel;
    QLabel *fanLabel;
    QLabel *holdLabel;
    QLabel *heatLabel;
    QLabel *coolLabel;
    QLabel *tempLabel;

    QLabel *heatToLabel;
    QLabel *coolToLabel;
    QLabel *startLabel;
    QLabel *stopLabel;
    QLabel *periodLabel;

    QLabel *dailyHeatToLabel[4];
    QLabel *dailyCoolToLabel[4];
    QLabel *dailyTimeLabel[4];
    QLabel *dailyPeriodLabel[4];


    QLineEdit *lockNums[4];
    QLineEdit *minuteText;
    QLineEdit *hourText;
    QLineEdit *ampmText;

    QLineEdit *monthText;
    QLineEdit *dayText;
    QLineEdit *yearText;

    QPushButton *upButton;
    QPushButton *dnButton;
    QPushButton *setupButton;
    QPushButton *modeButton;
    QPushButton *fanButton;
    QPushButton *holdButton;
    QPushButton *heatButton;
    QPushButton *coolButton;
    QPushButton *dayButtons;
    QPushButton *selectModeButtons;
    QPushButton *fanModeButtons;
    QPushButton *fanControlButtons;
    QPushButton *systemTypeButtons;
    QPushButton *schedulingOptionsButtons;
    QPushButton *backupHeatButtons;
    QPushButton *coolingStagesButtons;
    QPushButton *heatingStagesButtons;
    QPushButton *clockFormatButtons;
    QPushButton *dstButtons;
    QPushButton *tempuratureScaleButtons;
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
    QPushButton *abcButton;
    QPushButton *numberButton;
    QPushButton *fields;

public slots:
    void procStyler(QString);
    void loadConfig(QNetworkReply *reply);
    void mainButtonReleased(QAbstractButton* button);
    void modeButtonReleased(QAbstractButton* button);
    void fanButtonReleased(QAbstractButton* button);
    void colorButtonReleased(QAbstractButton* button);
    // NEW STUFF
    void fanControlReleased(QAbstractButton* button);
    void systemTypeReleased(QAbstractButton* button);
    void languagesReleased(QAbstractButton* button);
    void schedulingOptionsReleased(QAbstractButton* button);
    void clockFormatReleased(QAbstractButton* button);
    void dstReleased(QAbstractButton* button);
    void systemsChangeOverReleased(QAbstractButton* button);
    void keyboardLockoutReleased(QAbstractButton* button);
    void schedulePeriodsReleased(QAbstractButton* button);
    void backupHeatReleased(QAbstractButton* button);
    void coolingStagesReleased(QAbstractButton* button);
    void heatingStagesReleased(QAbstractButton* button);
    void tempuratureScaleReleased(QAbstractButton* button);
//    void iconsReleased(QAbstractButton* button);

    void keyButtonReleased(QAbstractButton* button);
    void scheduleButtonReleased();
    void locationButtonReleased(QAbstractButton* button);
    void editScheduleButtonReleased(QAbstractButton* button);
    void settingsButtonReleased(QAbstractButton* button);
    void dayButtonPressed(QAbstractButton* button);
    void dayButtonReleased(QAbstractButton* button);

    void statsFileChanged(QString);
    void showTime();
    void returnPressed();
    void screenSaver();

    void changeSize();
    void idleClicked();
    void restartTimer();

    void upPressed();
    void upReleased();
    void downPressed();
    void downReleased();

    void upNumPressed();
    void upNumReleased();
    void downNumPressed();
    void downNumReleased();

    void backReleased();
    void nextReleased();

    void buttonReleased();
    void prefsReleased();
    void profileReleased();
    void setupReleased();
    void keyBackReleased();
    void cancelReleased();
    void editReleased();
    void calendarChanged();
    void doneReleased();
    void saveReleased();
    void dateChanged(QDate);
    void timeChanged(QTime);
    void ssBoxChanged(QString);

signals:

};

#endif // RANGER_H
