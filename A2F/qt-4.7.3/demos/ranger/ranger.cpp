#include "ranger.h"

QString appPath;
//QTimer *screenTimer;

Ranger::Ranger(QWidget *parent) : RangerCommon(parent)
{
#ifdef Q_OS_WIN
    configFile = appPath + "/../../ranger/config.json";
    statsFile = appPath + "/../../ranger/stats.json";
#else
    configFile = appPath + "/../ranger/config.json";
    statsFile = appPath + "/../ranger/stats.json";
#endif

    initialize();

    readConfig(configFile);
    readStats(statsFile);

//    watcher = new QFileSystemWatcher();
//    watcher->addPath(statsFile);

//    QObject::connect(watcher, SIGNAL(fileChanged(QString)), this,
//                     SLOT(statsFileChanged(QString)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(60000);

//    screenTimer = new QTimer(this);
//    connect(screenTimer, SIGNAL(timeout()), this, SLOT(screenSaver()));
//    screenTimer->start(60000);

    setColorScheme(currentScheme);

//    RangerCommon *xxx = new RangerCommon();
//    connect(xxx, SIGNAL(tst(QString)),
//                     this, SLOT(procStyler(QString)));

    buildThemes();
    buildButtonIcons();
    //getConfig();
    //putConfig();
}

Ranger::~Ranger()
{}

void Ranger::procStyler(QString s) {
    //qDebug() << "PROC"<<s;
}

void Ranger::restartTimer() {
    //screenTimer->start(60000);
}

void Ranger::showWindow()
{
    windowXpos = 0;
    windowYpos = 0;
    windowWidth = 480*windowSize;
    windowHeight = 272*windowSize;

    setFixedSize(windowWidth, windowHeight);

    loadTestData();

    customerNameText = "Frank";
    thermostatMACText = "00:50:56:8A:E3:C5";
    zipCodeText = "123456";
    thermostatIpText = "192.168.1.30";
    thermostatStatusText = "Working";
    thermostatCRCText ="3453";
    wifiNetworkText = "My Wifi Network";

    mainPanel = createMainPanel();
    selectModePanel = createSelectModePanel();
    fanModePanel = createFanModePanel();
    heatPanel = createHeatPanel();
    coolPanel = createCoolPanel();
    disconnectPanel = createDisconnectPanel();
    idlePanel = createIdlePanel();
    lockPanel = createLockPanel();

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(14*windowSize);
    registerPanel = createRegisterPanel();
    editThermoNamePanel = createEditThermoNamePanel();
    wifiPasswordPanel = createWifiPasswordPanel();
    wifiSetupPanel = createWifiSetupPanel();
    alphaKeyboardPanel = createAlphaKeyboard();
    numericKeyboardPanel = createNumericKeyboard();

    settingsPanel = createSettingsPanel();
    timeDatePanel = createTimeDatePanel();
    colorsPanel = createColorsPanel();
    preferencesPanel = createPreferencesPanel();
    schedulingOptionsPanel = createSchedulingOptionsPanel();
    tempuratureScalePanel = createTempuratureScalePanel();
    clockFormatPanel = createClockFormatPanel();
    dstPanel = createDstPanel();
    systemsChangeOverPanel = createSytemsChangeOverPanel();
    keyboardLockoutPanel = createKeyboardLockoutPanel();
    schedulePeriodsPanel = createSchedulePeriodsPanel();
    profilePanel = createProfilePanel();

    tempuratureLimitsPanel = createTempuratureLimitsPanel();
    settingsSchedulePanel = createSettingsSchedulePanel();
    editSchedulePanel = createEditSchedulePanel();
    dailySchedulePanel = createDailySchedulePanel();
    setupPanel = createSetupPanel();
    fanControlPanel = createFanControlPanel();
    backupHeatPanel = createBackupHeatPanel();
    coolingStagesPanel = createCoolingStagesPanel();
    heatingStagesPanel = createHeatingStagesPanel();
    languagesPanel = createLanguagesPanel();
    thermostatLocationPanel = createThermostatLocationPanel();
    thermostatControlsPanel = createThermostatControlsPanel();
    editRoomPanel = createEditRoomPanel();
    hvacTypePanel = createSystemTypePanel();
    wifiPanel = createWifiPanel();
    connectionSuccesfulPanel = createConnectionSuccesfulPanel();

    labelFont.setPointSize(14*windowSize);
    labelFont.setWeight(QFont::Normal);

    labelFont.setPointSize(13*windowSize);
    labelFont.setWeight(QFont::Normal);
    keyboardEdit = new QLineEdit(this);
    keyboardEdit->setText("");
    keyboardEdit->move(10*windowSize, 56*windowSize);
    keyboardEdit->setFixedSize(460*windowSize, 32*windowSize);
    keyboardEdit->setAlignment(Qt::AlignHCenter);
    keyboardEdit->setFont(labelFont);
    keyboardEdit->setReadOnly(true);
    keyboardEdit->setDisabled(true);
    keyboardEdit->setStyleSheet("border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;");
    keyboardEdit->setCursorPosition(0);
    keyboardEdit->end(false);
    keyboardEdit->setFocus();
    keyboardEdit->setVisible(false);

    thermostatLocations();
    firstTime = 0;
    if (firstTime) {
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else {
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
   }

   show();
}

void Ranger::screenSaver() {
    //qDebug() <<"screenSaver"<<screenSaverCount;
    if (screenSaverTime == 0 || firstTime == 1) {
        return;
    }
    screenSaverCount++;
    if (screenSaverCount >= screenSaverTime) {
        currentPanel->setVisible(false);
        //idlePanel = createIdlePanel();
        idlePanel->setVisible(true);
        screenTimer->stop();
        screenSaverCount = 0;
    }
}

void Ranger::showTime() {
    if (mainPanel != NULL) {
        currentDate = QDate::currentDate();
        currentTime = QTime::currentTime();

        dayLabel->setText(currentDate.toString("ddd M/d/yy"));
        timeLabel->setText(currentTime.toString("h:mm A"));
    }
}

void Ranger::changeSize() {
    //qDebug() << "changeSize";

    if (windowSize == 1) {
        windowSize = 2;
    } else {
        windowSize = 1;
    }
    windowWidth = 480*windowSize;
    windowHeight = 270*windowSize;

    buildThemes();
    buildButtonIcons();

    headerFont.setPointSize(17*windowSize);
    labelFont.setPointSize(11*windowSize);

    setFixedSize(windowWidth, windowHeight);

    mainPanel->setVisible(false);
    mainPanel = createMainPanel();
    mainPanel->setVisible(true);
    saveConfig(statsFile);
}

void Ranger::statsFileChanged(QString file) {
    //qDebug() << "Changed: " << file ;
    readStats(statsFile);
    outsideLabel->setText(QString::number(outsideTemp) + degreeCode.at(degreeCode.length()-1)
            + "  |  " + QString::number(outsideHumidity) + "%");
    humidityLabel->setText(QString::number(insideHumidity) + "% HUMIDITY");
//    if (props["heatOn"] == "1") {
//        heatButton->setObjectName("heat");
//    } else {
//        heatButton->setObjectName("");
//    }
//    if (props["coolOn"] == "1") {
//        coolButton->setObjectName("cool");
//    } else {
//        coolButton->setObjectName("");
//    }
}

void Ranger::readConfig(QString path) {
    //qDebug() << "readConfig:"<<path;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qDebug() << "Using default config";
        file.setFileName(":/config_def.json");
        firstTime = 1;
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(0, "error", file.errorString());
            return;
        }
    }

    QString js = file.readAll();
    file.close();

    configJson = QJsonDocument::fromJson(js.toUtf8());
    configObject = configJson.object();
    configVersion = configObject["configVersion"].toString();
    serialNumber = configObject["serial"].toString();
    dateTime =  (int)configObject["epochTime"].toDouble();
    currentScheme = configObject["themeScheme"].toString();
    statControl = (int)configObject["tempSetPoint"].toDouble();
    holdMode = configObject["tempHold"].toBool();
    selectedFanMode = configObject["fanMode"].toString();
    selectedMode = configObject["hvacMode"].toString();
    language = configObject["language"].toString();
    tempuratureScale = configObject["metric"].toBool();
    lockCode = configObject["lockCode"].toString();
    firstNameText = configObject["ownersName"].toString();
    dst = configObject["dst"].toBool();
    keyboardLockout = configObject["keyboardLock"].toString();
    selectedSchedule = configObject["selectedSchedule"].toString();
    currFwVersion = configObject["currFwVersion"].toString();
    enableSchedule = configObject["enableSchedule"].toBool();
    filterChangeDate = (int)configObject["filterChangeDate"].toDouble();
    filterLifeInDays = (int)configObject["filterLifeInDays"].toDouble();
    localHumidity = (int)configObject["localHumidity"].toDouble();
    localTemp = (int)configObject["localTemp"].toDouble();
    nextFwVersion = configObject["nextFwVersion"].toString();
    securityMode = configObject["securityMode"].toString();
    tempSetPoint = (int)configObject["tempSetPoint"].toDouble();
    timeZoneOffset = (int)configObject["timeZoneOffset"].toDouble();
    unitLocked = configObject["unitLocked"].toBool();
    resetUnit = configObject["reset"].toBool();

    heatToDegrees = (int)configObject["heatToDegrees"].toDouble();
    coolToDegrees = (int)configObject["coolToDegrees"].toDouble();
    schedulingOption = configObject["schedulingOption"].toString();
    clockFormat = (int)configObject["clockFormat"].toDouble();
    systemsChangeOver = configObject["changeOver"].toString();
    schedulePeriods = (int)configObject["schedulePeriods"].toDouble();
    fanControl = configObject["fanControl"].toString();
    thermostatControls = (int)configObject["thermoControls"].toDouble();
//    coolToTime = (int)configObject["coolToTime"].toDouble();
//    heatToTime = (int)configObject["heatToTime"].toDouble();
//    heatToTimeText = QDateTime::fromTime_t(heatToTime).toString("h:mm A");
//    coolToTimeText = QDateTime::fromTime_t(coolToTime).toString("h:mm A");

    heatToHour = 12;
    heatToMinute = 0;
    coolToHour = 12;
    coolToMinute = 0;
    coolToAmPm = "AM";
    heatToAmPm = "AM";

    QJsonObject hvac = configObject["hvacConfig"].toObject();
    hvacConfig.setBackupHeatingType(hvac["backupHeatingType"].toString());
    hvacConfig.setCoolingStages((int)hvac["coolingStages"].toDouble());
    hvacConfig.setHeatingStages((int)hvac["heatingStages"].toDouble());
    hvacConfig.setHvacType(hvac["hvacType"].toString());

    backupHeat = hvacConfig.getBackupHeatingType();
    coolingStages = hvacConfig.getCoolingStages();
    heatingStages = hvacConfig.getHeatingStages();
    hvacType = hvacConfig.getHvacType();

    QJsonArray scheduleArray = configObject["schedules"].toArray();
    for (int i = 0; i < scheduleArray.size(); ++i) {
        QJsonObject so = scheduleArray[i].toObject();
        Schedule sched;
        sched.fromJson(so);
        schedules.append(sched);
        scheduleList << sched.getLabel().toUpper();
    }

    QMap<QString, DailySchedule> schedulesMap;
    foreach (Schedule s, schedules) {
        ////qDebug() <<"S" << s.getLabel();
        QList<Day> days = s.getDays();
        foreach (Day day,days) {
            ////qDebug() <<"  D" << day.getLabel();
            QList<Period> pl = day.getPeriods();
            schedulesMap.clear();
            foreach (Period p,pl) {
                ////qDebug() <<"    P" << p.getLabel().toUpper()<<p.getCoolTemp()<<p.getStartTime();
                DailySchedule ds;
                ds.setIndex(0);
                ds.setPeriod(p.getLabel().toUpper());
                ds.setCoolTo(p.getCoolTemp());
                ds.setHeatTo(p.getHeatTemp());
                ds.setStartTime(p.getStartTime());
                ds.setStopTime(p.getStopTime());
                schedulesMap.insert(p.getLabel().toUpper(), ds);
            }
            dailySchedules.insert(day.getLabel().toUpper(), schedulesMap);
        }
    }

    selectedDay = selectedSchedule.toUpper();
    selectedDaySchedule = dailySchedules[selectedDay];
}

void Ranger::readStats(QString path) {
     QFile file(path);
     if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QMessageBox::information(0, "error", file.errorString());
         return;
     }

     QString js = file.readAll();
     file.close();

     statJson = QJsonDocument::fromJson(js.toUtf8());
     statsObject = statJson.object();

     outsideTemp = (int)statsObject["temp_out"].toDouble();
     outsideHumidity = (int)statsObject["hum_out"].toDouble();
     insideTemp = (int)statsObject["temp_in"].toDouble();
     insideHumidity = (int)statsObject["hum_in"].toDouble();
}

void Ranger::updateConfig() {
    configObject["fanControl"] = fanControl;
    configObject["metric"] = tempuratureScale;
    configObject["clockFormat"] = clockFormat;
    configObject["dst"] = dst;
    configObject["keyboardLock"] = keyboardLockout;
    configObject["schedulePeriods"] = schedulePeriods;
    configObject["schedulingOption"] = schedulingOption;
    configObject["selectedSchedule"] = selectedSchedule;
    configObject["thermoControls"] = thermostatControls;
    configObject["lockCode"] = lockCode;
    configObject["firstName"] = customerNameText;
    configObject["zipCode"] = zipCodeText;
    configObject["coolToDegrees"] = coolToDegrees;
//    configObject["coolToTime"] = coolToTime;
    configObject["heatToDegrees"] = heatToDegrees;
//    configObject["heatToTime"] = heatToTime;
    configObject["schedule"] = selectedSchedule;
    configObject["hvacMode"] = selectedMode;
    configObject["tempHold"] = holdMode;
    configObject["tempSetPoint"] = statControl;
    configObject["language"] = language;
    configObject["themeScheme"] = currentScheme;

    hvacConfig.setBackupHeatingType(backupHeat);
    hvacConfig.setCoolingStages(coolingStages);
    hvacConfig.setHeatingStages(heatingStages);
    hvacConfig.setHvacType(hvacType);
}

void Ranger::saveConfig(QString path) {
    return;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QFile::Truncate)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    updateConfig();

    QJsonObject hvac;
    hvac["backupHeatingType"] = hvacConfig.getBackupHeatingType();
    hvac["coolingStages"] = hvacConfig.getCoolingStages();
    hvac["heatingStages"] = hvacConfig.getHeatingStages();
    hvac["hvacType"] = hvacConfig.getHvacType();
    configObject["hvacConfig"] = hvac;

    QList<Schedule> scheds;

    foreach (Schedule s, schedules) {
        QList<Day> days = s.getDays();
        QList<Day> dayst;
        foreach (Day day,days) {
            ////qDebug() <<"  D"<<day.getLabel().toUpper();
            QMap<QString, DailySchedule> daySched = dailySchedules[day.getLabel().toUpper()];
            QList<Period> pl = day.getPeriods();
            QList<Period> plt;
            foreach (Period p,pl) {
                DailySchedule ds = daySched[p.getLabel().toUpper()];
                ////qDebug() <<"   P"<<p.getLabel().toUpper()<<ds.getPeriod()<<ds.getStartTime();
                p.setCoolTemp(ds.getCoolTo());
                p.setHeatTemp(ds.getHeatTo());
                p.setStartTime(ds.getStartTime());
                p.setStopTime(ds.getStopTime());
                plt << p;
            }
            day.setPeriods(plt);
            dayst << day;
        }
        s.setDays(dayst);
        scheds << s;
    }

    schedules = scheds;

    QJsonArray scheduleAerray;
    foreach (Schedule s, schedules) {
        QJsonObject so;
        s.toJson(so);
        scheduleAerray.append(so);
    }
    configObject["schedules"] = scheduleAerray;

    QJsonDocument saveDoc(configObject);
    file.write(saveDoc.toJson());

    file.close();
}

void Ranger::saveStats(QString path) {
    QFile jsonFile(":/config.json");
    if (jsonFile.open(QFile::WriteOnly | QFile::Truncate)) {
         QJsonDocument saveDoc(statsObject);
         jsonFile.write(saveDoc.toJson());

        jsonFile.close();
    }
}

QGroupBox *Ranger::createMainPanel() {
    QGroupBox *box = backgroundBox();

    labelFont.setPointSize(13*windowSize);
    int y = 15*windowSize;
    dayLabel = addLabel(currentDate.toString("ddd M/d/yy"), 10, y, box);
    dayLabel->setAlignment(Qt::AlignRight);
    dayLabel->setFixedWidth(140*windowSize);

    QLabel *bar = addLabel("|", 166*windowSize, y, box);
    bar->setFixedWidth(10*windowSize);

    QString ct = (clockFormat == 12) ? currentTime.toString("hh:mm A") : currentTime.toString("HH:mm");
    timeLabel = addLabel(ct, 190*windowSize,y, box);
    timeLabel->setAlignment(Qt::AlignLeft);
    timeLabel->setFixedWidth(70*windowSize);

    QPushButton *wifi = addImageButton("wifi3_"+currentScheme+".png", 310*windowSize,8*windowSize, .8, box);
    wifi->setFixedWidth(50*windowSize);

    connect(wifi , SIGNAL(clicked()), this,SLOT(changeSize()));
    labelFont.setPointSize(11*windowSize);
    outsideLabel = addLabel(QString::number(outsideTemp) + degreeCode.at(degreeCode.length()-1) +
            " | " + QString::number(outsideHumidity) + "%", 360*windowSize,y-7*windowSize, box);
    outsideLabel->setFixedWidth(120*windowSize);
    outsideLabel->setAlignment(Qt::AlignHCenter);

    labelFont.setWeight(QFont::Normal);
    labelFont.setPointSize(10*windowSize);
    QLabel *t = addLabel(tr("OUTSIDE"), 360*windowSize,y+11*windowSize, box);
    t->setFixedWidth(120*windowSize);
    t->setAlignment(Qt::AlignHCenter);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(11*windowSize);

    QPushButton *settingsButton = new QPushButton(tr("SETTINGS"), box);
    settingsButton->setProperty("button","settings");
    settingsButton->move(10*windowSize, 110*windowSize);
    settingsButton->setFixedWidth(115*windowSize);
    settingsButton->setFont(labelFont);

    connect(settingsButton , SIGNAL(released()), this,SLOT(buttonReleased()));

    labelFont.setWeight(QFont::DemiBold);
    locationLabel = new QLabel(tr("INDOOR"), box);
    locationLabel->move(180*windowSize,62*windowSize);
    locationLabel->setFont(labelFont);
    locationLabel->setStyleSheet("color: #808080;");

    tempuratureLabel = new QLabel(QString::number(insideTemp) , box);
    tempuratureLabel->setAlignment(Qt::AlignRight);
    tempuratureLabel->setFixedWidth(200*windowSize);

#ifdef Q_OS_WIN
    tempuratureLabel->setStyleSheet("color: #808080;font-weight:540;font-size:"+
                                    QString::number(120*windowSize)+"px;");
    tempuratureLabel->move(110*windowSize, 50*windowSize);
#else
    tempuratureLabel->setStyleSheet("color: #808080;font-size:"+
                                    QString::number(120*windowSize)+"px;");
    tempuratureLabel->move(110*windowSize, 55*windowSize);
#endif

    QLabel *deg = new QLabel(degreeCode.at(degreeCode.length()-1), box);
    deg->setStyleSheet("color: #757575;font-size:"+QString::number(40*windowSize)+"px;");

#ifdef Q_OS_WIN
    deg->move(305*windowSize, 70*windowSize);
#else
    deg->move(300*windowSize, 73*windowSize);
#endif

    humidityLabel = new QLabel(QString::number(insideHumidity) + "% HUMIDITY", box);
    humidityLabel->move(180*windowSize,176*windowSize);
    humidityLabel->setFont(labelFont);
    humidityLabel->setStyleSheet("color: #808080;");

    QPixmap pixmap(2*windowSize,100*windowSize);
    pixmap.fill(QColor("#e5e5e4"));
    QLabel *label = new QLabel(box);
    label->setPixmap(pixmap);
    label->move(350*windowSize, 80*windowSize);

//    QPushButton *up = new QPushButton("", box);
//    QPixmap pixmap1;
//    pixmap1 = QPixmap(":/images/up_blue.png");
//    up->setGeometry( 390*windowSize, 70*windowSize, 120,120);
//    up->setStyleSheet("outline: 2px;border: 2px solid black;");
//    up->setMask(pixmap1.mask());

    QPushButton *up = addUpButton(390*windowSize, 70*windowSize, "#ccc", box);
            //addImageButton("up_blue.png", 390*windowSize, 70*windowSize, 1, box);
    up->setProperty("button", "main");

    QPushButton *dn = addDownButton(390*windowSize, 145*windowSize, "#ccc", box);
    dn->setProperty("button", "main");

    QFontMetrics fm(labelFont);

    int xl = 370*windowSize;
    addLabels(tr("SET TO"), xl, 122*windowSize, "color:#808080;", box);
    tempLabel = addLabel(QString::number(statControl) + degreeCode.at(degreeCode.length()-1),
                         xl+fm.width("SET TO "), 122*windowSize,box);
    tempLabel->setStyleSheet("color:#808080;");
    tempLabel->setAlignment(Qt::AlignLeft);
    tempLabel->setFixedWidth(100*windowSize);

    connect(up , SIGNAL(pressed()), this, SLOT(upPressed()));
    connect(up , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dn , SIGNAL(pressed()), this, SLOT(downPressed()));
    connect(dn , SIGNAL(released()), this,SLOT(downReleased()));

    labelFont.setWeight(QFont::DemiBold);

    QGroupBox *statBox = new QGroupBox(box);
    statBox->setGeometry(0,201*windowSize,480*windowSize,22*windowSize);
    statBox->setObjectName("grayBar");

    int x = 5*windowSize;
    int xi = 96*windowSize;
    int w = 90*windowSize;
#ifdef Q_OS_WIN
    y = 205*windowSize;
#else
    y = 203*windowSize;
#endif
    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(9*windowSize);

    if (selectedMode == "off") {
        holdMode = false;
        modeLabel = addLabel(tr("OFF"), x, y, box);
    } else if (selectedMode == "auto") {
        modeLabel = addLabel(tr("AUTO"), x, y, box);
    } else if (selectedMode == "heat") {
        modeLabel = addLabel(tr("HEAT"), x, y, box);
    } else if (selectedMode == "cool") {
        modeLabel = addLabel(tr("COOL"), x, y, box);
    } else if (selectedMode == "emer_heat") {
        modeLabel = addLabel(tr("EMERG"), x, y, box);
    }
    modeLabel->setFixedWidth(w);
    modeLabel->setAlignment(Qt::AlignHCenter);

    if (selectedFanMode.toLower() == "auto") {
        fanLabel  = addLabel(tr("AUTO"), (x+=xi), y, box);
    } else if (selectedFanMode.toLower() == "on") {
        fanLabel  = addLabel(tr("ON"), (x+=xi), y, box);
    }
    fanLabel->setFixedWidth(w);
    fanLabel->setAlignment(Qt::AlignHCenter);

    if (!holdMode) {
        holdLabel = addLabel(tr("ON SCHED"), (x+=xi), y, box);
    } else if (holdMode) {
        holdLabel = addLabel(tr("ON HOLD"), (x+=xi), y, box);
    }
    holdLabel->setFixedWidth(w);
    holdLabel->setAlignment(Qt::AlignHCenter);

    heatLabel = addLabel(QString::number(heatToDegrees) + degreeCode.at(degreeCode.length()-1)
                         , (x+=xi), y, box);
    heatLabel->setFixedWidth(w);
    heatLabel->setAlignment(Qt::AlignHCenter);

    coolLabel = addLabel(QString::number(coolToDegrees) + degreeCode.at(degreeCode.length()-1)
                         , (x+=xi), y, box);
    coolLabel->setFixedWidth(w);
    coolLabel->setAlignment(Qt::AlignHCenter);

    x = 10*windowSize;
    y = 230*windowSize;
    w = 75*windowSize;

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    modeButton = addButton(tr("MODE"), x, y, box);
    modeButton->setFixedWidth(w);
    modeButton->setProperty("button","mode");
    modeButton->setDisabled(false);
    if (holdMode) {
        modeButton->setDisabled(true);
    }

    fanButton  = addButton(tr("FAN"), (x+=xi), y, box);
    fanButton->setFixedWidth(w);
    fanButton->setProperty("button","fan");
    fanButton->setDisabled(false);
    if (holdMode) {
         fanButton->setDisabled(true);
    }

    holdButton = addButton(tr("HOLD"), (x+=xi), y, box);
    holdButton->setFixedWidth(w);
    holdButton->setProperty("button","hold");
    if (!holdMode || selectedMode == "off") {
        holdButton->setObjectName("");
    } else {
        holdButton->setObjectName("red");
    }

    heatButton = addButton(tr("HEAT"), (x+=xi), y, box);
    heatButton->setFixedWidth(w);
    heatButton->setProperty("button","heat");
    heatButton->setDisabled(false);
    if (selectedMode == "heat") {
        heatButton->setObjectName("heat");
    } else {
        heatButton->setObjectName("");
    }

    if (holdMode) {
        heatButton->setDisabled(true);
    }

    coolButton = addButton(tr("COOL"), (x+=xi), y, box);
    coolButton->setFixedWidth(w);
    coolButton->setProperty("button","cool");
    coolButton->setDisabled(false);
    if (selectedMode == "cool") {
        coolButton->setObjectName("cool");
    } else {
        coolButton->setObjectName("");
    }
    if (holdMode) {
         coolButton->setDisabled(true);
    }

    QButtonGroup *bg = new QButtonGroup;
    bg->addButton(modeButton);
    bg->addButton(fanButton);
    bg->addButton(holdButton);
    bg->addButton(heatButton);
    bg->addButton(coolButton);

    connect(bg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(mainButtonReleased(QAbstractButton*)));

    box->setVisible(false);
    return box;
}

QGroupBox *Ranger::createIdlePanel() {
    QGroupBox *box = backgroundBox();

    int y = 15*windowSize;
    labelFont.setPointSize(12*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    dayLabel = addLabel(currentDate.toString("ddd M/d/yy"), 0,y, box);
    dayLabel->setAlignment(Qt::AlignRight);
    dayLabel->setFixedWidth(132*windowSize);

    QLabel *bar = addLabel("|", 148*windowSize, y, box);
    bar->setFixedWidth(10*windowSize);

    timeLabel = addLabel(currentTime.toString("hh:mm A"), 175*windowSize,y, box);
    timeLabel->setAlignment(Qt::AlignLeft);
    timeLabel->setFixedWidth(150*windowSize);

    labelFont.setPointSize(11*windowSize);
    outsideLabel = addLabel(QString::number(outsideTemp) + degreeCode.at(degreeCode.length()-1)
            +" | " + QString::number(outsideHumidity) + "%", 360*windowSize,y-7*windowSize, box);
    outsideLabel->setFixedWidth(120*windowSize);
    outsideLabel->setAlignment(Qt::AlignHCenter);

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::Normal);
    QLabel *t = addLabel(tr("OUTSIDE"), 360*windowSize,y+11*windowSize, box);
    t->setFixedWidth(120*windowSize);
    t->setAlignment(Qt::AlignHCenter);

    labelFont.setPointSize(14*windowSize);
    labelFont.setWeight(QFont::Black);
    locationLabel = new QLabel(tr("INDOOR"), box);
    locationLabel->move(190*windowSize,70*windowSize);
    locationLabel->setFont(labelFont);
    locationLabel->setStyleSheet("color: #aaa;");

    tempuratureLabel = new QLabel(QString::number(insideTemp) , box);
    tempuratureLabel->setAlignment(Qt::AlignRight);
    tempuratureLabel->setFixedWidth(230*windowSize);

#ifdef Q_OS_WIN
    tempuratureLabel->setStyleSheet("font-weight:520;color: #757575;font-size:"+
                                    QString::number(146*windowSize)+"px;");
    tempuratureLabel->move(110*windowSize, 60*windowSize);
#else
    tempuratureLabel->setStyleSheet("font-weight:520;color: #757575;font-size:"+
                                    QString::number(146*windowSize)+"px;");
    tempuratureLabel->move(120*windowSize, 72*windowSize);
#endif

    QLabel *deg = new QLabel(degreeCode.at(degreeCode.length()-1), box);
    deg->setStyleSheet("font-weight:bold;color: #757575;font-size:"+QString::number(40*windowSize)+"px;");

#ifdef Q_OS_WIN
    deg->move(340*windowSize, 86*windowSize);
#else
    deg->move(344*windowSize, 92*windowSize);
#endif

    humidityLabel = new QLabel(QString::number(insideHumidity) + "% HUMIDITY", box);
    humidityLabel->move(190*windowSize, 220*windowSize);
    humidityLabel->setFont(labelFont);
    humidityLabel->setStyleSheet("color: #aaa;");

    QPushButton *pu = new QPushButton(box);
    pu->setGeometry(windowXpos, windowYpos, windowWidth, windowHeight);
    pu->setStyleSheet("border:0 none;background-image:transparent;background-color:transparent;");

    connect(pu , SIGNAL(clicked()), this, SLOT(idleClicked()));

    box->setVisible(false);
    return box;
}

QGroupBox *Ranger::createRegisterPanel() {
    QGroupBox *box = backgroundBox();
    box->setVisible(false);

//    QFontMetrics f1(headerFont);
//    int w1 = f1.width("REGISTER:");
//    QLabel *reg = addHeaderLabel(tr("REGISTER:"), box);

//    headerFont.setWeight(QFont::DemiBold);
//    QFontMetrics f2(headerFont);
//    QString txt;

//    int w2 = f2.width(txt);
    registerHeaderLabel = addHeaderLabel("", box);

//    int xw = (windowWidth/2)-((w1+w2)/2);
//    reg->move(xw, 11*windowSize);
//    registerHeaderLabel->move(xw+w1 + 5*windowSize, 11*windowSize);

    return box;
}

QGroupBox *Ranger::createAlphaKeyboard() {
    QGroupBox *box =  new QGroupBox(this);
    box->setGeometry(windowXpos, 100*windowSize,
                     windowWidth, 190*windowSize);
    box->setStyleSheet("QGroupBox {border:0 none;}");
    box->setVisible(false);

    QStringList keys;
    keys << "qwertyuiop"
         << "asdfghjkl"
         << "zxcvbnm";

    labelFont.setPointSize(16*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    int x = 10*windowSize;
    int y = 0;
    int xi = 47*windowSize;
    int yi = 40*windowSize;
    int row = 0;
    int key = 0;
    for (int i=0;i<keys[row].length();i++) {
        QString s = shiftOn ? keys[row].at(i).toUpper() : keys[row].at(i);
        alphaKeyButtons[key] = addBaseButton(s, x, y, box);
        alphaKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    x = 34*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        QString s = shiftOn ? keys[row].at(i).toUpper() : keys[row].at(i);
        alphaKeyButtons[key] = addBaseButton(s, x, y, box);
        alphaKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    x = 82*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        QString s = shiftOn ? keys[row].at(i).toUpper() : keys[row].at(i);
        alphaKeyButtons[key] = addBaseButton(s, x, y, box);
        alphaKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    // SHIFT
    x = 10*windowSize;
    int ys = 32*windowSize;
    labelFont.setPointSize(11*windowSize);
    alphaKeyButtons[key] = new QPushButton("SHIFT", box);
    alphaKeyButtons[key]->move(x,y);
    alphaKeyButtons[key]->setFont(labelFont);
    alphaKeyButtons[key]->setProperty("button", keyboardEditText);
    if (shiftOn) {
        alphaKeyButtons[key]->setObjectName("keyshift");
    } else {
        alphaKeyButtons[key]->setObjectName("keyspecial");
    }
    alphaKeyButtons[key++]->setFixedSize(65*windowSize, ys);

    // left arrow key
    x = 410*windowSize;
    QPixmap pix(":images/backspace_" + rangerThemes["keyboard"].getTextColor() + ".png");
    QIcon qi = QIcon(pix);
    QSize qs(pix.rect().width()*(windowSize/2),pix.rect().height()*(windowSize/2));
    alphaBack  = addBaseButton("", x, y, box);
    alphaBack->setGeometry(x,y, 60*windowSize, ys);
    alphaBack->setIcon(qi);
    alphaBack->setIconSize(qs);
    alphaBack->setObjectName("keyspecial");

    connect(alphaBack , SIGNAL(released()), this,SLOT(keyBackReleased()));

    // SPACE bar
    y += yi;
    alphaKeyButtons[key] = addBaseButton("SPACE", 200*windowSize, y, box);
    alphaKeyButtons[key]->setObjectName("keyspecial");
    alphaKeyButtons[key++]->setGeometry(200*windowSize, y, 190*windowSize, ys);

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<key;j++) {
        cg->addButton(alphaKeyButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                this,SLOT(keyButtonReleased(QAbstractButton*)));

    alphaCancel = addBaseButton(tr("CANCEL"), 28*windowSize, y, box);
    //alphaCancel->setProperty("button", "BLOM");
    alphaCancel->setFixedSize(85*windowSize, ys);
    alphaCancel->setObjectName("keyspecial");

    numberButton = addBaseButton(".?123", 122*windowSize, y, box);
    numberButton->setProperty("button", "numButton");
    numberButton->setFixedSize(70*windowSize, ys);
    numberButton->setObjectName("keyspecial");
    connect(numberButton , SIGNAL(released()), this,SLOT(buttonReleased()));

    alphaDone = addBaseButton(tr("DONE"), 400*windowSize, y, box);
    alphaDone->setFixedSize(70*windowSize, ys);
    alphaDone->setObjectName("keyspecial");

    connect(alphaCancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(alphaDone , SIGNAL(released()), this,SLOT(doneReleased()));

    box->setVisible(false);
    return box;
}

QGroupBox *Ranger::createNumericKeyboard() {
    QGroupBox *box =  new QGroupBox(this);
    box->setGeometry(windowXpos, 100*windowSize,
                     windowWidth, 190*windowSize);
    box->setStyleSheet("QGroupBox {border:0 none;}");
    box->setVisible(false);

    QStringList keys;
    keys << "1234567890"
         << "-/:;()$@"
         << ".,?!'";

    labelFont.setPointSize(16*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    int x = 10*windowSize;
    int y = 0;
    int xi = 47*windowSize;
    int yi = 40*windowSize;
    int row = 0;
    int key = 0;

    for (int i=0;i<keys[row].length();i++) {
        numericKeyButtons[key] = addBaseButton(keys[row].at(i), x, y, box);
        numericKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    x = 34*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        numericKeyButtons[key] = addBaseButton(keys[row].at(i), x, y, box);
        if (keyboardEditText == "zipCode" && i != 0) {
           numericKeyButtons[key]->setDisabled(true);
        }
        numericKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    // Handle ampersand (&)
    numericKeyButtons[key] = addBaseButton("&&", x, y, box);
    if (keyboardEditText == "zipCode") {
       numericKeyButtons[key]->setDisabled(true);
    }
    numericKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);

    x = 82*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        numericKeyButtons[key] = addBaseButton(keys[row].at(i), x, y, box);
        if (keyboardEditText == "zipCode") {
           numericKeyButtons[key]->setDisabled(true);
        }
        numericKeyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    // SHIFT key
    x = 10*windowSize;
    int ys = 32*windowSize;
    labelFont.setPointSize(11*windowSize);
    numericKeyButtons[key] = new QPushButton("SHIFT", box);
    numericKeyButtons[key]->move(x,y);
    numericKeyButtons[key]->setFont(labelFont);
    numericKeyButtons[key]->setDisabled(true);
    numericKeyButtons[key]->setObjectName("keyspecial");
    numericKeyButtons[key++]->setFixedSize(65*windowSize, ys);


    // .COM key
    x += 305*windowSize;
    numericKeyButtons[key] = addBaseButton(".COM", x, y, box);
    numericKeyButtons[key]->setObjectName("keyspecial");
    if (keyboardEditText == "zipCode") {
       numericKeyButtons[key]->setDisabled(true);
    }
    numericKeyButtons[key++]->setFixedSize(82*windowSize, ys);

    // left arrow key
    x = 410*windowSize;
    QPixmap pix(":images/backspace_" + rangerThemes["keyboard"].getTextColor() + ".png");
    QIcon qi = QIcon(pix);
    QSize qs(pix.rect().width()*(windowSize/2),pix.rect().height()*(windowSize/2));
    numericBack  = addBaseButton("", x, y, box);
    numericBack->setProperty("button", keyboardEditText+"name");
    numericBack->setGeometry(x,y, 60*windowSize, ys);
    numericBack->setIcon(qi);
    numericBack->setIconSize(qs);
    numericBack->setObjectName("keyspecial");

    connect(numericBack , SIGNAL(released()), this,SLOT(keyBackReleased()));

    // SPACE bar
    y += yi;
    numericKeyButtons[key] = addBaseButton(tr("SPACE"), 200*windowSize, y, box);
    numericKeyButtons[key]->setProperty("button","space");
    numericKeyButtons[key]->setObjectName("keyspecial");
    if (keyboardEditText == "zipCode") {
       numericKeyButtons[key]->setDisabled(true);
    }
    numericKeyButtons[key++]->setGeometry(200*windowSize, y, 190*windowSize, ys);

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<key;j++) {
        //numericKeyButtons[j]->setProperty("button", keyboardEditText);
        cg->addButton(numericKeyButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                this,SLOT(keyButtonReleased(QAbstractButton*)));

    numericCancel = addBaseButton(tr("CANCEL"), 28*windowSize, y, box);
    numericCancel->setFixedSize(85*windowSize, ys);
    numericCancel->setObjectName("keyspecial");

    abcButton = addBaseButton("ABC", 122*windowSize, y, box);
    abcButton->setProperty("button", "abcButton");
    abcButton->setFixedSize(70*windowSize, ys);
    if (keyboardEditText == "zipCode") {
       abcButton->setDisabled(true);
    }
    abcButton->setObjectName("keyspecial");

    numericDone = addBaseButton(tr("DONE"), 400*windowSize, y, box);
    numericDone->setFixedSize(70*windowSize, ys);
    numericDone->setObjectName("keyspecial");

    connect(numericCancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(abcButton , SIGNAL(released()), this,SLOT(buttonReleased()));
    connect(numericDone , SIGNAL(released()), this,SLOT(doneReleased()));

    box->setVisible(false);
    return box;
}

QGroupBox *Ranger::createEditSchedulePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *edit = addHeaderLabel(tr("EDIT SCHEDULE:"), 0, 11*windowSize, box);
    edit->setFixedWidth(280*windowSize);
    edit->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    editScehduleHeaderLabel = addHeaderLabel(selectedDay, 286*windowSize, 11*windowSize, box);
    editScehduleHeaderLabel->setFixedWidth(180*windowSize);
    editScehduleHeaderLabel->setAlignment(Qt::AlignLeft);

    int x = 15*windowSize;
    int y = 60*windowSize;
    int w = 90*windowSize;
    int xi = 92*windowSize;

    labelFont.setPointSize(11*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    int i=0;
    periodLabel = addLabels("P", (x), y, "color:#999;", box);
    periodLabel->setFixedWidth(w);
    periodLabel->setAlignment(Qt::AlignHCenter);

    startLabel = addLabels("ST", (x+=xi), y, "color:#999;", box);
    startLabel->setFixedWidth(w);
    startLabel->setAlignment(Qt::AlignHCenter);

    stopLabel = addLabels("ET", (x+=xi), y, "color:#999;", box);
    stopLabel->setFixedWidth(w);
    stopLabel->setAlignment(Qt::AlignHCenter);

    heatToLabel = addLabels(QString::number(0)+degreeCode.at(degreeCode.length()-1),
                            (x+=xi), y, "color:#cd4744;", box);
    heatToLabel->setFixedWidth(w);
    heatToLabel->setAlignment(Qt::AlignHCenter);

    coolToLabel = addLabels(QString::number(0)+degreeCode.at(degreeCode.length()-1),
                            (x+=xi), y, "color:#2d89c6;", box);
    coolToLabel->setFixedWidth(w);
    coolToLabel->setAlignment(Qt::AlignHCenter);

    QStringList days;
    days << tr("PERIOD") << tr("START") << tr("STOP") << tr("HEAT TO") << tr("COOL TO");

    editScheduleButtons = new QPushButton[days.length()];
    x = 15*windowSize;
    y = 90*windowSize;
    labelFont.setPointSize(9*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    for (int i=0;i<days.length();i++) {
        editScheduleButtons[i].setParent(box);
        editScheduleButtons[i].setText(days[i]);
        editScheduleButtons[i].setFont(labelFont);
        editScheduleButtons[i].setGeometry(x, y, 88*windowSize, 30*windowSize);
        editScheduleButtons[i].setObjectName("butoff");
        editScheduleButtons[i].show();
        x += xi;
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<days.length();j++) {
        cg->addButton(&editScheduleButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                this,SLOT(editScheduleButtonReleased(QAbstractButton*)));

    x = 35*windowSize + (0*92*windowSize);
    upButton = addUpButton(x, 125*windowSize, "#ccc", box);
    upButton->setProperty("button", "editSchedule");

    dnButton = addDownButton(x, 178*windowSize, "#ccc", box);
    dnButton->setProperty("button", "editSchedule");

    connect(upButton , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upButton , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnButton , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnButton , SIGNAL(released()), this,SLOT(downReleased()));

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    weekButton = addButton(tr("WEEKDAY"), 200*windowSize, 230*windowSize, box);
    weekButton->setProperty("button","weekButton");
    connect(weekButton , SIGNAL(released()), this,SLOT(buttonReleased()));

    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", "editSchedule");

    QPushButton *save = addButton(tr("SAVE"), 390*windowSize, 230*windowSize, box);
    save->setProperty("button", "editSchedule");
    save->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(save , SIGNAL(released()), this,SLOT(saveReleased()));

    box->setVisible(false);

    return box;
}

void Ranger::loadSchedules() {

    currentSchedule = dailySchedules[selectedDay];
    DailySchedule sched = currentSchedule["WAKE"];
    currentDailySchedule =  currentSchedule["WAKE"];

    periodLabel->setText(sched.getPeriod());
    startLabel->setText(sched.getStartTime());
    stopLabel->setText(sched.getStopTime());
    heatToLabel->setText(QString::number(sched.getHeatTo())+degreeCode.at(degreeCode.length()-1));
    coolToLabel->setText(QString::number(sched.getCoolTo())+degreeCode.at(degreeCode.length()-1));

    QStringList days;
    days << tr("PERIOD") << tr("START") << tr("STOP") << tr("HEAT TO") << tr("COOL TO");

    for (int i=0;i<days.length();i++) {
        //editScheduleButtons[i].setText(days[i]);
        editScheduleButtons[i].setObjectName("butoff");
//        if (i == sched.getIndex()) {
//            editScheduleButtons[i].setObjectName("");
//        } else {
//            editScheduleButtons[i].setObjectName("butoff");
//        }
    }
    editScheduleButtons[0].setObjectName("");
    upButton->move(35*windowSize,125*windowSize);
    dnButton->move(35*windowSize,178*windowSize);
    currentIndex = 0;

    setStyleSheet(rangerStyleSheet);
}

QGroupBox *Ranger::createDailySchedulePanel() {
    QGroupBox *box = backgroundBox();

    QStringList days;
    days << "M" << "T" << "W" << "TH" << "F" << "S" << "SU";

    dailyButtons = new QPushButton[days.length()];

    int x = 12*windowSize;
    int y = 10*windowSize;
    labelFont.setPointSize(18*windowSize);
    labelFont.setWeight(QFont::Normal);
    for (int i=0;i<7;i++) {
        dailyButtons[i].setParent(box);
        dailyButtons[i].setText(days[i]);
        dailyButtons[i].setFont(labelFont);
        dailyButtons[i].move(x,y);
        dailyButtons[i].setFixedWidth(48*windowSize);
        dailyButtons[i].show();
        x += 68*windowSize;
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<days.length();j++) {
        cg->addButton(&dailyButtons[j]);
    }

    connect(cg , SIGNAL(buttonPressed(QAbstractButton*)),
            this,SLOT(dayButtonPressed(QAbstractButton*)));
    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(dayButtonReleased(QAbstractButton*)));

    x = 30*windowSize;
    y = 65*windowSize;
    QString st = "color:#aaa;";
    QPixmap pixmap(480*windowSize,2*windowSize);
    pixmap.fill(QColor("#eaeaea"));

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(11*windowSize);
    for (int i=0;i<periodList.length();i++) {
        dailyPeriodLabel[i] = addLabels("", x , y, st, box);
        dailyPeriodLabel[i]->setFixedWidth(80*windowSize);
        dailyTimeLabel[i] = addLabels("", x + 110*windowSize, y, st, box);
        dailyTimeLabel[i]->setFixedWidth(180*windowSize);
        dailyHeatToLabel[i] = addLabels("",
                  x + 320*windowSize, y, "color:#cd4744;", box);
        dailyHeatToLabel[i]->setFixedWidth(40*windowSize);
        dailyCoolToLabel[i] = addLabels("",
                  x + 394*windowSize, y, "color:#2d89c6;", box);
        dailyHeatToLabel[i]->setFixedWidth(40*windowSize);
        if (i == 3) continue;
        QLabel *label = new QLabel(box);
        label->setPixmap(pixmap);
        label->move(1,y+28*windowSize);
        y += 42*windowSize;
    }

    labelFont.setPointSize(10*windowSize);
    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button","dailySchedule");

    QPushButton *edit = addButton(tr("EDIT"), 400*windowSize, 230*windowSize, box);
    edit->setProperty("button","dailySchedule");
    edit->setFixedWidth(65*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(edit , SIGNAL(released()), this,SLOT(editReleased()));

    box->setVisible(false);

    return box;
}

void Ranger::loadDailySchedules() {
    QStringList weekdays;
    weekdays << "MONDAY" << "TUESDAY" << "WEDNESDAY" << "THURSDAY"
             << "FRIDAY" << "SATURDAY" << "SUNDAY";
    for (int i=0;i<7;i++) {
        dailyButtons[i].setProperty("button",weekdays[i]);
        if (weekdays[i] == selectedDay) {
            dailyButtons[i].setStyleSheet("font-weight:bold;color:white;border:0 none;background-image: none;background-color: transparent;");
        } else {
            dailyButtons[i].setStyleSheet("color:#ddd;border:0 none;background-image: none;background-color: transparent;");
        }
    }

    for (int i=0;i<periodList.length();i++) {
        DailySchedule ds = selectedDaySchedule[periodList[i]];
        dailyPeriodLabel[i]->setText(ds.getPeriod());
        dailyTimeLabel[i]->setText(ds.getStartTime() + " - " + ds.getStopTime());
        dailyHeatToLabel[i]->setText(QString::number(ds.getHeatTo()) + degreeCode.at(degreeCode.length()-1));
        dailyCoolToLabel[i]->setText(QString::number(ds.getCoolTo()) + degreeCode.at(degreeCode.length()-1));
    }
}

QGroupBox *Ranger::createHeatPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    addHeaderLabel(tr("HEAT TO"), 50*windowSize, 11*windowSize, box);
    addHeaderLabel(tr("UNTIL"), 315*windowSize, 11*windowSize, box);

#ifdef Q_OS_WIN
    labelFont.setPointSize(50*windowSize);
    heatToDegreesLabel = addLabel(QString::number(heatToDegrees),
                                  15*windowSize, 110*windowSize, box);
    heatToDegreesLabel->setFixedWidth(120*windowSize);
    heatToDegreesLabel->setAlignment(Qt::AlignRight);
#else
    labelFont.setPointSize(60*windowSize);
    heatToDegreesLabel = addLabel(QString::number(heatToDegrees),
                                  windowSize, 100*windowSize, box);
    heatToDegreesLabel->setAlignment(Qt::AlignRight);
    heatToDegreesLabel->setFixedWidth(150*windowSize);
#endif
    heatToDegreesLabel->setStyleSheet("color:#757575;");
    labelFont.setPointSize(30*windowSize);

#ifdef Q_OS_WIN
    QLabel *deg = addLabel(degreeCode.at(degreeCode.length()-1),
                           134*windowSize, 115*windowSize, box);
#else
    QLabel *deg = addLabel(degreeCode.at(degreeCode.length()-1),
                           146*windowSize, 108*windowSize, box);
#endif

    deg->setStyleSheet("color:#757575;");

    QPushButton *upTemp = addUpButton(75*windowSize,70*windowSize, "#ccc", box);
    upTemp->setProperty("button","heatToDegrees");

    QPushButton *dnTemp = addDownButton(75*windowSize,180*windowSize, "#ccc", box);
    dnTemp->setProperty("button","heatToDegrees");

    connect(upTemp , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upTemp , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnTemp , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnTemp , SIGNAL(released()), this,SLOT(downReleased()));

#ifdef Q_OS_WIN
    labelFont.setPointSize(50*windowSize);
    heatToTimeLabel = addLabel("", 195*windowSize, 110*windowSize, box);
#else
    labelFont.setPointSize(60*windowSize);
    heatToTimeLabel = addLabel("", 228*windowSize, 100*windowSize, box);
#endif

    heatToTimeLabel->setStyleSheet("color:#757575;");
    heatToTimeLabel->setAlignment(Qt::AlignRight);
    heatToTimeLabel->setFixedWidth(170*windowSize);

    heatToAmPmLabel = new QLabel("", box);

#ifdef Q_OS_WIN
   heatToAmPmLabel->setStyleSheet("color:#757575;font-size:"+
                        QString::number(44*windowSize)+"px;font-weight:normal;");
   heatToAmPmLabel->move(376*windowSize, 120*windowSize);
   heatToAmPmLabel->setFixedWidth(80);
#else
   heatToAmPmLabel->setStyleSheet("color:#757575;font-size:"+
                        QString::number(35*windowSize)+"px;font-weight:normal;");
   heatToAmPmLabel->move(360*windowSize, 184*windowSize);
#endif

    QPushButton *upHour = addUpButton(210*windowSize,70*windowSize, "#ccc", box);
    upHour->setProperty("button","heatToHour");

    QPushButton *dnHour = addDownButton(210*windowSize,180*windowSize, "#ccc", box);
    dnHour->setProperty("button","heatToHour");

    QPushButton *upMinute = addUpButton(300*windowSize,70*windowSize, "#ccc", box);
    upMinute->setProperty("button","heatToMinute");

    QPushButton *dnMinute = addDownButton(300*windowSize,180*windowSize, "#ccc", box);
    dnMinute->setProperty("button","heatToMinute");

    QPushButton *upAmPm = addUpButton(380*windowSize,70*windowSize, "#ccc", box);
    upAmPm->setProperty("button","heatToAmPm");

    QPushButton *dnAmPm = addDownButton(380*windowSize,180*windowSize, "#ccc", box);
    dnAmPm->setProperty("button","heatToAmPm");

    connect(upHour , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upHour , SIGNAL(released()), this,SLOT(upReleased()));
    connect(upMinute , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upMinute , SIGNAL(released()), this,SLOT(upReleased()));
    connect(upAmPm , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upAmPm , SIGNAL(released()), this,SLOT(upReleased()));

    connect(dnHour , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnHour , SIGNAL(released()), this,SLOT(downReleased()));
    connect(dnMinute , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnMinute , SIGNAL(released()), this,SLOT(downReleased()));
    connect(dnAmPm , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnAmPm , SIGNAL(released()), this,SLOT(downReleased()));

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", "heatTo");

    QPushButton *save = addButton(tr("SAVE"), 390*windowSize, 230*windowSize, box);
    save->setProperty("button", "heatTo");
    save->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(save , SIGNAL(released()), this,SLOT(saveReleased()));
    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createCoolPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    addHeaderLabel(tr("COOL TO"), 50*windowSize, 11*windowSize, box);
    addHeaderLabel(tr("UNTIL"), 315*windowSize, 11*windowSize, box);

#ifdef Q_OS_WIN
    labelFont.setPointSize(50*windowSize);
    coolToDegreesLabel = addLabel(QString::number(coolToDegrees),
                                  15*windowSize, 110*windowSize, box);
    coolToDegreesLabel->setFixedWidth(120*windowSize);
    coolToDegreesLabel->setAlignment(Qt::AlignRight);
#else
    labelFont.setPointSize(60*windowSize);
    coolToDegreesLabel = addLabel(QString::number(coolToDegrees),
                                  windowSize, 100*windowSize, box);
    coolToDegreesLabel->setAlignment(Qt::AlignRight);
    coolToDegreesLabel->setFixedWidth(150*windowSize);
 #endif
    coolToDegreesLabel->setStyleSheet("color:#757575;");

    labelFont.setPointSize(30*windowSize);

#ifdef Q_OS_WIN
    QLabel *deg = addLabel(degreeCode.at(degreeCode.length()-1),
                           134*windowSize, 115*windowSize, box);
#else
    QLabel *deg = addLabel(degreeCode.at(degreeCode.length()-1),
                           146*windowSize, 108*windowSize, box);
#endif

    deg->setStyleSheet("color:#757575;");

    labelFont.setPointSize(45*windowSize);
    QPushButton *upDeg = addUpButton(75*windowSize,70*windowSize, "#ccc", box);
    upDeg->setProperty("button","coolToDegrees");

    QPushButton *dnDeg = addDownButton(75*windowSize,180*windowSize, "#ccc", box);
    dnDeg->setProperty("button","coolToDegrees");

    connect(upDeg , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upDeg , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnDeg , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnDeg , SIGNAL(released()), this,SLOT(downReleased()));

#ifdef Q_OS_WIN
    labelFont.setPointSize(50*windowSize);
    coolToTimeLabel = addLabel("", 195*windowSize, 110*windowSize, box);
#else
    labelFont.setPointSize(60*windowSize);
    coolToTimeLabel = addLabel("", 228*windowSize, 100*windowSize, box);
#endif

    coolToTimeLabel->setStyleSheet("color:#757575;");
    coolToTimeLabel->setAlignment(Qt::AlignRight);
    coolToTimeLabel->setFixedWidth(170*windowSize);

    coolToAmPmLabel = new QLabel("", box);

#ifdef Q_OS_WIN
   coolToAmPmLabel->setStyleSheet("color:#757575;font-size:"+
                        QString::number(44*windowSize)+"px;font-weight:normal;");
   coolToAmPmLabel->move(376*windowSize, 120*windowSize);
   coolToAmPmLabel->setFixedWidth(80);
#else
   coolToAmPmLabel->setStyleSheet("color:#757575;font-size:"+
                        QString::number(35*windowSize)+"px;font-weight:normal;");
   coolToAmPmLabel->move(360*windowSize, 184*windowSize);
#endif

    QPushButton *upHour = addUpButton(210*windowSize,70*windowSize, "#ccc", box);
    upHour->setProperty("button","coolToHour");

    QPushButton *dnHour = addDownButton(210*windowSize,180*windowSize, "#ccc", box);
    dnHour->setProperty("button","coolToHour");

    QPushButton *upMinute = addUpButton(300*windowSize,70*windowSize, "#ccc", box);
    upMinute->setProperty("button","coolToMinute");

    QPushButton *dnMinute = addDownButton(300*windowSize,180*windowSize, "#ccc", box);
    dnMinute->setProperty("button","coolToMinute");

    QPushButton *upAmPm = addUpButton(380*windowSize,70*windowSize, "#ccc", box);
    upAmPm->setProperty("button","coolToAmPm");

    QPushButton *dnAmPm = addDownButton(380*windowSize,180*windowSize, "#ccc", box);
    dnAmPm->setProperty("button","coolToAmPm");

    connect(upHour , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upHour , SIGNAL(released()), this,SLOT(upReleased()));
    connect(upMinute , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upMinute , SIGNAL(released()), this,SLOT(upReleased()));
    connect(upAmPm , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upAmPm , SIGNAL(released()), this,SLOT(upReleased()));

    connect(dnHour , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnHour , SIGNAL(released()), this,SLOT(downReleased()));
    connect(dnMinute , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnMinute , SIGNAL(released()), this,SLOT(downReleased()));
    connect(dnAmPm , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnAmPm , SIGNAL(released()), this,SLOT(downReleased()));

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", "coolTo");

    QPushButton *save = addButton(tr("SAVE"), 390*windowSize, 230*windowSize, box);
    save->setProperty("button", "coolTo");
    save->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(save , SIGNAL(released()), this,SLOT(saveReleased()));

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createFanModePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    addHeaderLabel(tr("SELECT FAN MODE"), 110*windowSize, 11*windowSize, box);

    labelFont.setPointSize(12*windowSize);

    QStringList buts;
    buts << tr("AUTO") << tr("ON") ;

    QStringList props;
    props << "auto" << "on" ;

    fanModeButtons = new QPushButton[buts.length()];
    int x = 122*windowSize;
    for (int i=0;i<buts.length();i++) {
        fanModeButtons[i].setParent(box);
        fanModeButtons[i].setText(buts[i]);
        fanModeButtons[i].setProperty("button",props[i]);
        fanModeButtons[i].setFont(labelFont);
        fanModeButtons[i].setGeometry(x, 120*windowSize, 90*windowSize, 50*windowSize);
        x += 150*windowSize;
        if (selectedFanMode.toLower() == props[i]) {
            fanModeButtons[i].setObjectName("");
        } else {
            fanModeButtons[i].setObjectName("butoff");
        }
        fanModeButtons[i].show();
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<buts.length();j++) {
        cg->addButton(&fanModeButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(fanButtonReleased(QAbstractButton*)));

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", "fanMode");

    QPushButton *save = addButton(tr("SAVE"), 390*windowSize, 230*windowSize, box);
    save->setProperty("button", "fanMode");
    save->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(save , SIGNAL(released()), this,SLOT(saveReleased()));

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createSettingsSchedulePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *st = addHeaderLabel(tr("SETTINGS:"), 0, 11*windowSize, box);
    st->setFixedWidth(240*windowSize);
    st->setAlignment(Qt::AlignRight);

    QPushButton *ret = addImageButton("return.png", 20*windowSize, 10*windowSize, .5, box);
    ret->setProperty("button","schedule");
    connect(ret , SIGNAL(pressed()), this,SLOT(returnPressed()));

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ql = addHeaderLabel(tr("SCHEDULE"), 250*windowSize, 11*windowSize, box);
    ql->setFixedWidth(150*windowSize);
    ql->setAlignment(Qt::AlignLeft);

    QStringList buts;
    buts << tr("ALL DAYS") << tr("WEEKDAY") + "/\n" + tr("WEEKEND")
         << tr("EACH DAY") << tr("VACATION");

    QStringList props;
    props << "all days" << "weekend" << "monday" << "vacation";
    scheduleButtons = new QPushButton[buts.length()];

    labelFont.setPointSize(9*windowSize);
    int x = 20*windowSize;
    for (int i=0;i<buts.length();i++) {
        scheduleButtons[i].setParent(box);
        scheduleButtons[i].setText(buts[i]);
        scheduleButtons[i].setProperty("button", props[i]);
        scheduleButtons[i].setFont(labelFont);
        scheduleButtons[i].setGeometry(x, 125*windowSize, 100*windowSize, 50*windowSize);
        if (selectedDay.toLower() == props[i]) {
            scheduleButtons[i].setObjectName("");
        } else if (selectedSchedule == props[i]) {
            scheduleButtons[i].setObjectName("butoffDefault");
        } else {
            scheduleButtons[i].setObjectName("butoff");
        }
        scheduleButtons[i].show();
        x += 115*windowSize;
        connect(&scheduleButtons[i] , SIGNAL(released()), this, SLOT(scheduleButtonReleased()));
    }

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    QPushButton *setSchedule = addButton(tr("SET SCHEDULE"), 20*windowSize, 230*windowSize, box);
    setSchedule->setProperty("button", "setSchedule");

    QPushButton *edit = addButton(tr("EDIT"), 390*windowSize, 230*windowSize, box);
    edit->setProperty("button", "editSchedule");
    edit->setFixedWidth(75*windowSize);

    connect(setSchedule , SIGNAL(released()), this, SLOT(scheduleButtonReleased()));
    connect(edit , SIGNAL(released()), this, SLOT(scheduleButtonReleased()));

    box->setVisible(false);
    return box;
}

QGroupBox *Ranger::createEditThermoNamePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *ed = addHeaderLabel(tr("EDIT:"), 0, 11*windowSize, box);
    ed->setFixedWidth(150*windowSize);
    ed->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ql = addHeaderLabel(tr("THERMOSTAT NAME"), 160*windowSize, 11*windowSize, box);
    ql->setFixedWidth(270*windowSize);
    ql->setAlignment(Qt::AlignLeft);

    shiftOn = false;
    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createWifiPasswordPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ed = addHeaderLabel(tr("ENTER WIFI PASSWORD"), 0, 11*windowSize, box);
    ed->setFixedWidth(windowWidth);
    ed->setAlignment(Qt::AlignHCenter);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createDeleteThermoPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *ed = addHeaderLabel(tr("DELETE:"), 0, 11*windowSize, box);
    ed->setFixedWidth(180*windowSize);
    ed->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ql = addHeaderLabel(thermoNameText,
                                190*windowSize, 11*windowSize, box);
    ql->setFixedWidth(270*windowSize);
    ql->setAlignment(Qt::AlignLeft);

    labelFont.setPointSize(14*windowSize);
    int x = 100*windowSize;
    QPushButton *yes = addButton(tr("YES"), x, 125*windowSize, box);
    yes->setProperty("button","deleteYes");
    yes->setFixedSize(125*windowSize, 50*windowSize);

    x += 170*windowSize;
    QPushButton *no = addButton(tr("NO"), x, 125*windowSize, box);
    no->setProperty("button","deleteNo");
    no->setFixedSize(125*windowSize, 50*windowSize);

    connect(yes , SIGNAL(released()), this,SLOT(buttonReleased()));
    connect(no , SIGNAL(released()), this,SLOT(buttonReleased()));

    box->setVisible(false);

    return box;

}

QGroupBox *Ranger::createThermostatLocationPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    addHeaderLabel(tr("THERMOSTAT LOCATION(S)"), 85*windowSize, 11*windowSize, box);


    labelFont.setPointSize(10*windowSize);
    if (firstTime) {
//        QPushButton *addNew = addButton(tr("ADD NEW"), 190*windowSize, 230*windowSize, box);
//        addNew->setProperty("button","addNewLocation");
//        connect(addNew , SIGNAL(released()), this,SLOT(buttonReleased()));

        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "thermostatLocationBack");

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "thermostatLocationNext");
        next->setFixedWidth(75*windowSize);

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
        cancel->setProperty("button","thermostatLocationCancel");
        connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));

//        QPushButton *addNew = addButton(tr("ADD NEW"), 365*windowSize, 230*windowSize, box);
//        addNew->setProperty("button","addNewLocation");
//        connect(addNew , SIGNAL(released()), this,SLOT(buttonReleased()));

    }

    box->setVisible(false);

    return box;
}

void Ranger::thermostatLocations() {
    if (thermostats.size() != 0) {
        locationButtons = new QPushButton[thermostats.size()];

        QMap<QString, Thermostat>::iterator i;
        QMap<QString, Thermostat> thermos = thermostats;
        labelFont.setPointSize(9*windowSize);

        QFontMetrics fm(labelFont);
        labelFont.setWeight(QFont::DemiBold);

        int x = 20*windowSize;
        int y = 100*windowSize;
        int k = 0;
        for (i=thermos.begin(); i != thermos.end(); ++i) {
            Thermostat t = i.value();
            if (t.getRoom().length() == 0) continue;
            locationButtons[k].setParent(thermostatLocationPanel);
            QString st = t.getRoom();
            if (fm.width(t.getRoom()) > 130) {
                int ix = t.getRoom().indexOf(" ");
                if (ix > 0) {
                    st = t.getRoom().replace(ix, 1, "\n");
                }
            }
            locationButtons[k].setText(st);
            locationButtons[k].setGeometry(x, y, 130*windowSize, 50*windowSize);
            locationButtons[k].setProperty("button",i.key());
            locationButtons[k].setFont(labelFont);
            locationButtons[k].show();
            k++;
            if (k == 3) {
                x = 20*windowSize;
                y += 55*windowSize;
            } else {
               x += 155*windowSize;
            }
        }

        QButtonGroup *cg = new QButtonGroup;
        for (int j=0;j<thermostats.size();j++) {
            cg->addButton(&locationButtons[j]);
        }

        connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                    this, SLOT(locationButtonReleased(QAbstractButton*)));
    }
}

// SETTINGS
QGroupBox *Ranger::createSettingsPanel() {
    QGroupBox *box = backgroundBox();

    QPushButton *ret = addImageButton("return.png", 20*windowSize, 10*windowSize, .5, box);
    ret->setProperty("button","settings");
    connect(ret , SIGNAL(pressed()), this,SLOT(returnPressed()));

    headerFont.setWeight(QFont::DemiBold);
    addHeaderLabel(tr("SETTINGS"), 180*windowSize, 11*windowSize, box);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(11*windowSize);

    QStringList labels;
    labels << tr("Time/Date") << tr("Lock") << tr("Schedule") << tr("Languages")
           << tr("Colors") <<  tr("Profile") << tr("Preferences") << tr("Setup");

    QStringList props;
    props << "time_date" << "lock" << "schedule" << "languages"
           << "colors" << "profile" << "preferences" << "setup";

    iconsButtons = new QPushButton[props.length()];

    int x = 30*windowSize;
    int y = 75*windowSize;
    for (int i=0;i<props.length();i++) {
        QPixmap pix(":/images/" + props[i]+"_"+currentScheme);
        pix = pix.scaledToWidth(pix.rect().width()*.5*windowSize);

        QIcon qi = QIcon(pix);
        QSize qs(pix.rect().width(),pix.rect().height());

        iconsButtons[i].setParent(box);
        iconsButtons[i].setIcon(qi);
        iconsButtons[i].setIconSize(qs);
        iconsButtons[i].setObjectName("pixmap");
        iconsButtons[i].setProperty("button",props[i]);
        iconsButtons[i].move(x +(10*windowSize),y);

        QLabel *lbl = addLabel(labels[i], x-22*windowSize, y + 55*windowSize, box);
        lbl->setAlignment(Qt::AlignHCenter);
        lbl->setStyleSheet("color:#666;");
        lbl->setFixedWidth(120*windowSize);
        x += 115*windowSize;
        if (i == 3) {
            x = 30*windowSize;
            y = 165*windowSize;
        }
    }

    x = 30*windowSize;
    iconsButtons[3].move(x+3*115*windowSize, 80*windowSize);
    iconsButtons[5].move(x+126*windowSize, 170*windowSize);
    iconsButtons[6].move(x+2*115*windowSize, 155*windowSize);
    iconsButtons[7].move(x+3*115*windowSize, 160*windowSize);

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<props.length();j++) {
        cg->addButton(&iconsButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(settingsButtonReleased(QAbstractButton*)));

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createColorsPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *st = addHeaderLabel(tr("SETTINGS:"), 0, 11*windowSize, box);
    st->setFixedWidth(250*windowSize);
    st->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("COLORS"), 256*windowSize, 11*windowSize, box);
    cl->setAlignment(Qt::AlignLeft);
    cl->setFixedWidth(180*windowSize);

    QStringList props;
        props << "green" << "lightBlue" << "yellow" << "pink" << "blue";

    colorButtons = new QPushButton[props.length()];
    int x = 20*windowSize;

    for (int i=0;i<props.length();i++) {
        colorButtons[i].setParent(box);
        colorButtons[i].setProperty("button", props[i]);
        colorButtons[i].setObjectName(props[i]+"But");
        colorButtons[i].setGeometry(x, 118*windowSize, 74*windowSize, 47*windowSize);
        if (props[i] == currentScheme){
             colorButtons[i].setObjectName(props[i]+"ButSelected");
        }
        colorButtons[i].show();
        x += 91*windowSize;
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<props.length();j++) {
        cg->addButton(&colorButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(colorButtonReleased(QAbstractButton*)));

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", "colors");

    QPushButton *done = addButton(tr("SAVE"), 390*windowSize, 230*windowSize, box);
    done->setProperty("button", "colors");
    done->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(done , SIGNAL(released()), this,SLOT(saveReleased()));

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createEditRoomPanel() {
    QGroupBox *box = backgroundBox();
    //qDebug() << "createEditRoomPanel"<<thermoNameText;

//    headerFont.setWeight(QFont::Normal);
//    QFontMetrics f1(headerFont);

//    int w1 = f1.width("EDIT: ");
//    QLabel *ed = addHeaderLabel(tr("EDIT:"), 0, 11*windowSize, box);
//    ed->setAlignment(Qt::AlignLeft);

    headerFont.setWeight(QFont::DemiBold);
//    QFontMetrics f2(headerFont);
//    int w2 = f2.width(thermoNameText);
    editRoomHeaderLabel =  addHeaderLabel(thermoNameText,
                                180*windowSize, 11*windowSize, box);
    editRoomHeaderLabel->setAlignment(Qt::AlignLeft);

//    int xw = (windowWidth/2)-((w1+w2)/2);
//    ed->move(xw, 11*windowSize);
//    ql->move(xw+w1, 11*windowSize);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(9*windowSize);
    QStringList buts;
    buts << tr("CHANGE\nNAME") << tr("DELETE\nTHERMOSTAT");

    QStringList props;
    props << "changeName" << "delete_thermostat";

    QPushButton *modeButtons[buts.length()];

    int x = 90*windowSize;
    for (int i=0;i<buts.length();i++) {
        modeButtons[i] = addButton(buts[i], x, 125*windowSize, box);
        modeButtons[i]->setFont(labelFont);
        modeButtons[i]->setProperty("button", props[i]);
        modeButtons[i]->setFixedSize(135*windowSize, 50*windowSize);
        connect(modeButtons[i] , SIGNAL(released()), this,SLOT(buttonReleased()));
        x += 170*windowSize;
    }

    createCancelDone("editRoom", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createLockPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    if (screenLocked) {
        QLabel *st = addHeaderLabel(tr("ENTER CODE"), 0, 11*windowSize, box);
        st->setFixedWidth(windowWidth);
        st->setAlignment(Qt::AlignHCenter);
        for (int i=0;i<4;i++) {
            lockCodes[i] = 0;
        }
    } else {
        QLabel *st = addHeaderLabel(tr("SETTINGS:"), 0, 11*windowSize, box);
        st->setFixedWidth(260*windowSize);
        st->setAlignment(Qt::AlignRight);

        headerFont.setWeight(QFont::DemiBold);
        QLabel *cl = addHeaderLabel(tr("LOCK"), 270*windowSize, 11*windowSize, box);
        cl->setAlignment(Qt::AlignLeft);
        cl->setFixedWidth(180*windowSize);
        for (int i=0;i<4;i++) {
            lockCodes[i] =  QString(lockCode.at(i)).toInt();
        }
    }

    labelFont.setPointSize(40*windowSize);

    QPushButton *upButs[4];
    QPushButton *dnButs[4];

    int x = 28*windowSize;
    int y = 100*windowSize;

    QSize qs(36*windowSize,36*windowSize);
    for (int i=0;i<4;i++) {
        lockNums[i] = addText(QString::number(lockCodes[i]),x, y,
                  "border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;", box);
        lockNums[i]->setFixedWidth(54*windowSize);
        lockNums[i]->setAlignment(Qt::AlignHCenter);

        upButs[i] = addButton("",x+(50*windowSize), y-(4*windowSize), box);
        upButs[i]->setProperty("button", "lock"+QString::number(i));
        upButs[i]->setIcon(*upIcon);
        upButs[i]->setObjectName("upButton");
        upButs[i]->setIconSize(qs);

        dnButs[i] = addButton("",x+(50*windowSize), y+(34*windowSize), box);
        dnButs[i]->setProperty("button", "lock"+QString::number(i));
        dnButs[i]->setIcon(*dnIcon);
        dnButs[i]->setObjectName("dnButton");
        dnButs[i]->setIconSize(qs);

        connect(upButs[i] , SIGNAL(pressed()), this,SLOT(upNumPressed()));
        connect(upButs[i] , SIGNAL(released()), this,SLOT(upNumReleased()));
        connect(dnButs[i] , SIGNAL(pressed()), this,SLOT(downNumPressed()));
        connect(dnButs[i] , SIGNAL(released()), this,SLOT(downNumReleased()));

        x += 110*windowSize;
    }

    if (screenLocked) {
        labelFont.setPointSize(10*windowSize);
        labelFont.setWeight(QFont::DemiBold);

        QPushButton *done = addButton(tr("DONE"), 390*windowSize, 230*windowSize, box);
        done->setProperty("button", "lock");
        done->setFixedWidth(75*windowSize);

        connect(done , SIGNAL(released()), this,SLOT(doneReleased()));
    } else {
        createCancelSave("lock", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createLockErrorPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *ed = addHeaderLabel(tr("INCORRECT CODE"), 0, 11*windowSize, box);
    ed->setFixedWidth(windowWidth);
    ed->setAlignment(Qt::AlignHCenter);

    labelFont.setPointSize(14*windowSize);
    int x = 100*windowSize;
    QPushButton *yes = addButton(tr("Try Again"), x, 125*windowSize, box);
    yes->setProperty("button","lockCodeYes");
    yes->setFixedSize(125*windowSize, 50*windowSize);

    x += 170*windowSize;
    QPushButton *no = addButton(tr("Give Up"), x, 125*windowSize, box);
    no->setProperty("button","lockCodeNo");
    no->setFixedSize(125*windowSize, 50*windowSize);

    connect(yes , SIGNAL(released()), this,SLOT(buttonReleased()));
    connect(no , SIGNAL(released()), this,SLOT(buttonReleased()));

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createProfilePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *st = addHeaderLabel(tr("REGISTRATION"), 0, 11*windowSize, box);
    st->setFixedWidth(480*windowSize);
    st->setAlignment(Qt::AlignHCenter);

    QStringList labels;
    labels << "Name:" << "Zip Code:" <<"MAC Address:" << "CRC:";

    QStringList props;
    props << "customerName" << "zipCode" <<"mac" << "crc";

    QStringList data;
    data << customerNameText << zipCodeText << thermostatMACText << thermostatCRCText;

    fields = new QPushButton[labels.size()];

    labelFont.setWeight(QFont::Normal);
    labelFont.setPointSize(16*windowSize);

    int x = 20*windowSize;
    int y = 65*windowSize;
    for (int i=0;i<labels.size();i++) {
        labelFont.setWeight(QFont::Normal);
        QLabel *fnl = addLabels(labels[i], x, y, "color:#557535;", box);
        fnl->setAlignment(Qt::AlignRight);
        fnl->setFixedWidth(160*windowSize);

        labelFont.setWeight(QFont::DemiBold);
        fields[i].setParent(box);
        fields[i].setText(data[i]);
        fields[i].setFont(labelFont);
        fields[i].setGeometry(x + 164*windowSize, y, 250*windowSize, 30*windowSize);
        fields[i].setProperty("button",props[i]);
        fields[i].setObjectName("text");
        fields[i].show();
        connect(&fields[i] , SIGNAL(released()), this, SLOT(profileReleased()));

        y += 40*windowSize;
    }

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);
        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "profileBack");

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "profileNext");
        next->setFixedWidth(75*windowSize);

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        createCancelSave("profile", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createTimeDatePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *st = addHeaderLabel(tr("SETTINGS:"), 0, 11*windowSize, box);
    st->setFixedWidth(220*windowSize);
    st->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    QLabel *td = addHeaderLabel(tr("TIME/DATE"), 230*windowSize, 11*windowSize, box);
    td->setFixedWidth(200*windowSize);
    td->setAlignment(Qt::AlignLeft);

    labelFont.setPointSize(28*windowSize);
    QFontMetrics fm(labelFont);

    int x = 54*windowSize;
    int y = 80*windowSize;
    int y1 = y-(13*windowSize);
    int y2 = y+(19*windowSize);

    QSize qs(34*windowSize,34*windowSize);
    QString style = "border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;";

    // TIME
    labelFont.setPointSize(11*windowSize);
    addLabels("Hour", x, y1-(5*windowSize), "color:#557535;text-align:left;", box);

    labelFont.setPointSize(28*windowSize);
    hourText = addText(QString::number(currentHour),x, y, style, box);
    hourText->setFixedWidth(fm.width("00")*1.2);
    hourText->setAlignment(Qt::AlignRight);

    QPushButton *upHour = addUpButton(x+fm.width("00")*1.2, y1, "#ccc", box);
    upHour->setProperty("button", "hour");
    upHour->setIcon(*upIcon);
    upHour->setIconSize(qs);

    QPushButton *dnHour = addDownButton(x+fm.width("00")*1.2, y2, "#ccc", box);
    dnHour->setProperty("button", "hour");
    dnHour->setIcon(*dnIcon);
    dnHour->setIconSize(qs);

    connect(upHour , SIGNAL(pressed()), this,SLOT(upNumPressed()));
    connect(upHour , SIGNAL(released()), this,SLOT(upNumReleased()));
    connect(dnHour , SIGNAL(pressed()), this,SLOT(downNumPressed()));
    connect(dnHour , SIGNAL(released()), this,SLOT(downNumReleased()));

    x = 184*windowSize;
    labelFont.setPointSize(11*windowSize);
    addLabels("Minute", x, y1-(5*windowSize), "color:#557535;text-align:left;", box);

    labelFont.setPointSize(28*windowSize);
    minuteText = addText(QString::number(currentMinute),x, y, style, box);
    minuteText->setFixedWidth(fm.width("00")*1.2);
    minuteText->setAlignment(Qt::AlignRight);

    QPushButton *upMinute = addUpButton(x+fm.width("00")*1.2, y1, "#ccc", box);
    upMinute->setProperty("button", "minute");
    upMinute->setIcon(*upIcon);
    upMinute->setIconSize(qs);

    QPushButton *dnMinute = addDownButton(x+fm.width("00")*1.2, y2, "#ccc", box);
    dnMinute->setProperty("button", "minute");
    dnMinute->setIcon(*dnIcon);
    dnMinute->setIconSize(qs);

    connect(upMinute , SIGNAL(pressed()), this,SLOT(upNumPressed()));
    connect(upMinute , SIGNAL(released()), this,SLOT(upNumReleased()));
    connect(dnMinute , SIGNAL(pressed()), this,SLOT(downNumPressed()));
    connect(dnMinute , SIGNAL(released()), this,SLOT(downNumReleased()));

    x = 348*windowSize;
    labelFont.setPointSize(11*windowSize);
    addLabels("AM/PM", x, y1-(5*windowSize), "color:#557535;text-align:left;", box);

    labelFont.setPointSize(28*windowSize);
    ampmText = addText(currentAmPm,x, y,style, box);
    ampmText->setFixedWidth(fm.width("AM")*1.2);
    ampmText->setAlignment(Qt::AlignHCenter);

    QPushButton *upAmPm = addUpButton(x+fm.width("AM")*1.2, y1, "#ccc", box);
    upAmPm->setProperty("button", "ampm");
    upAmPm->setIcon(*upIcon);
    upAmPm->setIconSize(qs);

    QPushButton *dnAmPm = addDownButton(x+fm.width("AM")*1.2, y2, "#ccc", box);
    dnAmPm->setProperty("button", "ampm");
    dnAmPm->setIcon(*dnIcon);
    dnAmPm->setIconSize(qs);

    connect(upAmPm , SIGNAL(pressed()), this,SLOT(upNumPressed()));
    connect(upAmPm , SIGNAL(released()), this,SLOT(upNumReleased()));
    connect(dnAmPm , SIGNAL(pressed()), this,SLOT(downNumPressed()));
    connect(dnAmPm , SIGNAL(released()), this,SLOT(downNumReleased()));

    // DATE
    x = 22*windowSize;
    y = 158*windowSize;
    y1 = y-(13*windowSize);
    y2 = y+(19*windowSize);

    labelFont.setPointSize(11*windowSize);
    addLabels("Month", x, y1-(5*windowSize), "color:#557535;text-align:left;", box);

    labelFont.setPointSize(28*windowSize);
    monthText = addText(months.at(currentMonth),x, y,style, box);
    monthText->setFixedWidth(fm.width("May")*1.2);
    monthText->setAlignment(Qt::AlignHCenter);

    QPushButton *upMonth = addUpButton(x+(fm.width("May")*1.2), y1, "#ccc", box);
    upMonth->setProperty("button", "month");
    upMonth->setIcon(*upIcon);
    upMonth->setIconSize(qs);

    QPushButton *dnMonth = addDownButton(x+(fm.width("May")*1.2), y2, "#ccc",  box);
    dnMonth->setProperty("button", "month");
    dnMonth->setIcon(*dnIcon);
    dnMonth->setIconSize(qs);

    connect(upMonth , SIGNAL(pressed()), this,SLOT(upNumPressed()));
    connect(upMonth , SIGNAL(released()), this,SLOT(upNumReleased()));
    connect(dnMonth , SIGNAL(pressed()), this,SLOT(downNumPressed()));
    connect(dnMonth , SIGNAL(released()), this,SLOT(downNumReleased()));

    x = 186*windowSize;
    labelFont.setPointSize(11*windowSize);
    addLabels("Day", x, y1-(5*windowSize), "color:#557535;text-align:left;", box);

    labelFont.setPointSize(28*windowSize);
    dayText = addText(QString::number(currentDay),x, y,style, box);
    dayText->setFixedWidth(fm.width("00")*1.2);
    dayText->setAlignment(Qt::AlignRight);

    QPushButton *upday = addUpButton(x+fm.width("00")*1.2, y1, "#ccc", box);
    upday->setProperty("button", "day");
    upday->setIcon(*upIcon);
    upday->setIconSize(qs);

    QPushButton *dnDay = addDownButton(x+fm.width("00")*1.2, y2, "#ccc", box);
    dnDay->setProperty("button", "day");
    dnDay->setIcon(*dnIcon);
    dnDay->setIconSize(qs);

    connect(upday , SIGNAL(pressed()), this,SLOT(upNumPressed()));
    connect(upday , SIGNAL(released()), this,SLOT(upNumReleased()));
    connect(dnDay , SIGNAL(pressed()), this,SLOT(downNumPressed()));
    connect(dnDay , SIGNAL(released()), this,SLOT(downNumReleased()));

    x = 310*windowSize;
    labelFont.setPointSize(11*windowSize);
    addLabels("Year", x, y1-(5*windowSize), "color:#557535;text-align:left;", box);

    labelFont.setPointSize(28*windowSize);
    yearText = addText(QString::number(currentYear),x, y, style, box);
    yearText->setFixedWidth(fm.width("0000")*1.2);
    yearText->setAlignment(Qt::AlignHCenter);

    QPushButton *upYear = addUpButton(x+fm.width("0000")*1.2, y1, "#ccc", box);
    upYear->setProperty("button", "year");
    upYear->setIcon(*upIcon);
    upYear->setIconSize(qs);

    QPushButton *dnYear = addDownButton(x+fm.width("0000")*1.2, y2, "#ccc", box);
    dnYear->setProperty("button", "year");
    dnYear->setIcon(*dnIcon);
    dnYear->setStyleSheet("border:0 none;outline:0");
    dnYear->setIconSize(qs);

    connect(upYear , SIGNAL(pressed()), this,SLOT(upNumPressed()));
    connect(upYear , SIGNAL(released()), this,SLOT(upNumReleased()));
    connect(dnYear , SIGNAL(pressed()), this,SLOT(downNumPressed()));
    connect(dnYear , SIGNAL(released()), this,SLOT(downNumReleased()));

//    QString style1 = "border: 1px solid #aaa;border-radius:2px;background-color:#557535;color:white;";
//    QString style2 = "border: 1px solid #aaa;border-radius:2px;background-color:#e2e2e2;color:#444;";
//    x = 115*windowSize;
//    y = 220*windowSize;
//    labelFont.setPointSize(10*windowSize);
//    QLabel *dst = addLabels("Daylight Savings Time:", x, y, "color:#557535;text-align:left;", box);
//    dst->setAlignment(Qt::AlignRight);
//    dst->setFixedWidth(185*windowSize);

//    QPushButton *t1 = addButton("On",301*windowSize, y-2*windowSize, box);
//    t1->setStyleSheet(style1);
//    t1->setFixedWidth(30*windowSize);

//    QPushButton *t2 = addButton("Off",330*windowSize, y-2*windowSize, box);
//    t2->setStyleSheet(style2);
//    t2->setFixedWidth(30*windowSize);

//    y = 242*windowSize;
//    QLabel *tm = addLabels("12/24 Hour Clock:", x, y, "color:#557535;text-align:left;", box);
//    tm->setAlignment(Qt::AlignRight);
//    tm->setFixedWidth(185*windowSize);

//    QPushButton *t3 = addButton("12",301*windowSize, y, box);
//    t3->setStyleSheet(style2);
//    t3->setFixedWidth(30*windowSize);

//    QPushButton *t4 = addButton("24",330*windowSize, y, box);
//    t4->setStyleSheet(style1);
//    t4->setFixedWidth(30*windowSize);

    createCancelSave("timeDate", box);

    box->setVisible(false);

    return box;
}

// PREFERENCES
QGroupBox *Ranger::createPreferencesPanel() {
    QGroupBox *box = backgroundBox();

    QPushButton *ret = addImageButton("return.png", 20*windowSize, 10*windowSize, .5, box);
    ret->setProperty("button","preferences");
    connect(ret , SIGNAL(pressed()), this,SLOT(returnPressed()));

    headerFont.setWeight(QFont::Normal);

    QFontMetrics f1(headerFont);
    int w1 = f1.width("SETTINGS:");
    QLabel *reg = addHeaderLabel(tr("SETTINGS:"), box);

    headerFont.setWeight(QFont::DemiBold);
    QFontMetrics f2(headerFont);
    int w2 = f2.width(tr("PREFERENCES"));
    QLabel *nm = addHeaderLabel(tr("PREFERENCES"), box);

    int xw = (windowWidth/2)-((w1+w2)/2);
    reg->move(xw, 11*windowSize);
    nm->move(xw+w1 + 5*windowSize, 11*windowSize);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(16*windowSize);
    QFontMetrics fm(labelFont);
    QString style = "padding-left:4px;padding-right:4px;border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;";
    QSize qs(32*windowSize,28*windowSize);

    QStringList prefs;
    prefs << tr("Scheduling Options") << tr("Tempurature Scale") << tr("Clock Format")
          << tr("Daylight Savings Time") << tr("System Change Over") << tr("Tempurature Limits")
          << tr("Keyboard Lockout") << tr("Schedule Periods");

    QStringList props;
    props << tr("schedulingOptions") << tr("tempuratureScale") << tr("clockFormat")
          << tr("dst") << tr("systemsChangeOver") << tr("tempuratureLimits")
          << tr("keyboardLockout") << tr("schedulePeriods");

    QPushButton *buts[prefs.size()];

    int x = 10*windowSize;
    int y = 70* windowSize;
    int xp = 0;
    if (prefsPage == 1) {
        QString lbl = tr("Scheduling Options:");
        addLabels(lbl, x, y, "color:#557535;", box);
        //schedText = addText("Automatic",fm.width(lbl)*1.07, y, style, box);
        //schedText->setAlignment(Qt::AlignHCenter);
        schedText = addButton(schedulingOption,fm.width(lbl)*1.07, y, box);
        schedText->setStyleSheet(style);
        schedText->setProperty("button","Scheduling Options");
        connect(schedText , SIGNAL(released()), this, SLOT(prefsReleased()));

        xp = fm.width(lbl)*1.07 + fm.width("Automatic")*1.15;
        QPushButton *up = addUpButton(xp, y, "#ccc", box);
        up->setProperty("button", "sched");
        up->setIcon(*upIcon);
        up->setIconSize(qs);
        up->setFixedSize(32*windowSize,28*windowSize);

        xp = fm.width(lbl)*1.07 + fm.width("Automatic")*1.327;
        QPushButton *dn = addUpButton(xp, y+1*windowSize, "#ccc", box);
        dn->setProperty("button", "sched");
        dn->setIcon(*dnIcon);
        dn->setIconSize(qs);
        dn->setFixedSize(32*windowSize,28*windowSize);

        connect(up , SIGNAL(pressed()), this, SLOT(upPressed()));
        connect(up , SIGNAL(released()), this,SLOT(upReleased()));
        connect(dn , SIGNAL(pressed()), this, SLOT(downPressed()));
        connect(dn , SIGNAL(released()), this,SLOT(downReleased()));

        lbl = tr("Temperature Scale:");
        y += fm.height()*1.6;
        addLabels(lbl, x, y, "color:#557535;", box);
        QLineEdit *tempLimitsText = addText("Farenheight",fm.width(lbl)*1.07, y, style, box);
        tempLimitsText->setAlignment(Qt::AlignHCenter);

        xp = fm.width(lbl)*1.07 + fm.width("Farenheight")*1.2;
        QPushButton *up1 = addUpButton(xp, y, "#ccc", box);
        up1->setProperty("button", "hour");
        up1->setIcon(*upIcon);
        up1->setIconSize(qs);
        up1->setFixedSize(32*windowSize,28*windowSize);

        xp = fm.width(lbl)*1.07 + fm.width("Farenheight")*1.35;
        QPushButton *dn1 = addUpButton(xp, y+1*windowSize, "#ccc", box);
        dn1->setProperty("button", "hour");
        dn1->setIcon(*dnIcon);
        dn1->setIconSize(qs);
        dn1->setFixedSize(32*windowSize,28*windowSize);

        lbl = tr("Clock Format:");
        y += fm.height()*1.6;
        addLabels(lbl, x, y, "color:#557535;", box);
        QLineEdit *clockFormatText = addText("12 Hour",fm.width(lbl)*1.09, y, style, box);
        clockFormatText->setAlignment(Qt::AlignHCenter);

        xp = fm.width(lbl)*1.07 + fm.width("12 Hour")*1.25;
        QPushButton *up2 = addUpButton(xp, y, "#ccc", box);
        up2->setProperty("button", "hour");
        up2->setIcon(*upIcon);
        up2->setIconSize(qs);
        up2->setFixedSize(32*windowSize,28*windowSize);

        xp = fm.width(lbl)*1.07 + fm.width("12 Hour")*1.47;
        QPushButton *dn2 = addUpButton(xp, y+1*windowSize, "#ccc", box);
        dn2->setProperty("button", "hour");
        dn2->setIcon(*dnIcon);
        dn2->setIconSize(qs);
        dn2->setFixedSize(32*windowSize,28*windowSize);

        lbl = tr("Daylight Savings Time:");
        y += fm.height()*1.6;
        addLabels(lbl, x, y, "color:#557535;", box);
        QLineEdit *dstText = addText("On",fm.width(lbl)*1.07, y, style, box);
        dstText->setFixedWidth(fm.width("OO")*1.3);
        dstText->setAlignment(Qt::AlignHCenter);

        xp = fm.width(lbl)*1.07 + fm.width("OO")*1.4;
        QPushButton *up3 = addUpButton(xp, y, "#ccc", box);
        up3->setProperty("button", "hour");
        up3->setIcon(*upIcon);
        up3->setIconSize(qs);
        up3->setFixedSize(32*windowSize,28*windowSize);

        xp = fm.width(lbl)*1.07 + fm.width("OO")*1.96;
        QPushButton *dn3 = addUpButton(xp, y+1*windowSize, "#ccc", box);
        dn3->setProperty("button", "hour");
        dn3->setIcon(*dnIcon);
        dn3->setIconSize(qs);
        dn3->setFixedSize(32*windowSize,28*windowSize);

    } else {
        labelFont.setPointSize(11.5*windowSize);
        for (int i=0;i<prefs.size();i++) {
            buts[i] = addButton(prefs[i], x, y, box);
            buts[i]->setProperty("button",props[i]);
            buts[i]->setFixedSize(225*windowSize, 36*windowSize);
            connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));

            if (i == 3) {
                x = 242*windowSize;
                y = 70*windowSize;
            } else {
                y += 45*windowSize;
            }
        }
    }

//    labelFont.setPointSize(11*windowSize);
//    QPushButton *left = addButton("PREV",165*windowSize, 230*windowSize, box);
//    left->setProperty("button", "left");

//    QPushButton *right = addButton("NEXT",255*windowSize, 230*windowSize, box);
//    right->setProperty("button", "right");

//    connect(left , SIGNAL(pressed()), this, SLOT(leftPressed()));
//    connect(left , SIGNAL(released()), this,SLOT(leftReleased()));
//    connect(right , SIGNAL(pressed()), this, SLOT(rightPressed()));
//    connect(right , SIGNAL(released()), this,SLOT(rightReleased()));

//    createCancelDone("preferences", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createSelectModePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("SELECT MODE"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    labelFont.setPointSize(9*windowSize);
    QStringList buts;
    buts << tr("OFF") << tr("AUTO") << tr("HEAT") << tr("COOL") << tr("E.HEAT");

    QStringList props;
    props << "off" << "auto" << "heat" << "cool" << "e_heat";
    selectModeButtons = new QPushButton[buts.length()];

    int x = 20*windowSize;
    int xi = (windowWidth/buts.length()) * .94;
    for (int i=0;i<buts.length();i++) {
        selectModeButtons[i].setParent(box);
        selectModeButtons[i].setText(buts[i]);
        selectModeButtons[i].setProperty("button",props[i]);
        selectModeButtons[i].setFont(labelFont);
        selectModeButtons[i].setObjectName("butoff");
        selectModeButtons[i].setGeometry(x, 125*windowSize, 80*windowSize, 50*windowSize);
        x += xi;
    }

    if (selectedMode == "off") {
        selectModeButtons[0].setObjectName("");
    } else if (selectedMode == "auto") {
        selectModeButtons[1].setObjectName("");
    } else if (selectedMode == "heat") {
        selectModeButtons[2].setObjectName("");
    } else if (selectedMode == "cool") {
        selectModeButtons[3].setObjectName("");
    } else if (selectedMode == "emer_heat") {
        selectModeButtons[4].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<buts.length();i++) {
        cg->addButton(&selectModeButtons[i]);
        selectModeButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(modeButtonReleased(QAbstractButton*)));

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", "selectMode");

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createSchedulingOptionsPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("SCHEDULING OPTIONS"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Programmable") << tr("Non-Programmable");

    QStringList props;
    props  << "programmable" << "non-programmable";

    schedulingOptionsButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 120*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        schedulingOptionsButtons[i].setParent(box);
        schedulingOptionsButtons[i].setText(prefs[i]);
        schedulingOptionsButtons[i].setFont(labelFont);
        schedulingOptionsButtons[i].setProperty("button", props[i]);
        schedulingOptionsButtons[i].setGeometry(x, y, 250*windowSize, 50*windowSize);
        schedulingOptionsButtons[i].setObjectName("butoff");
        y += 75*windowSize;
    }

    if (schedulingOption == "programmable") {
        schedulingOptionsButtons[0].setObjectName("");
    } else {
        schedulingOptionsButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&schedulingOptionsButtons[i]);
        schedulingOptionsButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(schedulingOptionsReleased(QAbstractButton*)));

    createCancelSave("schedulingOptions", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createTempuratureScalePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("TEMPURATURE SCALE"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Farenheit") << tr("Celsius");

    QStringList props;
    props << "F" << "C";

    tempuratureScaleButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        tempuratureScaleButtons[i].setParent(box);
        tempuratureScaleButtons[i].setText(prefs[i]);
        tempuratureScaleButtons[i].setFont(labelFont);
        tempuratureScaleButtons[i].setProperty("button", props[i]);
        tempuratureScaleButtons[i].setObjectName("butoff");
        tempuratureScaleButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        y += 75*windowSize;
    }

    if (!tempuratureScale) {
        tempuratureScaleButtons[0].setObjectName("");
    } else {
        tempuratureScaleButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&tempuratureScaleButtons[i]);
        tempuratureScaleButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(tempuratureScaleReleased(QAbstractButton*)));

    createCancelSave("tempuratureScale", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createClockFormatPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("CLOCK FORMAT"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("12 Hour") << tr("24 Hour");

    QStringList props;
    props << "12hour" << "24hour";

    clockFormatButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        clockFormatButtons[i].setParent(box);
        clockFormatButtons[i].setText(prefs[i]);
        clockFormatButtons[i].setFont(labelFont);
        clockFormatButtons[i].setProperty("button", props[i]);
        clockFormatButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        clockFormatButtons[i].setObjectName("butoff");
        y += 75*windowSize;
    }
    if (clockFormat == 12) {
        clockFormatButtons[0].setObjectName("");
    } else {
        clockFormatButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&clockFormatButtons[i]);
        clockFormatButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(clockFormatReleased(QAbstractButton*)));

    createCancelSave("clockFormat", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createDstPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("DAYLIGHT SAVING TIME"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("On") << tr("Off");

    QStringList props;
    props << "dstOn" << "dstOff";

    dstButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 158*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        dstButtons[i].setParent(box);
        dstButtons[i].setText(prefs[i]);
        dstButtons[i].setFont(labelFont);
        dstButtons[i].setProperty("button", props[i]);
        dstButtons[i].setObjectName("butoff");
        dstButtons[i].setGeometry(x, y, 160*windowSize, 50*windowSize);
        y += 75*windowSize;
    }
//qDebug() << "DST" << dst;
    if (dst) {
        dstButtons[0].setObjectName("");
    } else {
        dstButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&dstButtons[i]);
        dstButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(dstReleased(QAbstractButton*)));

    createCancelSave("dst", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createSytemsChangeOverPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("SYSTEMS CHANGE OVER"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Manual") << tr("Automatic");

    QStringList props;
    props << "manualChangeover" << "automaticChangeover";

    systemsChangeOverButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        systemsChangeOverButtons[i].setParent(box);
        systemsChangeOverButtons[i].setText(prefs[i]);
        systemsChangeOverButtons[i].setFont(labelFont);
        systemsChangeOverButtons[i].setProperty("button", props[i]);
        systemsChangeOverButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        systemsChangeOverButtons[i].setObjectName("butoff");
        y += 75*windowSize;
    }
    if (systemsChangeOver == "manual") {
        systemsChangeOverButtons[0].setObjectName("");
    } else {
        systemsChangeOverButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&systemsChangeOverButtons[i]);
        systemsChangeOverButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(systemsChangeOverReleased(QAbstractButton*)));

    createCancelSave("systemsChangeover", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createTempuratureLimitsPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("TEMPURATURE LIMITS"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    tempuratureLimits = "manual";
    createCancelSave("tempuratureLimits", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createKeyboardLockoutPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("KEYBOARD LOCKOUT"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Unlocked") << tr("Partially Unlocked") << "Locked";

    QStringList props;
    props << "keyboardUnlocked" << "keyboardPartiallyUnlocked" << "keyboardLocked";

    keyboardLockoutButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 125*windowSize;
    int y = 65*windowSize;
    for (int i=0;i<prefs.length();i++) {
        keyboardLockoutButtons[i].setParent(box);
        keyboardLockoutButtons[i].setText(prefs[i]);
        keyboardLockoutButtons[i].setFont(labelFont);
        keyboardLockoutButtons[i].setProperty("button", props[i]);
        keyboardLockoutButtons[i].setGeometry(x, y, 250*windowSize, 44*windowSize);
        keyboardLockoutButtons[i].setObjectName("butoff");
        y += 58*windowSize;
    }

    if (keyboardLockout == "unlocked") {
        keyboardLockoutButtons[0].setObjectName("");
    } else if (keyboardLockout == "partial") {
        keyboardLockoutButtons[1].setObjectName("");
    } else {
        keyboardLockoutButtons[2].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&keyboardLockoutButtons[i]);
        keyboardLockoutButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(keyboardLockoutReleased(QAbstractButton*)));

    createCancelSave("keyboardLockout", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createSchedulePeriodsPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("SCHEDULE PERIODS"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("2 Per Day") << tr("4 Per day");

    QStringList props;
    props << "2perday" << "4perday";

    schedulePeriodsButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        schedulePeriodsButtons[i].setParent(box);
        schedulePeriodsButtons[i].setText(prefs[i]);
        schedulePeriodsButtons[i].setFont(labelFont);
        schedulePeriodsButtons[i].setProperty("button", props[i]);
        schedulePeriodsButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        schedulePeriodsButtons[i].setObjectName("butoff");
        //x += 210*windowSize;
        y += 75*windowSize;
    }

    if (schedulePeriods == 2) {
        schedulePeriodsButtons[0].setObjectName("");
    } else {
        schedulePeriodsButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&schedulePeriodsButtons[i]);
        schedulePeriodsButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(schedulePeriodsReleased(QAbstractButton*)));

    createCancelSave("schedulePeriods", box);

    box->setVisible(false);

    return box;
}

// SETUP PANELS
QGroupBox *Ranger::createSetupPanel() {
    QGroupBox *box = backgroundBox();

    QPushButton *ret = addImageButton("return.png", 20*windowSize, 10*windowSize, .5, box);
    ret->setProperty("button","setup");
    connect(ret , SIGNAL(pressed()), this,SLOT(returnPressed()));

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("SYSTEM SETUP"), 140*windowSize, 11*windowSize, box);
    cl->setAlignment(Qt::AlignLeft);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(16*windowSize);
    QFontMetrics fm(labelFont);
    //tr("Change Over Valve") << "changeOverValve"
    QStringList prefs;
    prefs << tr("Thermostat Location") << tr("System Type") << tr("Thermostat Controls") << tr("Fan Control")
          << tr("WIFI Setup") << tr("Backup Heat") << tr("Cooling Stages") << tr("Heating Stages");

    QStringList props;
    props << "thermostatLocation" << "hvacType" << "thermostatControls" << "fanControl"
           << "wifiSetup" << "backupHeat" << "coolingStages" << "heatingStages";

    QPushButton *buts[prefs.size()];

    int x = 10*windowSize;
    int y = 70* windowSize;
    labelFont.setPointSize(11.5*windowSize);
    for (int i=0;i<prefs.size();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setProperty("button",props[i]);
        buts[i]->setFixedSize(220*windowSize, 36*windowSize);
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));

        if (i == 3) {
            x = 242*windowSize;
            y = 70*windowSize;
        } else {
            y += 45*windowSize;
        }
    }

   // createCancelDone("setup", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createLanguagesPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("LANGUAGES / IDIOMAS"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("English") << tr("Español");

    QStringList props;
    props << "english" << "spanish";

    languagesButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        languagesButtons[i].setParent(box);
        languagesButtons[i].setText(prefs[i]);
        languagesButtons[i].setFont(labelFont);
        languagesButtons[i].setProperty("button", props[i]);
        languagesButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        languagesButtons[i].setObjectName("butoff");
        y += 75*windowSize;
    }

    if (language == "english") {
        languagesButtons[0].setObjectName("");
    } else {
        languagesButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&languagesButtons[i]);
        languagesButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(languagesReleased(QAbstractButton*)));

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "languagesNext");
        next->setFixedWidth(75*windowSize);

        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        createCancelSave("language", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createSystemTypePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("SYSTEM TYPE"), 156*windowSize, 11*windowSize, box);
    cl->setAlignment(Qt::AlignLeft);

    QStringList prefs;
    prefs << tr("Forced Air") << tr("Heat Pump") << tr("Hot Water or Steam");

    QStringList props;
    props << "forcedAir" << "heatPump" << "waterOrSteam";

    systemTypeButtons = new QPushButton[props.length()];

    int x = 115*windowSize;
    int y = 75* windowSize;

    labelFont.setPointSize(11.5*windowSize);

    for (int i=0;i<prefs.size();i++) {
        systemTypeButtons[i].setParent(box);
        systemTypeButtons[i].setText(prefs[i]);
        systemTypeButtons[i].setProperty("button", props[i]);
        systemTypeButtons[i].setObjectName("butoff");
        systemTypeButtons[i].setFont(labelFont);
        systemTypeButtons[i].setGeometry(x, y, 260*windowSize, 36*windowSize);
        y += 50*windowSize;
    }

    if (hvacType == "air") {
        systemTypeButtons[0].setObjectName("");
    } else if (hvacType == "heat") {
        systemTypeButtons[1].setObjectName("");
    } else {
        systemTypeButtons[2].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&systemTypeButtons[i]);
        systemTypeButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(systemTypeReleased(QAbstractButton*)));

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);
        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "hvacTypeBack");
        back->setFixedWidth(75*windowSize);

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "hvacTypeNext");
        next->setFixedWidth(75*windowSize);

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        createCancelSave("hvacType", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createWifiPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("SELECT WIFI NETWORK"), 0, 11*windowSize, box);
    cl->setFixedWidth(windowWidth);
    cl->setAlignment(Qt::AlignHCenter);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(18*windowSize);
    QFontMetrics fm(labelFont);

    labelFont.setPointSize(16*windowSize);
    QPushButton *wifi = addButton("MY WIFI NETWORK",85*windowSize, 70*windowSize, box);
    wifi->setFixedSize(320*windowSize, 40*windowSize);
    wifi->setProperty("button", "myNetwork");
    connect(wifi , SIGNAL(released()), this,SLOT(buttonReleased()));

    QPushButton *pub = addButton("PUBLIC",85*windowSize, 115*windowSize, box);
    pub->setFixedSize(320*windowSize, 40*windowSize);
    pub->setProperty("button", "publicNetwork");
    connect(pub , SIGNAL(released()), this,SLOT(buttonReleased()));

    QPushButton *neighbor = addButton("NEIGHBOR",85*windowSize, 160*windowSize, box);
    neighbor->setFixedSize(320*windowSize, 40*windowSize);
    neighbor->setProperty("button", "neighborNetwork");
    connect(neighbor , SIGNAL(released()), this,SLOT(buttonReleased()));

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);
        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "wifiBack");

        QPushButton *rescan = addButton("RESCAN",190*windowSize, 230*windowSize, box);
        rescan->setProperty("button", "rescan");
        rescan->setFixedWidth(100*windowSize);

        labelFont.setPointSize(10*windowSize);
        QPushButton *later = addButton("LATER",390*windowSize, 230*windowSize, box);
        later->setProperty("button", "later");

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(rescan , SIGNAL(released()), this,SLOT(buttonReleased()));
        connect(later , SIGNAL(released()), this,SLOT(buttonReleased()));
    } else {
        createCancelDone("wifi", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createWifiSetupPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("WIFI SETUP"), 0, 11*windowSize, box);
    cl->setFixedWidth(windowWidth);
    cl->setAlignment(Qt::AlignHCenter);


    labelFont.setPointSize(11*windowSize);
    labelFont.setWeight(QFont::Normal);

    QStringList lbls;
    lbls << "Signal Strength:" << "Wifi Network:" << "Status" << "IPAddress"
         << "Thermostat MAC:" << "Thermostat CRC:";

    QStringList vals;
    vals << "20" << wifiNetworkText << thermostatStatusText << thermostatIpText
         << thermostatMACText << thermostatCRCText;

    QLabel *l1;
    int y = 60;
    for (int i=0;i<lbls.length();i++) {
        labelFont.setWeight(QFont::Normal);
        l1 = addLabels(lbls[i],
                         0, y*windowSize, "color:black;", box);
        l1->setFixedWidth(200*windowSize);
        l1->setAlignment(Qt::AlignRight);

        labelFont.setWeight(QFont::DemiBold);
        l1 = addLabels(vals[i],
                       210*windowSize, y*windowSize, "color:black;", box);
        l1->setFixedWidth(200*windowSize);
        l1->setAlignment(Qt::AlignLeft);

        y += 20;
    }

    labelFont.setWeight(QFont::Normal);
    labelFont.setPointSize(10*windowSize);
    QFontMetrics fm(labelFont);

    y += 5;

    addLabels("Please visit", 35*windowSize, y*windowSize, "color:black;", box);

    int x = 30*windowSize + fm.width("Please visitM");
    labelFont.setWeight(QFont::DemiBold);
    QFontMetrics fm1(labelFont);
    addLabels("http://www.restranger.com",
                           x, y*windowSize, "color:black;", box);

    x += fm1.width("http://www.restranger.comm");
    labelFont.setWeight(QFont::Normal);
    addLabels("to setup your", x, y*windowSize, "color:black;", box);
    y += 20;
    addLabels("thermostat for remote access",
                           130*windowSize, y*windowSize, "color:black;", box);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(10*windowSize);

    QPushButton *disconnect = addButton("Disconnect from Network",120*windowSize, 230*windowSize, box);
    disconnect->setProperty("button", "disconnect");
    disconnect->setFixedWidth(250*windowSize);

    connect(disconnect , SIGNAL(released()), this,SLOT(buttonReleased()));

    createCancelDone("wifiSetup", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createConnectionSuccesfulPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("CONNECTION SUCCESFUL"), 0, 11*windowSize, box);
    cl->setFixedWidth(windowWidth);
    cl->setAlignment(Qt::AlignHCenter);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(12*windowSize);
    QFontMetrics fm(labelFont);

    addLabels("Please register online for Remote Access",
                           30*windowSize, 65*windowSize, "color:black;", box);

    labelFont.setPointSize(11*windowSize);
    labelFont.setWeight(QFont::Normal);
    int y = 95;
    addLabels("Please visit http://www.restranger.com on your",
                           35*windowSize, (y)*windowSize, "color:black;", box);

    addLabels("tablet, personal computer, or smart phone to setup",
                           20*windowSize, (y+=17)*windowSize, "color:black;", box);

    addLabels("your thermostat for remote access. You will need",
                           35*windowSize, (y+=17)*windowSize, "color:black;", box);

    addLabels("the following information:",
                           100*windowSize, (y+=17)*windowSize, "color:black;", box);

    addLabels("MAC Address:",
                           85*windowSize, (y+=25)*windowSize, "color:black;", box);

    addLabels("00:00:00:00:00",
                           205*windowSize, y*windowSize, "color:black;font-weight:bold;", box);

    addLabels("Thermostat CRC:",
                           55*windowSize, (y+=17)*windowSize, "color:black;", box);

    addLabels("1234", 205*windowSize, y*windowSize, "color:black;font-weight:bold;", box);

    if (firstTime) {
        QPushButton *done = addButton("DONE",390*windowSize, 230*windowSize, box);
        done->setProperty("button", "connectionSuccesful");
        done->setFixedWidth(75*windowSize);

        connect(done , SIGNAL(released()), this,SLOT(doneReleased()));
    } else {
        createCancelSave("connectionSuccesful", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createThermostatControlsPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("THERMOSTAT CONTROLS"), 80*windowSize, 11*windowSize, box);
    cl->setAlignment(Qt::AlignLeft);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(18*windowSize);

    QStringList prefs;
    prefs << tr("Cooling") << tr("Heating");

    QStringList props;
    props << tr("coolingControl") << tr("heatingControl");

    labelFont.setPointSize(14*windowSize);
    int x = 170*windowSize;
    int y = 85* windowSize;

    coolingBox = addCheckBox(prefs[0], x, y, box);
    coolingBox->setProperty("button",props[0]);
    coolingBox->setFixedSize(150*windowSize, 42*windowSize);
    connect(coolingBox , SIGNAL(released()), this, SLOT(setupReleased()));

    y += 60*windowSize;
    heatingBox = addCheckBox(prefs[1], x, y, box);
    heatingBox->setProperty("button",props[1]);
    heatingBox->setFixedSize(150*windowSize, 42*windowSize);
    connect(heatingBox , SIGNAL(released()), this, SLOT(setupReleased()));

    if (thermostatControls == 0) {
        coolingBox->setChecked(false);
        heatingBox->setChecked(false);
    } else if (thermostatControls == 1) {
        coolingBox->setChecked(true);
        heatingBox->setChecked(false);
    } else if (thermostatControls == 2) {
        coolingBox->setChecked(false);
        heatingBox->setChecked(true);
    } else {
        coolingBox->setChecked(true);
        heatingBox->setChecked(true);
    }

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);
        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "thermostatControlsBack");

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "thermostatControlsNext");
        next->setFixedWidth(75*windowSize);

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        createCancelSave("thermostatControls", box);
    }

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createFanControlPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("FAN CONTROL"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Thermostat") << tr("Heating");

    QStringList props;
    props << "thermostat" << "heating";

    fanControlButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 134*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        fanControlButtons[i].setParent(box);
        fanControlButtons[i].setText(prefs[i]);
        fanControlButtons[i].setFont(labelFont);
        fanControlButtons[i].setProperty("button", props[i]);
        fanControlButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);

        if (fanControl == props[i]) {
            fanControlButtons[i].setObjectName("");
        } else {
            fanControlButtons[i].setObjectName("butoff");
        }

        y += 75*windowSize;
        fanControlButtons[i].show();
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<prefs.length();j++) {
        cg->addButton(&fanControlButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(fanControlReleased(QAbstractButton*)));

    createCancelSave("fanControl", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createChangeOverValvePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("CHANGEOVER VALVE"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Cooling Changeover") << tr("Heating Changeover");

    QStringList props;
    props << "changeoverValveCooling" << "changeoverValveHeating";

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 110*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(280*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 75*windowSize;
    }

    if (changeOverValve == "changeoverValveCooling") {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

    createCancelSave("changeOverValve", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createBackupHeatPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("BACKUP HEAT"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("Yes") << tr("No");

    QStringList props;
    props << "backupHeatYes" << "backupHeatNo";

    backupHeatButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 160*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        backupHeatButtons[i].setParent(box);
        backupHeatButtons[i].setText(prefs[i]);
        backupHeatButtons[i].setProperty("button", props[i]);
        backupHeatButtons[i].setObjectName("butoff");
        backupHeatButtons[i].setFont(labelFont);
        backupHeatButtons[i].setGeometry(x, y, 180*windowSize, 50*windowSize);
        y += 75*windowSize;
    }

    if (backupHeat == "yes") {
        backupHeatButtons[0].setObjectName("");
    } else {
        backupHeatButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&backupHeatButtons[i]);
        backupHeatButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(backupHeatReleased(QAbstractButton*)));

    createCancelSave("backupHeat", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createCoolingStagesPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("COOLING STAGES"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("1 Stage") << tr("2 Stages");

    QStringList props;
    props << "cooling1stage" << "cooling2stage";

    coolingStagesButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        coolingStagesButtons[i].setParent(box);
        coolingStagesButtons[i].setText(prefs[i]);
        coolingStagesButtons[i].setProperty("button", props[i]);
        coolingStagesButtons[i].setObjectName("butoff");
        coolingStagesButtons[i].setFont(labelFont);
        coolingStagesButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        y += 75*windowSize;
    }
    if (coolingStages == 1) {
        coolingStagesButtons[0].setObjectName("");
    } else {
        coolingStagesButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&coolingStagesButtons[i]);
        coolingStagesButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(coolingStagesReleased(QAbstractButton*)));

    createCancelSave("coolingStages", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createHeatingStagesPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);

    QLabel *hl = addHeaderLabel(tr("HEATING STAGES"), 0, 11*windowSize, box);
    hl->setFixedWidth(windowWidth);
    hl->setAlignment(Qt::AlignHCenter);

    QStringList prefs;
    prefs  << tr("1 Stage") << tr("2 Stages");

    QStringList props;
    props << "heating1stage" << "heating2stage";

    heatingStagesButtons = new QPushButton[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        heatingStagesButtons[i].setParent(box);
        heatingStagesButtons[i].setText(prefs[i]);
        heatingStagesButtons[i].setProperty("button", props[i]);
        heatingStagesButtons[i].setObjectName("butoff");
        heatingStagesButtons[i].setFont(labelFont);
        heatingStagesButtons[i].setGeometry(x, y, 230*windowSize, 50*windowSize);
        y += 75*windowSize;
    }
    if (heatingStages == 1) {
        heatingStagesButtons[0].setObjectName("");
    } else {
        heatingStagesButtons[1].setObjectName("");
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int i=0;i<prefs.length();i++) {
        cg->addButton(&heatingStagesButtons[i]);
        heatingStagesButtons[i].show();
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(heatingStagesReleased(QAbstractButton*)));

    createCancelSave("heatingStages", box);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createDisconnectPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *ed = addHeaderLabel(tr("WIFI DISCONNECT"), 0, 11*windowSize, box);
    ed->setFixedWidth(windowWidth);
    ed->setAlignment(Qt::AlignHCenter);

    labelFont.setPointSize(14*windowSize);
    int x = 100*windowSize;
    QPushButton *yes = addButton(tr("YES"), x, 125*windowSize, box);
    yes->setProperty("button","disconnectYes");
    yes->setFixedSize(125*windowSize, 50*windowSize);

    x += 170*windowSize;
    QPushButton *no = addButton(tr("NO"), x, 125*windowSize, box);
    no->setProperty("button","disconnectNo");
    no->setFixedSize(125*windowSize, 50*windowSize);

    connect(yes , SIGNAL(released()), this,SLOT(buttonReleased()));
    connect(no , SIGNAL(released()), this,SLOT(buttonReleased()));

    box->setVisible(false);

    return box;
}

// Utility
void Ranger::createCancelDone(QString prop, QGroupBox *box) {
    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    if (prop != "preferences" && prop != "setup") {
        QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
        cancel->setProperty("button", prop);
        connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    }

    QPushButton *done = addButton(tr("DONE"), 390*windowSize, 230*windowSize, box);
    done->setProperty("button", prop);
    done->setFixedWidth(75*windowSize);

    connect(done , SIGNAL(released()), this,SLOT(doneReleased()));
}

void Ranger::createCancelSave(QString prop, QGroupBox *box) {
    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", prop);

    QPushButton *save = addButton(tr("SAVE"), 390*windowSize, 230*windowSize, box);
    save->setProperty("button", prop);
    save->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(save , SIGNAL(released()), this,SLOT(saveReleased()));
}

void Ranger::ssBoxChanged(QString ss) {
    //qDebug() << "ssBoxChanged"<<ss;
    if (ss == "off") {
        screenTimer->stop();
    } else {
        screenSaverTime = ss.toInt();
        //qDebug() << "ssBoxChanged:active"<<screenTimer->isActive();
        screenTimer->start(60000);
    }
}

void Ranger::calendarChanged() {}

void Ranger::dateChanged(QDate dt) {
    //qDebug() << "dateChanged" << dt.toString();
    dayLabel->setText(dt.toString("ddd M/d/yy"));
    currentDate = dt;
}

void Ranger::timeChanged(QTime tm) {
    //qDebug() << "timeChanged" << tm.toString();
    timeLabel->setText(tm.toString("hh:mm A"));
    currentTime = tm;
}

void Ranger::idleClicked() {
    //qDebug() << "idleClicked"<<keyboardLockout <<lockCode;
    screenSaverCount = 0;
    idlePanel->setVisible(false);
    if (keyboardLockout == "keyboardLocked") {
        screenLocked = true;
        //lockPanel = createLockPanel();
        lockPanel->setVisible(true);
    } else {
        currentPanel->setVisible(true);
        screenTimer->start(60000);
    }
}

// ACTION LISTENERS
void Ranger::buttonReleased() {      // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() <<"buttonReleased" << p<<selectedRoom;

    if (p == "abcButton") {
        if (saveShift != NULL) {
            saveShift->setStyleSheet(offButton);
        }
        numberButton->setObjectName("key");
        numericKeyboardPanel->setVisible(false);
        alphaKeyboardPanel->setVisible(true);
        currentPanel = alphaKeyboardPanel;
    } else if (p == "addNewLocation"){     // FIX ME
        addNewLocation = true;
        thermoNameText = "";
        keyboardText = "";
        thermostatLocationPanel->setVisible(false);
        keyboardEditText = "newLocation";

        setPropertyNames(keyboardEditText);
        keyboardEdit->setText(keyboardText);
        keyboardEdit->setVisible(true);
        alphaKeyboardPanel->setVisible(true);
        editThermoNamePanel->setVisible(true);
        currentPanel = editThermoNamePanel;
    } else if (p == "changeName"){     // FIX ME
        keyboardText = thermostats[selectedRoom].getRoom().simplified();
        keyboardEditText = "locationName";
        setPropertyNames(keyboardEditText);
        keyboardEdit->setText(keyboardText);

        editRoomPanel->setVisible(false);
        keyboardEdit->setVisible(true);
        editThermoNamePanel->setVisible(true);
        alphaKeyboardPanel->setVisible(true);
        currentPanel = editThermoNamePanel;
        addNewLocation = false;
    } else if (p == "deleteNo"){
        deleteThermoPanel->setVisible(false);
        //editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "delete_thermostat"){
        editRoomPanel->setVisible(false);
        //deleteThermoPanel = createDeleteThermoPanel();
        deleteThermoPanel->setVisible(true);
        currentPanel = deleteThermoPanel;
    } else if (p == "deleteYes"){ //   FIX ME
        thermostats.remove(selectedRoom);
        deleteThermoPanel->setVisible(false);
        //thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "disconnect") {
        wifiSetupPanel->setVisible(false);
//        disconnectPanel = createDisconnectPanel();
        disconnectPanel->setVisible(true);
        currentPanel = disconnectPanel;
    } else if (p == "disconnectYes"){
        disconnectPanel->setVisible(false);
        //wifiPanel = createWifiPanel();
        wifiPanel->setVisible(true);
        currentPanel = wifiPanel;
    } else if (p == "disconnectNo"){
        disconnectPanel->setVisible(false);
        //setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "later"){
        firstTime = 0;
        wifiPanel->setVisible(false);
        //mainPanel = createMainPanel();
        hvacTypePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        //writeConfig(configFile);
    } else if (p == "lockCodeNo"){
        lockErrorPanel->setVisible(false);
//        idlePanel = createIdlePanel();
        idlePanel->setVisible(true);
    } else if (p == "lockCodeYes"){        // FIX ME
        lockErrorPanel->setVisible(false);
        screenLocked = true;
        //lockPanel = createLockPanel();
        lockPanel->setVisible(true);
    } else if (p == "myNetwork") { //   FIX ME
        keyboardText =  wifiPasswordText;
        wifiPanel->setVisible(false);
        setPropertyNames("wifiPassword");
        keyboardEdit->setText(keyboardText);

        wifiPasswordPanel->setVisible(true);
        keyboardEdit->setVisible(true);
        alphaKeyboardPanel->setVisible(true);
        currentPanel = wifiPasswordPanel;
    } else if (p == "numButton"){
        if (saveShift != NULL) {
            saveShift->setStyleSheet(offButton);
        }
        abcButton->setObjectName("key");
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(true);
        currentPanel = numericKeyboardPanel;
    } else if (p == "settings"){
        mainPanel->setVisible(false);
        hvacTypePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "weekButton") {
       if (selectedDay == "WEEKEND"){
           selectedDay = tr("WEEKDAY");
           weekButton->setText(tr("WEEKEND"));
       } else {
           selectedDay = tr("WEEKEND");
           weekButton->setText(tr("WEEKDAY"));
       }
       editScehduleHeaderLabel->setText(selectedDay);
       loadSchedules();
//       QStringList props;
//       props << "all days" << "weekend" << "monday" << "vacation";

//       for (int i=0;i<props.length();i++) {
//           if (selectedDay.toLower() == props[i]) {
//               scheduleButtons[i].setObjectName("");
//           } else if (selectedSchedule == props[i]) {
//               scheduleButtons[i].setObjectName("butoffDefault");
//           } else {
//               scheduleButtons[i].setObjectName("butoff");
//           }
//       }
//       setStyleSheet(rangerStyleSheet);
    }

    restartTimer();
}

void Ranger::prefsReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() <<"prefsReleased"<<p;
    QString style = "padding-left:4px;padding-right:4px;border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;";

    if (p == "2perday" || p == "4perday") {  // FIX ME
        if (p == "2perday") {
            schedulePeriods = 2;
        } else {
            schedulePeriods = 4;
         }
        schedulePeriodsPanel->setVisible(false);
        //schedulePeriodsPanel = createSchedulePeriodsPanel();
        schedulePeriodsPanel->setVisible(true);
        currentPanel = schedulePeriodsPanel;
    } else if (p == "12hour" || p == "24hour") {  // FIX ME
        if (p == "12hour") {
            clockFormat = 12;
        } else {
            clockFormat = 24;
         }
        clockFormatPanel->setVisible(false);
//        clockFormatPanel = createClockFormatPanel();
        clockFormatPanel->setVisible(true);
        currentPanel = clockFormatPanel;
    } else if (p == "clockFormat") {
        preferencesPanel->setVisible(false);
//        clockFormatPanel = createClockFormatPanel();
        clockFormatPanel->setVisible(true);
        currentPanel = clockFormatPanel;
    } else if (p == "dst") {
        preferencesPanel->setVisible(false);
//        dstPanel = createDstPanel();
        dstPanel->setVisible(true);
        currentPanel = dstPanel;
    } else if (p == "dstOn" || p == "dstOff") {  // FIX ME
        if (p == "dstOff") {
            dst = false;
        } else {
            dst = true;
        }
        dstPanel->setVisible(false);
        //dstPanel = createDstPanel();
        dstPanel->setVisible(true);
        currentPanel = dstPanel;
    } else if (p == "F" || p == "C") {  // FIX ME
        tempuratureScale = (p == "C");
        tempuratureScalePanel->setVisible(false);
        //tempuratureScalePanel = createTempuratureScalePanel();
        tempuratureScalePanel->setVisible(true);
        currentPanel = tempuratureScalePanel;
    } else if (p == "keyboardUnlocked" || p == "keyboardPartiallyUnlocked" || p == "keyboardLocked") {  // FIX ME
        if (p == "keyboardUnlocked") {
           keyboardLockout = "unlocked";
        } else if (p == "keyboardPartiallyUnlocked") {
            keyboardLockout = "partial";
         } else {
            keyboardLockout = "locked";
        }

        keyboardLockoutPanel->setVisible(false);
        //keyboardLockoutPanel = createKeyboardLockoutPanel();
        keyboardLockoutPanel->setVisible(true);
        currentPanel = keyboardLockoutPanel;
    } else if (p.startsWith("keyboardLockout")) {
        preferencesPanel->setVisible(false);
//        keyboardLockoutPanel = createKeyboardLockoutPanel();
        keyboardLockoutPanel->setVisible(true);
        currentPanel = keyboardLockoutPanel;
    } else if (p == "manualChangeover" || p == "automaticChangeover") {  // FIX ME
        if (p == "manualChangeover") {
            systemsChangeOver = "manual";
        } else  {
            systemsChangeOver = "automatic";
        }
        systemsChangeOverPanel->setVisible(false);
        //systemsChangeOverPanel = createSytemsChangeOverPanel();
        systemsChangeOverPanel->setVisible(true);
        currentPanel = systemsChangeOverPanel;
    } else if (p == "non-programmable" || p == "programmable") {  // FIX ME
        schedulingOption = p;
        schedulingOptionsPanel->setVisible(false);
        //schedulingOptionsPanel = createSchedulingOptionsPanel();
        schedulingOptionsPanel->setVisible(true);
        currentPanel = schedulingOptionsPanel;
    } else if (p == "schedulePeriods") {
        preferencesPanel->setVisible(false);
//        schedulePeriodsPanel = createSchedulePeriodsPanel();
        schedulePeriodsPanel->setVisible(true);
        currentPanel = schedulePeriodsPanel;
    } else if (p == "schedulingOptions") {
        preferencesPanel->setVisible(false);
        //schedulingOptionsPanel = createSchedulingOptionsPanel();
        schedulingOptionsPanel->setVisible(true);
        currentPanel = schedulingOptionsPanel;
//    } else if (p == "automatic") {
//        schedulingOption = tr("Automatic");
//        schedText->setText(schedulingOption);
//        schedText->setStyleSheet(style);
//        schedulingOptionsPanel->setVisible(false);
//        preferencesPanel->setVisible(true);
//        currentPanel = preferencesPanel;
//    } else if (p == "manual") {
//        schedulingOption = tr("Manual");
//        schedText->setText(schedulingOption);
//        schedText->setStyleSheet(style);
//        schedulingOptionsPanel->setVisible(false);
//        preferencesPanel->setVisible(true);
//        currentPanel = preferencesPanel;
//    } else if (p == "systemType") {
//        setupPanel->setVisible(false);
//        systemTypePanel = createSystemTypePanel();
//        systemTypePanel->setVisible(true);
//        currentPanel = systemTypePanel;
    } else if (p == "systemsChangeOver") {
        preferencesPanel->setVisible(false);
//        systemsChangeOverPanel = createSytemsChangeOverPanel();
        systemsChangeOverPanel->setVisible(true);
        currentPanel = systemsChangeOverPanel;
    } else if (p == "thermostatControls") {
        setupPanel->setVisible(false);
        //thermostatControlsPanel = createThermostatControlsPanel();
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "tempuratureLimits") {
        preferencesPanel->setVisible(false);
//        tempuratureLimitsPanel = createTempuratureLimitsPanel();
        tempuratureLimitsPanel->setVisible(true);
        currentPanel = tempuratureLimitsPanel;
    } else if (p == "tempuratureScale") {
        preferencesPanel->setVisible(false);
//        tempuratureScalePanel = createTempuratureScalePanel();
        tempuratureScalePanel->setVisible(true);
        currentPanel = tempuratureScalePanel;
     }

    restartTimer();
}

void Ranger::setupReleased() {       // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() <<"setupReleased"<<p;

    if (p == "backupHeat") {
        setupPanel->setVisible(false);
        backupHeatPanel->setVisible(true);
        currentPanel = backupHeatPanel;
    } else if (p == "backupHeatYes" || p == "backupHeatNo") {   // FIX ME
        backupHeat = (p == "backupHeatYes") ? "yes" : "no";
        backupHeatPanel->setVisible(false);
        //backupHeatPanel = createBackupHeatPanel();
        backupHeatPanel->setVisible(true);
        currentPanel = backupHeatPanel;
    } else if (p == "changeOverValve") {
        setupPanel->setVisible(false);
        //changeOverValvePanel = createChangeOverValvePanel();
        changeOverValvePanel->setVisible(true);
        currentPanel = changeOverValvePanel;
    } else if (p == "changeoverValveCooling" || p == "changeoverValveHeating") {  // FIX ME
        changeOverValve = (p == "changeoverValveCooling") ? "cool" : "heat";
        changeOverValvePanel->setVisible(false);
        //changeOverValvePanel = createChangeOverValvePanel();
        changeOverValvePanel->setVisible(true);
        currentPanel = changeOverValvePanel;
    } else if (p == "coolingStages") {
        setupPanel->setVisible(false);
        coolingStagesPanel->setVisible(true);
        currentPanel = coolingStagesPanel;
    } else if (p == "cooling1stage" || p == "cooling2stage") {  // FIX ME
        coolingStages = (p == "cooling1stage") ? 1 : 2;
        coolingStagesPanel->setVisible(false);
        //coolingStagesPanel = createCoolingStagesPanel();
        coolingStagesPanel->setVisible(true);
        currentPanel = coolingStagesPanel;
    } else if (p == "english" || p == "spanish") {
        language = p;
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "fanControl") {
        setupPanel->setVisible(false);
        fanControlPanel->setVisible(true);
        currentPanel = fanControlPanel;
    } else if (p == "forcedAir" || p == "heatPump" || p == "waterOrSteam") {   // FIX ME
        if (p  == "forcedAir") {
            hvacType = "air";
        } else if (p  == "heatPump") {
            hvacType = "heat";
        } else {
            hvacType = "water";
        }
        hvacTypePanel->setVisible(false);
        hvacTypePanel = createSystemTypePanel();
        hvacTypePanel->setVisible(true);
        currentPanel = hvacTypePanel;
    } else if (p == "heatingControl" || p == "coolingControl") {
        if (!coolingBox->isChecked() && !heatingBox->isChecked()) {
            thermostatControls = 0;
        } else if (coolingBox->isChecked() && !heatingBox->isChecked()) {
            thermostatControls = 1;
        } else if (!coolingBox->isChecked() && heatingBox->isChecked()) {
            thermostatControls = 2;
        } else if (coolingBox->isChecked() && heatingBox->isChecked()) {
            thermostatControls = 3;
        }
    } else if (p == "heatingStages") {
        setupPanel->setVisible(false);
        heatingStagesPanel->setVisible(true);
        currentPanel = heatingStagesPanel;
    } else if (p == "heating1stage" || p == "heating2stage") {  // FIX ME
        heatingStages = (p == "heating1stage") ? 1 : 2;
        heatingStagesPanel->setVisible(false);
       // heatingStagesPanel = createHeatingStagesPanel();
        heatingStagesPanel->setVisible(true);
        currentPanel = heatingStagesPanel;
    } else if (p == "hvacType") {
        setupPanel->setVisible(false);
        hvacTypePanel->setVisible(true);
        currentPanel = hvacTypePanel;
    } else if (p == "language") {
        setupPanel->setVisible(false);
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "thermostat" || p == "heating") {   // FIX ME
        fanControl = p;
        fanControlPanel->setVisible(false);
        fanControlPanel->setVisible(true);
        currentPanel = fanControlPanel;
    } else if (p == "thermostatControls") {
        setupPanel->setVisible(false);
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "thermostatLocation") {
        setupPanel->setVisible(false);
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "wifiSetup") {
        setupPanel->setVisible(false);
        wifiSetupPanel->setVisible(true);
        currentPanel = wifiSetupPanel;
    }

    restartTimer();
}

void Ranger::profileReleased() {         // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() <<"profileReleased" << p;

    if (p == "firstNameEdit"){
        profilePanel->setVisible(false);
        registerPanel->setVisible(true);
        currentPanel = registerPanel;
    } else if (p == "dstOn" || p == "dstOff"){
        dst = (p == "dstOn");
    } else if (p == "screenSaver"){
    } else if (p == "editThermo"){
        profilePanel->setVisible(false);
        thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "customerName"){
        keyboardText = customerNameText;
        keyboardEditText = "customerName";

        headerFont.setWeight(QFont::DemiBold);
        QFontMetrics f2(headerFont);
        QString txt = "FIRST NAME";

        registerHeaderLabel->setText(tr("FIRST NAME"));

        int w2 = f2.width(txt);
        int xw = (windowWidth/2)-((w2)/2);

        registerHeaderLabel->move(xw + 5*windowSize, 11*windowSize);
        for (int i=10;i<27;i++) {
            numericKeyButtons[i]->setDisabled(false);
        }
        abcButton->setDisabled(false);

        setPropertyNames(keyboardEditText);

        profilePanel->setVisible(false);
        registerPanel->setVisible(true);
        keyboardEdit->setText(keyboardText);
        keyboardEdit->setCursorPosition(keyboardText.length());
        keyboardEdit->setVisible(true);
        alphaKeyboardPanel->setVisible(true);
        currentPanel = registerPanel;
    } else if (p == "zipCode"){
        keyboardText = zipCodeText;
        keyboardEditText = "zipCode";
        registerHeaderLabel->setText("ZIP CODE");
        setPropertyNames(keyboardEditText);

        headerFont.setWeight(QFont::DemiBold);
        QFontMetrics f2(headerFont);
        QString txt = "ZIP CODE";

        registerHeaderLabel->setText(tr("ZIP CODE"));

        int w2 = f2.width(txt);
        int xw = (windowWidth/2)-((w2)/2);

        registerHeaderLabel->move(xw + 5*windowSize, 11*windowSize);
        for (int i=10;i<27;i++) {
            numericKeyButtons[i]->setDisabled(true);
        }
        abcButton->setDisabled(true);

        profilePanel->setVisible(false);
        registerPanel->setVisible(true);
        keyboardEdit->setText(keyboardText);
        keyboardEdit->setCursorPosition(keyboardText.length());
        keyboardEdit->setVisible(true);
        numericKeyboardPanel->setVisible(true);
        currentPanel = registerPanel;
//    } else {
//        selectedRoom = p;
//        thermoNameText = thermostats[selectedRoom].getRoom().simplified();
//        profilePanel->setVisible(false);
//        editRoomPanel = createEditRoomPanel();
//        editRoomPanel->setVisible(true);
//        currentPanel = editRoomPanel;
//       restartTimer();
    }

   restartTimer();
}

void Ranger::setPropertyNames(QString obj) {
    for (int j=0;j<28;j++) {
        alphaKeyButtons[j]->setProperty("button", obj);
    }

    for (int j=0;j<26;j++) {
        numericKeyButtons[j]->setProperty("button", obj);
    }

    alphaCancel->setProperty("button", obj);
    alphaDone->setProperty("button", obj);
    alphaBack->setProperty("button", obj);

    numericCancel->setProperty("button", obj);
    numericDone->setProperty("button", obj);
    numericBack->setProperty("button", obj);
}

void Ranger::returnPressed() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "returnPressed"<<p;

    if (p == "settings") {
        settingsPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "schedule") {
        settingsSchedulePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "setup") {
        setupPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    }

   restartTimer();
}

void Ranger::upPressed() {
    ((QPushButton*)sender())->setIcon(*upIconPressed);
}

void Ranger::upReleased() {
    ((QPushButton*)sender())->setIcon(*upIcon);
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "upReleased"<<p;

    if (p == "heatToDegrees") {
        heatToDegreesLabel->setText(QString::number(++heatToDegreesTemp));
        heatLabel->setText(QString::number(heatToDegreesTemp) + degreeCode.at(degreeCode.length()-1));
    } else if (p == "coolToDegrees"){
        coolToDegreesLabel->setText(QString::number(++coolToDegreesTemp));
        coolLabel->setText(QString::number(coolToDegreesTemp) + degreeCode.at(degreeCode.length()-1));
    } else if (p == "coolToHour"){
        coolToHourTemp++;
        if (coolToHourTemp == 13) coolToHourTemp = 1;
        coolToTimeLabel->setText(QString("%1:%2").arg(QString::number(coolToHourTemp),
                                 QString::number(coolToMinuteTemp).rightJustified(2, '0')));
    } else if (p == "coolToMinute"){
        coolToMinuteTemp++;
        if (coolToMinuteTemp == 60) coolToMinuteTemp = 0;
        coolToTimeLabel->setText(QString("%1:%2").arg(QString::number(coolToHourTemp),
                                 QString::number(coolToMinuteTemp).rightJustified(2, '0')));
    } else if (p == "coolToAmPm"){
        if (coolToAmPmTemp == "AM") {
            coolToAmPmTemp = "PM";
        } else {
            coolToAmPmTemp = "AM";
        }
        coolToAmPmLabel->setText(coolToAmPmTemp);
     } else if (p == "heatToHour"){
        heatToHourTemp++;
        if (heatToHourTemp == 13) heatToHourTemp = 1;
        QString tm = QString("%1:%2").arg(QString::number(heatToHourTemp),
                              QString::number(heatToMinuteTemp).rightJustified(2, '0'));
        heatToTimeLabel->setText(tm);
    } else if (p == "heatToMinute"){
        heatToMinuteTemp++;
        if (heatToMinuteTemp == 60) heatToMinuteTemp = 0;
        QString tm = QString("%1:%2").arg(QString::number(heatToHourTemp),
                              QString::number(heatToMinuteTemp).rightJustified(2, '0'));
        heatToTimeLabel->setText(tm);
    } else if (p == "heatToAmPm"){
        if (heatToAmPmTemp == "AM") {
            heatToAmPmTemp = "PM";
        } else {
            heatToAmPmTemp = "AM";
        }
        heatToAmPmLabel->setText(heatToAmPmTemp);
    } else if (p == "sched"){
        if (schedText->text() == "Automatic") {
            schedText->setText("Manual");
        } else {
            schedText->setText("Automatic");
         }
    } else if (p == "main"){
        tempLabel->setText(QString::number(++statControl) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "cool") {
            coolToDegrees = statControl;
            coolLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        } else if (selectedMode == "heat") {
            heatToDegrees = statControl;
            heatLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
         }
        saveConfig(configFile);
    } else if (p == "editSchedule"){
        if (currentIndex == 0) {
            if (periodCounter == 3)  periodCounter = -1;
            periodLabel->setText(periodList[++periodCounter]);
            currentDailySchedule = currentSchedule[periodLabel->text()];
            startLabel->setText(currentDailySchedule.getStartTime());
            stopLabel->setText(currentDailySchedule.getStopTime());
            heatToLabel->setText(QString::number(currentDailySchedule.getHeatTo())+degreeCode.at(degreeCode.length()-1));
            coolToLabel->setText(QString::number(currentDailySchedule.getCoolTo())+degreeCode.at(degreeCode.length()-1));
        }
        if (currentIndex == 1) {
            QDateTime dt = QDateTime::fromString(startLabel->text(),"h:mma").addSecs(60);
            startLabel->setText(dt.toString("h:mma"));
            currentDailySchedule.setStartTime(dt.toString("h:mma"));
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
         }
        if (currentIndex == 2) {
            QDateTime dt = QDateTime::fromString(stopLabel->text(),"h:mma").addSecs(60);
            stopLabel->setText(dt.toString("h:mma"));
            currentDailySchedule.setStopTime(dt.toString("h:mma"));
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
        }
        if (currentIndex == 3) {
            int d = currentDailySchedule.getHeatTo() + 1;
            heatToLabel->setText(QString::number(d) + degreeCode.at(degreeCode.length()-1));
            currentDailySchedule.setHeatTo(d);
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
        }
        if (currentIndex == 4) {
            int d = currentDailySchedule.getCoolTo() + 1;
            coolToLabel->setText(QString::number(d) + degreeCode.at(degreeCode.length()-1));
            currentDailySchedule.setCoolTo(d);
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
        }
    }

    restartTimer();
}

void Ranger::downPressed() {
    ((QPushButton*)sender())->setIcon(*dnIconPressed);
}

void Ranger::downReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "downReleased"<<p;
    ((QPushButton*)sender())->setIcon(*dnIcon);

    if (p == "heatToDegrees") {
        heatToDegreesLabel->setText(QString::number(++heatToDegreesTemp));
        heatLabel->setText(QString::number(heatToDegreesTemp) + degreeCode.at(degreeCode.length()-1));
    } else if (p == "coolToDegrees"){
        coolToDegreesLabel->setText(QString::number(--coolToDegreesTemp));
        coolLabel->setText(QString::number(coolToDegreesTemp) + degreeCode.at(degreeCode.length()-1));
    } else if (p == "coolToHour"){
        coolToHourTemp--;
        if (coolToHourTemp == 0) coolToHourTemp = 12;
        coolToTimeLabel->setText(QString("%1:%2").arg(QString::number(coolToHourTemp),
                                 QString::number(coolToMinuteTemp).rightJustified(2, '0')));
    } else if (p == "coolToMinute"){
        coolToMinuteTemp--;
        if (coolToMinuteTemp == -1) coolToMinuteTemp = 59;
        coolToTimeLabel->setText(QString("%1:%2").arg(QString::number(coolToHourTemp),
                                 QString::number(coolToMinuteTemp).rightJustified(2, '0')));
    } else if (p == "coolToAmPm"){
        if (coolToAmPmTemp == "AM") {
            coolToAmPmTemp = "PM";
        } else {
            coolToAmPmTemp = "AM";
        }
        coolToAmPmLabel->setText(coolToAmPmTemp);
     } else if (p == "heatToHour"){
        heatToHourTemp--;
        if (heatToHourTemp == 0) heatToHourTemp = 12;
        QString tm = QString("%1:%2").arg(QString::number(heatToHourTemp),
                              QString::number(heatToMinuteTemp).rightJustified(2, '0'));
        heatToTimeLabel->setText(tm);
    } else if (p == "heatToMinute"){
        heatToMinuteTemp--;
        if (heatToMinuteTemp == -1) heatToMinuteTemp = 59;
        QString tm = QString("%1:%2").arg(QString::number(heatToHourTemp),
                              QString::number(heatToMinuteTemp).rightJustified(2, '0'));
        heatToTimeLabel->setText(tm);
    } else if (p == "heatToAmPm"){
        if (heatToAmPmTemp == "AM") {
            heatToAmPmTemp = "PM";
        } else {
            heatToAmPmTemp = "AM";
        }
        heatToAmPmLabel->setText(heatToAmPmTemp);
    } else if (p == "main"){
        tempLabel->setText(QString::number(--statControl) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "cool") {
            coolToDegrees = statControl;
            coolLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        } else if (selectedMode == "heat") {
            heatToDegrees = statControl;
            heatLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
         }
        saveConfig(configFile);
    } else if (p == "sched"){
        if (schedText->text() == "Automatic") {
            schedText->setText("Manual");
        } else {
            schedText->setText("Automatic");
         }
    }

    restartTimer();
}

void Ranger::upNumPressed() {
    ((QPushButton*)sender())->setIcon(*upIconPressed);
}

void Ranger::upNumReleased() {
    ((QPushButton*)sender())->setIcon(*upIcon);
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "upNumReleased"<<p;

    if (p.startsWith("lock")) {
        int ic = QString(p.at(4)).toInt();
        if (lockCodes[ic] == 9) lockCodes[ic] = -1;
        lockCodes[ic] += 1;
        lockNums[ic]->setText(QString::number(lockCodes[ic]));
    } else if (p == "hour") {
        if (currentHour == 12) currentHour = 0;
        hourText->setText(QString::number(++currentHour));
    } else if (p == "minute") {
        if (currentMinute == 59) currentMinute = 0;
        minuteText->setText(QString::number(++currentMinute));
    } else if (p == "ampm") {
        if (currentAmPm == "AM") {
            currentAmPm = "PM";
        } else {
            currentAmPm = "AM";
        }
        ampmText->setText(currentAmPm);
    } else if (p == "month") {
        if (currentMonth == 12) currentMonth = 0;
        monthText->setText(months.at(++currentMonth));
    } else if (p == "day") {
        currentDate = currentDate.addDays(1);
        currentDay = currentDate.day();
        dayText->setText(QString::number(currentDay));
    } else if (p == "year") {
        yearText->setText(QString::number(++currentYear));
    }

    restartTimer();
}

void Ranger::downNumPressed() {
    ((QPushButton*)sender())->setIcon(*dnIconPressed);
}

void Ranger::downNumReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "downNumReleased"<<p;
    ((QPushButton*)sender())->setIcon(*dnIcon);

    if (p.startsWith("lock")) {
        int ic = QString(p.at(4)).toInt();
        if (lockCodes[ic] == 0) lockCodes[ic] = 10;
        lockCodes[ic] -= 1;
        lockNums[ic]->setText(QString::number(lockCodes[ic]));
    } else if (p == "hour") {
        if (currentHour == 1) currentHour = 13;
        hourText->setText(QString::number(--currentHour));
    } else if (p == "minute") {
        if (currentMinute == 1) currentMinute = 60;
        minuteText->setText(QString::number(--currentMinute));
    } else if (p == "ampm") {
        if (currentAmPm == "AM") {
            currentAmPm = "PM";
        } else {
            currentAmPm = "AM";
        }
        ampmText->setText(currentAmPm);
    } else if (p == "month") {
        if (currentMonth == 1) currentMonth = 13;
        monthText->setText(months.at(--currentMonth));
    } else if (p == "day") {
        currentDate = currentDate.addDays(-1);
        currentDay = currentDate.day();
        dayText->setText(QString::number(currentDay));
    } else if (p == "year") {
        yearText->setText(QString::number(--currentYear));
   }

    restartTimer();
}

void Ranger::backReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "backReleased"<<p;

    if (p == "profileBack") {
        profilePanel->setVisible(false);
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "thermostatLocationBack") {
        thermostatLocationPanel->setVisible(false);
         profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "thermostatControlsBack") {
        thermostatControlsPanel->setVisible(false);
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "hvacTypeBack") {
        hvacTypePanel->setVisible(false);
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "wifiBack") {
        wifiPanel->setVisible(false);
        hvacTypePanel->setVisible(true);
        currentPanel = hvacTypePanel;
    }

   restartTimer();
}

void Ranger::nextReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "nextReleased"<<p;

    if (p == "languagesNext1") {
        languagesPanel->setVisible(false);
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "languagesNext") {
        languagesPanel->setVisible(false);
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "thermostatLocationNext") {
        thermostatLocationPanel->setVisible(false);
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "thermostatControlsNext") {
        thermostatControlsPanel->setVisible(false);
        hvacTypePanel->setVisible(true);
        currentPanel = hvacTypePanel;
    } else if (p == "hvacTypeNext") {
        hvacTypePanel->setVisible(false);
        wifiPanel->setVisible(true);
        currentPanel = wifiPanel;
    }

    restartTimer();
}

void Ranger::cancelReleased() {         // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "cancelReleased"<< p;

    if (p == "backupHeat") {
        backupHeatPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "cancelEdit") {
        thermostatLocationPanel->setVisible(false);
        //registerPanel = createRegisterPanel("name", true);        // FIX ME
        registerPanel->setVisible(true);
        currentPanel = registerPanel;
    } else if (p == "changeOverValve") {
        changeOverValvePanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "clockFormat") {
        clockFormatPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "colors") {
        setStyleSheet(rangerStyleSheet);
        colorsPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "coolingStages") {
        coolingStagesPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "coolTo") {
        coolPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "customerName") {
        registerPanel->setVisible(false);
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "dailySchedule") {
        dailySchedulePanel->setVisible(false);
        //settingsSchedulePanel = createSettingsSchedulePanel();
        settingsSchedulePanel->setVisible(true);
        currentPanel = settingsSchedulePanel;
    } else if (p == "deleteThermo") {
        deleteThermoPanel->setVisible(false);
        //editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "dst") {
        dstPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "editRoomCancel") {
        editRoomPanel->setVisible(false);
        //thermostatLocationPanel  = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "editSchedule") {
        editSchedulePanel->setVisible(false);
        if (selectedDay == "ALL DAYS" || selectedDay.startsWith("WEEK") ||
                selectedDay == "VACATION") {
            settingsSchedulePanel->setVisible(true);
            currentPanel = settingsSchedulePanel;
        } else {
            dailySchedulePanel->setVisible(true);
            currentPanel = dailySchedulePanel;
        }

        QStringList props;
        props << "all days" << "weekend" << "monday" << "vacation";

        for (int i=0;i<props.length();i++) {
            if (selectedDay.toLower() == props[i] || (selectedDay.toLower() == "weekday" && i == 1)) {
                scheduleButtons[i].setObjectName("");
            } else if (selectedSchedule == props[i]) {
                scheduleButtons[i].setObjectName("butoffDefault");
            } else {
                scheduleButtons[i].setObjectName("butoff");
            }
        }
        setStyleSheet(rangerStyleSheet);
    } else if (p == "editRoom") {
        editRoomPanel->setVisible(false);
        if (firstTime) {
           // thermostatLocationPanel = createThermostatLocationPanel();
            thermostatLocationPanel->setVisible(true);
            currentPanel = thermostatLocationPanel;
        } else {
            //profilePanel = createProfilePanel();
            profilePanel->setVisible(true);
            currentPanel = profilePanel;
        }
    } else if (p == "fanControl") {
        fanControlPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "fanMode") {
        fanModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "heatingStages") {
        heatingStagesPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "heatTo") {
        heatPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "keyboardLockout") {
        keyboardLockoutPanel->setVisible(false);
        //preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "language") {
        languagesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "locationName") {
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "lock") {
        lockPanel->setVisible(false);
        //settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = lockPanel;
    } else if (p == "profile") {
        profilePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "systemType") {
        hvacTypePanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = hvacTypePanel;
    } else if (p == "selectMode") {
        selectModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "setup") {
        setupPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "schedulingOptions") {
        schedulingOptionsPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "schedulePeriods") {
        schedulePeriodsPanel->setVisible(false);
        //preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "systemsChangeover") {
        systemsChangeOverPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "tempuratureLimits") {
        tempuratureLimitsPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "tempuratureScale") {
        tempuratureScalePanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "thermostatControls") {
        thermostatControlsPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "thermostatLocationCancel") {
        thermostatLocationPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p.startsWith("thermo")) {
        editThermoNamePanel->setVisible(false);
        //editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "timeDate") {
        timeDatePanel->setVisible(false);
        //settingsPanel  = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "wifiSetup") {
        wifiSetupPanel->setVisible(false);
        //setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "wifi") {
        wifiSetupPanel->setVisible(false);
        //mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "wifiPassword") {
        keyboardEdit->setVisible(false);
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        wifiPasswordPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "zipCode") {
        registerPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    }

    restartTimer();
}

void Ranger::editReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "editReleased"<< p;

    if (p == "coolCancel") {
        coolPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "dailySchedule") {
        loadSchedules();
        dailySchedulePanel->setVisible(false);
        editSchedulePanel->setVisible(true);
        currentPanel = editSchedulePanel;
    } else if (p == "editRoomCancel") {
        editRoomPanel->setVisible(false);
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "fanModeCancel") {
        fanModePanel->setVisible(false);
        //mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "heatCancel") {
        heatPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "selectModeCancel") {
        selectModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    }

   restartTimer();
}

void Ranger::doneReleased() {     // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "doneReleased"<<p<<lockCode<<selectedRoom;

    if (p == "connectionSuccesful") {
        firstTime = 0;
        connectionSuccesfulPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        saveConfig(configFile);
    } else if (p == "customerName") {
        customerNameText = keyboardEdit->text();
        fields[0].setText(customerNameText);
        registerPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "deleteThermo") {
        deleteThermoPanel->setVisible(false);
        //editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "editRoom") {         // FIX ME
        editRoomPanel->setVisible(false);
        if (firstTime) {
            //thermostatLocationPanel = createThermostatLocationPanel();
            thermostatLocationPanel->setVisible(true);
            currentPanel = thermostatLocationPanel;
        } else {
            //profilePanel = createProfilePanel();
            profilePanel->setVisible(true);
            currentPanel = profilePanel;
        }

        //props.insert(selectedRoom, thermoName->text());
        //writeProperties(propertyOutFile);
//    } else if (p == "editSchedule") {             // FIX ME
//        editSchedulePanel->setVisible(false);
//        if (selectedDay == "ALL DAYS" || selectedDay.startsWith("WEEK") ||
//                selectedDay == "VACATION") {
//            settingsSchedulePanel = createSettingsSchedulePanel();
//            settingsSchedulePanel->setVisible(true);
//            currentPanel = settingsSchedulePanel;
//        } else {
//            selectedDaySchedule = currentSchedule;
//            dailySchedules[selectedDay] = currentSchedule;
//            dailySchedulePanel = createDailySchedulePanel();
//            dailySchedulePanel->setVisible(true);
//            currentPanel = dailySchedulePanel;
//        }
    } else if (p == "fanMode") {
        if (selectedFanModeTemp != NULL)  selectedFanMode = selectedFanModeTemp;
        fanLabel->setText(selectedFanMode);
        fanModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "languages") {
        languagesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "locationName") {
        thermostats[selectedRoom].setRoom(keyboardEdit->text().simplified());
        thermostatLocations();
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        setEditRoomHeader(keyboardEdit->text());
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "newLocation") {
        thermostats[selectedRoom].setRoom(keyboardEdit->text().simplified());
        thermostatLocations();
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        setEditRoomHeader(keyboardEdit->text());
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "lock") {
        QString code = lockNums[0]->text() + lockNums[1]->text() + lockNums[2]->text() + lockNums[3]->text();
        lockPanel->setVisible(false);
        if (code == lockCode) {
            currentPanel->setVisible(true);
            screenTimer->start(60000);
        } else {
            lockErrorPanel = createLockErrorPanel();
            lockErrorPanel->setVisible(true);
        }
    } else if (p == "newThermostat") {                 // FIX ME
        editThermoNamePanel->setVisible(false);
        thermoNameText = keyboardEdit->text();
        QMap<QString, Thermostat>::iterator i2;
        QMap<QString, Thermostat> thermos1 = thermostats;
        for (i2=thermos1.begin(); i2 != thermos1.end(); ++i2) {
            Thermostat t = i2.value();
            if (t.getRoom().length() == 0) {
                thermostats[i2.key()].setRoom(thermoNameText);
                break;
             }
        }
        if (firstTime) {
            thermostatLocationPanel = createThermostatLocationPanel();
            thermostatLocationPanel->setVisible(true);
            currentPanel = thermostatLocationPanel;
        } else {
            profilePanel = createProfilePanel();
            profilePanel->setVisible(true);
            currentPanel = profilePanel;
        }
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "profile") {
        profilePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "selectMode") {
        if (selectedModeTemp != NULL) selectedMode = selectedModeTemp;
        modeLabel->setText(selectedMode);
        selectModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "settingsSchedule") {
        settingsSchedulePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "setup") {
        setupPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p.startsWith("thermo")) {
        thermoNameText = keyboardEdit->text();
        thermostats[selectedRoom].setRoom(keyboardEdit->text());
        editThermoNamePanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "timeDate") {
        timeDatePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "wifiPassword") {
        wifiPasswordText = keyboardEdit->text();
        wifiPasswordPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        alphaKeyboardPanel->setVisible(false);
        numericKeyboardPanel->setVisible(false);
        connectionSuccesfulPanel->setVisible(true);
        currentPanel = connectionSuccesfulPanel;
    } else if (p == "wifiSetup") {
        wifiSetupPanel->setVisible(false);
        if (firstTime) {
            mainPanel = createMainPanel();
            mainPanel->setVisible(true);
            currentPanel = mainPanel;
        } else {
            setupPanel = createSetupPanel();
            setupPanel->setVisible(true);
            currentPanel = setupPanel;
        }
    } else if (p == "wifi") {         // FIX ME
        wifiSetupPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "zipCode") {
        zipCodeText = keyboardEdit->text();
        fields[1].setText(zipCodeText);
        registerPanel->setVisible(false);
        keyboardEdit->setVisible(false);
        profilePanel->setVisible(true);
        numericKeyboardPanel->setVisible(false);
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    }

    restartTimer();
}

void Ranger::saveReleased() {     // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "saveReleased"<<p;

    if (p == "alphaname" || p == "numericname") {
        registerPanel->setVisible(false);
        //thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        customerNameText = firstName->text();
        currentPanel = thermostatLocationPanel;
    } else if (p == "alphaprofile" || p == "numericprofile") {
        registerPanel->setVisible(false);
        customerNameText = firstName->text();
        //profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "alphathermo" || p == "numericthermo") {
        editThermoNamePanel->setVisible(false);
        thermoNameText = thermoName->text();
        //editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "backupHeat") {
        backupHeatPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "changeOverValve") {
        changeOverValvePanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "clockFormat") {
        clockFormatPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "colors") {
        currentScheme = currentSchemeTemp;
        upIcon = getButtonIcon(0., 1., "#ccc");
        upIconPressed = getButtonIcon(0., 1., rangerThemes[currentScheme].getBorderColorPressed());

        dnIcon = getButtonIcon(180., 1., "#ccc");
        dnIconPressed = getButtonIcon(180., 1., rangerThemes[currentScheme].getBorderColorPressed());

        lockPanel = createLockPanel();
        mainPanel = createMainPanel();
        editSchedulePanel = createEditSchedulePanel();
        settingsPanel = createSettingsPanel();
        heatPanel = createHeatPanel();
        coolPanel = createCoolPanel();

        colorsPanel->setVisible(false);
        setStyleSheet(rangerStyleSheet);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "coolTo") {
//        coolToTime = coolToTimeTemp;
        coolToDegrees = coolToDegreesTemp;
        coolPanel->setVisible(false);
        if (selectedMode == "cool") {
            statControl = coolToDegrees;
            tempLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        }
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "coolingStages") {
        coolingStagesPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "connectionSuccesful") {
        connectionSuccesfulPanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "deleteThermo") {
        deleteThermoPanel->setVisible(false);
        //editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "dst") {
        dstPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "editRoom") {                 // FIX ME
        editRoomPanel->setVisible(false);
        if (addNewLocation) {
            QMap<QString, Thermostat>::iterator i2;
            QMap<QString, Thermostat> thermos1 = thermostats;
            for (i2=thermos1.begin(); i2 != thermos1.end(); ++i2) {
                Thermostat t = i2.value();
                if (t.getRoom().length() == 0) {
                    thermostats[i2.key()].setRoom(thermoNameText);
                    break;
                 }
            }

            addNewLocation = false;
        } else {
            thermostats[selectedRoom].setRoom(thermoNameText);
        }
       //profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;

        //props.insert(selectedRoom, thermoName->text());
        //writeProperties(propertyOutFile);
    } else if (p == "editSchedule") {
        editSchedulePanel->setVisible(false);
        if (selectedDay == "ALL DAYS" || selectedDay.startsWith("WEEK") ||
                selectedDay == "VACATION") {
            loadSchedules();
            settingsSchedulePanel->setVisible(true);
            currentPanel = settingsSchedulePanel;
        } else {
            selectedDaySchedule = currentSchedule;
            dailySchedules[selectedDay] = currentSchedule;
            loadDailySchedules();
            dailySchedulePanel->setVisible(true);
            currentPanel = dailySchedulePanel;
        }
        QStringList props;
        props << "all days" << "weekend" << "monday" << "vacation";

        for (int i=0;i<props.length();i++) {
            if (selectedDay.toLower() == props[i]) {
                scheduleButtons[i].setObjectName("");
            } else if (selectedSchedule == props[i]) {
                scheduleButtons[i].setObjectName("butoffDefault");
            } else {
                scheduleButtons[i].setObjectName("butoff");
            }
        }
        setStyleSheet(rangerStyleSheet);
    } else if (p == "fanControl") {
        fanControlPanel->setVisible(false);
        fanControl = selectedFanControlTemp;
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "fanMode") {
        if (selectedFanModeTemp != NULL)  selectedFanMode = selectedFanModeTemp;
        fanLabel->setText(selectedFanMode);
        fanModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "heatingStages") {
        heatingStagesPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "heatTo") {
        heatToDegrees = heatToDegreesTemp;
 //       heatToTime = heatToTimeTemp;
        heatPanel->setVisible(false);
        if (selectedMode == "heat") {
            statControl = heatToDegrees;
            tempLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        }
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "hvacType") {
        hvacTypePanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "keyboardLockout") {
        keyboardLockoutPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "language") {
        languagesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "lock") {
        lockPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = lockPanel;
        lockCode = lockNums[0]->text() + lockNums[1]->text() + lockNums[2]->text() + lockNums[3]->text();
    } else if (p == "selectMode") {
        if (selectedModeTemp != NULL) selectedMode = selectedModeTemp;
        modeLabel->setText(selectedMode);
        selectModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "settingsSchedule") {
        settingsSchedulePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "profile") {
        profilePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "timeDate") {
        timeDatePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "thermostatControls") {
        thermostatControlsPanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "schedulingOptions") {
        schedulingOptionsPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "schedulePeriods") {
        schedulePeriodsPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "tempuratureLimits") {
        tempuratureLimitsPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "systemsChangeover") {
        systemsChangeOverPanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "tempuratureScale") {
        tempuratureScalePanel->setVisible(false);
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "thermo") {
        editThermoNamePanel->setVisible(false);
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;

        //writeConfig(configFile);
    }
    restartTimer();
}

void Ranger::scheduleButtonReleased() {     // FIX ME
    QString p = ((QPushButton*)sender())->property("button").toString();
     //qDebug() << "scheduleButtonReleased" << p;

    for (int i=0;i<4;i++) {
        scheduleButtons[i].setObjectName("butoff");
    }
    ((QPushButton*)sender())->setObjectName("");

    if (p == "all days") {
        selectedDay = "ALL DAYS";
    } else if (p == "weekend") {
        selectedDay = "WEEKEND";
    } else if (p == "monday") {
        selectedDay = "MONDAY";
    } else if (p == "vacation") {
        selectedDay = "VACATION";
    } else if (p == "setSchedule") {
        selectedSchedule = selectedDay.toLower();
        saveConfig(configFile);
    } else if (p == "editSchedule") {
        settingsSchedulePanel->setVisible(false);
        if (selectedDay == "ALL DAYS") {
            loadSchedules();
            editScehduleHeaderLabel->setText(selectedDay);
            weekButton->setVisible(false);
            editSchedulePanel->setVisible(true);
            currentPanel = editSchedulePanel;
        } else if (selectedDay == "WEEKEND") {
            loadSchedules();
            editScehduleHeaderLabel->setText(selectedDay);
            weekButton->setVisible(true);
            editSchedulePanel->setVisible(true);
            currentPanel = editSchedulePanel;
        } else if (selectedDay == "MONDAY") {
            selectedDaySchedule = dailySchedules[selectedDay];
            loadDailySchedules();
            weekButton->setVisible(false);
            dailySchedulePanel->setVisible(true);
            currentPanel = dailySchedulePanel;
        } else {
            loadSchedules();
            editScehduleHeaderLabel->setText(selectedDay);
            weekButton->setVisible(false);
            editSchedulePanel->setVisible(true);
            currentPanel = editSchedulePanel;
        }
        restartTimer();
        return;
    }

    QStringList props;
    props << "all days" << "weekend" << "monday" << "vacation";
    for (int i=0;i<props.length();i++) {
        if (selectedDay.toLower() == props[i]) {
            scheduleButtons[i].setObjectName("");
        } else if (selectedSchedule == props[i]) {
            scheduleButtons[i].setObjectName("butoffDefault");
        } else {
            scheduleButtons[i].setObjectName("butoff");
        }
    }
    setStyleSheet(rangerStyleSheet);
    currentPanel = settingsSchedulePanel;
    restartTimer();
}

void Ranger::settingsButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    //qDebug() << "settingsButtonReleased" << p;

    settingsPanel->setVisible(false);
    if (p == "colors") {
        colorsPanel->setVisible(true);
        currentPanel = colorsPanel;
    } else if (p == "languages") {
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "lock") {
        screenLocked = false;
        for (int i=0;i<4;i++) {
            lockNums[i]->setText(QString::number(lockCodes[i]));
        }
        lockPanel->setVisible(true);
        currentPanel = lockPanel;
    } else if (p == "schedule") {
        settingsSchedulePanel->setVisible(true);
        currentPanel = settingsSchedulePanel;
    } else if (p == "preferences") {
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "profile") {
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "setup") {
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "time_date") {
        currentDateTime =  QDateTime::currentDateTime();
        currentDate =  QDate::currentDate();
        currentTime =  QTime::currentTime();

        currentHour = currentTime.hour();
        currentMinute = currentTime.minute();
        currentAmPm = currentHour < 12 ? "AM" : "PM";

        currentDay = currentDate.day();
        currentMonth = currentDate.month();
        currentYear = currentDate.year();

        minuteText->setText(QString::number(currentMinute));
        hourText->setText(QString::number(currentHour));
        ampmText->setText(currentAmPm);

        monthText->setText(months.at(currentMonth));
        yearText->setText(QString::number(currentYear));
        dayText->setText(QString::number(currentDay));

        timeDatePanel->setVisible(true);
        currentPanel = timeDatePanel;
    }

    restartTimer();
}

void Ranger::editScheduleButtonReleased(QAbstractButton* button) {
    //qDebug() << "editScheduleButtonReleased" << button->text();
    for (int i=0;i<5;i++) {
        editScheduleButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    int x = 35*windowSize;
    int xi = 92*windowSize;
    int y1 = 125;
    int y2 = 178;

    if (button->text() == "PERIOD") {
        upButton->move(x,y1*windowSize);
        dnButton->move(x,y2*windowSize);
        currentIndex = 0;
    } else if (button->text() == "START") {
        upButton->move(x + xi,y1*windowSize);
        dnButton->move(x + xi,y2*windowSize);
        currentIndex = 1;
    } else if (button->text() == "STOP") {
        upButton->move(x +(xi*2),y1*windowSize);
        dnButton->move(x +(xi*2),y2*windowSize);
        currentIndex = 2;
    } else if (button->text() == "HEAT TO") {
        upButton->move(x +(xi*3),y1*windowSize);
        dnButton->move(x +(xi*3),y2*windowSize);
        currentIndex = 3;
    } else if (button->text() == "COOL TO") {
        upButton->move(x +(xi*4),y1*windowSize);
        dnButton->move(x +(xi*4),y2*windowSize);
        currentIndex = 4;
    }

    restartTimer();
}

void Ranger::locationButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    //qDebug() << "locationButtonReleased" << p;

    thermoNameText = thermostats[p].getRoom().simplified();
    selectedRoom = p;
    if (firstTime) {
        thermostatLocationPanel->setVisible(false);
    } else {
        profilePanel->setVisible(false);
    }
    setEditRoomHeader(thermoNameText);
    editRoomPanel->setVisible(true);
    currentPanel = editRoomPanel;

   restartTimer();
}

void Ranger::setEditRoomHeader(QString hdr) {
    thermostatLocationPanel->setVisible(false);
    headerFont.setWeight(QFont::DemiBold);
    QFontMetrics f2(headerFont);
    int w2 = f2.width("EDIT: " + hdr);
    editRoomHeaderLabel->setText("EDIT: " + hdr);

    int xw = (windowWidth/2)-((w2)/2);
    editRoomHeaderLabel->move(xw, 11*windowSize);
}

void Ranger::keyBackReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    //qDebug() << "keyBackReleased"<<p;

    if (keyboardEdit->text().length() == 0) {
        keyboardEdit->setFocus();
        return;
    }
    QString s;
    int k = 0;
    for (int i=0;i<keyboardEdit->text().length();i++) {
        if (i == keyboardEdit->cursorPosition()-1) {
            k = i;
        } else {
            s += keyboardEdit->text().at(i);
        }
    }
    keyboardEdit->setText(s);
    keyboardEdit->setCursorPosition(k);
    keyboardEdit->setFocus();

   restartTimer();
}

void Ranger::keyButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    //qDebug() << "keyButtonReleased" << button->text()<<p;

    QString key = button->text();
    if (key != "SHIFT") {
        key = button->text();
    } else if (key == "SHIFT") {
        shiftOn = !shiftOn;
        if (p == "name" || p == "profile") {
            firstName->setFocus();
        } else if (p == "thermo") {
            thermoName->setFocus();
        }

        if (shiftOn) {
            for (int i=0;i<26;i++) {
                alphaKeyButtons[i]->setText(alphaKeyButtons[i]->text().toUpper());
            }
        } else {
            for (int i=0;i<26;i++) {
                alphaKeyButtons[i]->setText(alphaKeyButtons[i]->text().toLower());
            }
        }
        restartTimer();

        return;
    }
    if (key == "SPACE") {
        key = " ";
    }

    QString s;
    int k = 0;

    for (int i=0;i<keyboardEdit->text().length();i++) {
        if (i == keyboardEdit->cursorPosition()) {
            s += key;
            k = i;
        }
        s += keyboardEdit->text().at(i);
    }

    if (k == 0) s += key;
    keyboardEdit->setText(s);
    if (k == 0) {
        keyboardEdit->end(false);
    } else {
        keyboardEdit->setCursorPosition(k+1);
    }

    keyboardEdit->setFocus();

    restartTimer();
}

void Ranger::setRangerSheet() {
    setStyleSheet(rangerStyleSheet);
}

void Ranger::colorButtonReleased(QAbstractButton* button) {
    QString scheme = button->property("button").toString();
    //qDebug() << "colorButtonReleased" << scheme;
    setColorScheme(scheme);

    restartTimer();
}

void Ranger::fanButtonReleased(QAbstractButton* button) {
    //qDebug() << "fanButtonReleased" << button->text();

    selectedFanModeTemp = button->text();
    for (int i=0;i<2;i++) {
        fanModeButtons[i].setObjectName("butoff");
    }

    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    // button->setStyleSheet("color: white;border: 2px solid cyan;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #14517a,stop: 1 #2d84c1);");

    restartTimer();
}

void Ranger::fanControlReleased(QAbstractButton* button) {
    //qDebug() << "fanControlReleased" << button->text();

    selectedFanControlTemp = button->text();
    for (int i=0;i<2;i++) {
        fanControlButtons[i].setObjectName("butoff");
    }

    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);


    restartTimer();
}

void Ranger::systemTypeReleased(QAbstractButton* button) {
    //qDebug() << "systemTypeReleased" << button->text();

    //selectedFanControlTemp = button->text();
    for (int i=0;i<3;i++) {
        systemTypeButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::backupHeatReleased(QAbstractButton* button) {
    //qDebug() << "backupHeatReleased" << button->text();

    for (int i=0;i<2;i++) {
        backupHeatButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::coolingStagesReleased(QAbstractButton* button) {
    //qDebug() << "coolingStagesReleased" << button->text();

    for (int i=0;i<2;i++) {
        coolingStagesButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::heatingStagesReleased(QAbstractButton* button) {
    //qDebug() << "heatingStagesReleased" << button->text();

    for (int i=0;i<2;i++) {
        heatingStagesButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::languagesReleased(QAbstractButton* button) {
    //qDebug() << "languagesReleased" << button->text();

    for (int i=0;i<2;i++) {
        languagesButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::schedulingOptionsReleased(QAbstractButton* button) {
    //qDebug() << "schedulingOptionsReleased" << button->text();

    for (int i=0;i<2;i++) {
        schedulingOptionsButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::clockFormatReleased(QAbstractButton* button) {
    //qDebug() << "clockFormatReleased" << button->text();

    for (int i=0;i<2;i++) {
        clockFormatButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::tempuratureScaleReleased(QAbstractButton* button) {
    //qDebug() << "tempuratureScaleReleased" << button->text();

    for (int i=0;i<2;i++) {
        tempuratureScaleButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::dstReleased(QAbstractButton* button) {
    //qDebug() << "dstReleased" << button->text();

    for (int i=0;i<2;i++) {
        dstButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::keyboardLockoutReleased(QAbstractButton* button) {
    //qDebug() << "keybaordLockoutReleased" << button->text();

    for (int i=0;i<3;i++) {
        keyboardLockoutButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::schedulePeriodsReleased(QAbstractButton* button) {
    //qDebug() << "schedulePeriodsReleased" << button->text();

    for (int i=0;i<2;i++) {
        schedulePeriodsButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::systemsChangeOverReleased(QAbstractButton* button) {
    //qDebug() << "systemsChangeOverReleased" << button->text();

    for (int i=0;i<2;i++) {
        systemsChangeOverButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setStyleSheet(rangerStyleSheet);

    restartTimer();
}

void Ranger::dayButtonPressed(QAbstractButton* button) {
    button->setStyleSheet("font-weight:bold;color:white;border:0 none;background-image: none;background-color: transparent;");
}

void Ranger::dayButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    button->setStyleSheet("font-weight:normal;color:#ddd;border:0 none;background-image: none;background-color: transparent;");
    //qDebug() << "dayButtonReleased" << p;

    selectedDay = p;
    selectedDayTemp = p;
    dailySchedulePanel->setVisible(false);
    selectedDaySchedule = dailySchedules[selectedDay];
    loadDailySchedules();
    //dailySchedulePanel = createDailySchedulePanel();
    dailySchedulePanel->setVisible(true);
    currentPanel = dailySchedulePanel;

    restartTimer();
}

void Ranger::modeButtonReleased(QAbstractButton* button) {
    selectedModeTemp = button->property("button").toString();
    //qDebug() << "modeButtonReleased" << selectedModeTemp;

    for (int i=0;i<5;i++) {
        selectModeButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");

    if (selectedModeTemp != NULL) selectedMode = selectedModeTemp;

    heatButton->setObjectName("");
    coolButton->setObjectName("");
    if (selectedMode == "heat") {
        statControl = heatToDegrees;
        modeLabel->setText(tr("HEAT"));
        heatButton->setObjectName("heat");
    } else if (selectedMode == "cool") {
        statControl = coolToDegrees;
        modeLabel->setText(tr("COOL"));
        coolButton->setObjectName("cool");
    } else if (selectedMode == "off") {
        modeLabel->setText(tr("OFF"));
    } else if (selectedMode == "auto") {
        modeLabel->setText(tr("AUTO"));
    } else if (selectedMode == "e_heat") {
        modeLabel->setText(tr("E.HEAT"));
    }

    setStyleSheet(rangerStyleSheet);

    selectModePanel->setVisible(false);
    mainPanel->setVisible(true);
    currentPanel = mainPanel;

    restartTimer();
}

void Ranger::mainButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    //qDebug() << "mainButtonReleased"<<p;

    mainPanel->setVisible(false);
    if (p == "mode") {
        selectModePanel->setVisible(true);
        currentPanel = selectModePanel;
    } else if (p == "fan") {
        fanModePanel->setVisible(true);
        currentPanel = fanModePanel;
    } else if (p == "hold" && selectedMode != "off") {
        if (!holdMode) {
            holdMode = true;
            holdLabel->setText(tr("ON HOLD"));
        } else {
            holdLabel->setText(tr("ON SCHED"));
            holdMode = false;
        }
        if (!holdMode || selectedMode == "off") {
            holdButton->setObjectName("");
        } else {
            holdButton->setObjectName("red");
        }
        if (holdMode) {
            fanButton->setDisabled(true);
            modeButton->setDisabled(true);
            heatButton->setDisabled(true);
            coolButton->setDisabled(true);
        } else {
            fanButton->setDisabled(false);
            modeButton->setDisabled(false);
            heatButton->setDisabled(false);
            coolButton->setDisabled(false);
        }
        setStyleSheet(rangerStyleSheet);
        mainPanel->setVisible(true);
        saveConfig(configFile);
    } else if (p == "heat") {
        heatToDegreesLabel->setText(QString::number(heatToDegrees));
        heatToDegreesTemp = heatToDegrees;
        heatToHourTemp = heatToHour;
        heatToMinuteTemp = heatToMinute;
        heatToAmPmTemp = heatToAmPm;
        QString tm = QString("%1:%2").arg(QString::number(heatToHourTemp),
                              QString::number(heatToMinuteTemp).rightJustified(2, '0'));
        heatToTimeLabel->setText(tm);
        heatToAmPmLabel->setText(heatToAmPmTemp);

        heatPanel->setVisible(true);
        currentPanel = heatPanel;
    } else if (p == "cool") {
        coolToDegreesLabel->setText(QString::number(coolToDegrees));
        coolToDegreesTemp = coolToDegrees;
        coolToHourTemp = coolToHour;
        coolToMinuteTemp = coolToMinute;
        coolToAmPmTemp = coolToAmPm;
        QString tm = QString("%1:%2").arg(QString::number(coolToHourTemp),
                              QString::number(coolToMinuteTemp).rightJustified(2, '0'));
        coolToTimeLabel->setText(tm);
        coolToAmPmLabel->setText(coolToAmPmTemp);

        coolPanel->setVisible(true);
        currentPanel = coolPanel;
    }

    restartTimer();
}

void Ranger::setColorScheme(QString color) {
    currentSchemeTemp = color;
    //qDebug() << "Theme:"<<color;

    QFile file(":/qss/"+color+".qss");
    bool bOpened = file.open(QFile::ReadOnly);

    rangerStyleSheet = QLatin1String(file.readAll());

    setStyleSheet(rangerStyleSheet);
 }

QString tstat = "tstat/configs/1111";
QString config = "thermostats/1111/configs";
void Ranger::getConfig() {
    QUrl url("http://localhost/rest/api/" + tstat);
    QNetworkAccessManager man;
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    QObject::connect(&man, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    timer.start(8000);

    QNetworkReply *reply = man.get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    bool success = false;
    if (timer.isActive()) {
        success = true;
    } else {
        //qDebug() << "ConfigUtils::getInfo timeout:";
    }
    timer.stop();

    //qDebug() << reply->readAll().simplified();
    reply->deleteLater();
}

void Ranger::putConfig() {
    QUrl configUrl("http://localhost/rest/api/"+tstat);
    QByteArray postData = configJson.toJson();

    networkManager = new QNetworkAccessManager(this);
    QNetworkRequest rqst(configUrl);
    rqst.setRawHeader("Content-Type", "application/json");
    QByteArray strSize = QByteArray().append(QString::number(postData.size()));
    rqst.setRawHeader("Content-Length", strSize);

     // connect 'SSL errors' signal with consumer
//    connect(pNetworkReply, SIGNAL(sslErrors(const QList<QSslError> &)),
//            this, SLOT(onSSLError(const QList<QSslError> &)));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                               this, SLOT(loadConfig(QNetworkReply*)));
    networkManager->put(rqst, postData);
    //reply->readAll();
}

void Ranger::loadConfig(QNetworkReply *reply) {
    //qDebug() << "loadConfig:" << " host:" << reply->url().host();
    if (reply->error() == 0) {
        //configReply->deleteLater();
        //qDebug() << "REPLY"<<reply->readAll();
    } else {
        //qDebug() << "putConfig reply error:" << reply->errorString();
    }
}

void Ranger::drawGrid(QWidget *widget) {
    QPixmap pixmapy(999,1);
    pixmapy.fill(QColor("#aaa"));
    for (int y=0;y<578;y+=50) {
        QLabel *label = new QLabel(widget);
        label->setPixmap(pixmapy);
        label->move(1,y);
    }

    QPixmap pixmapz(999,2);
    pixmapz.fill(QColor("#666"));
    for (int y=0;y<578;y+=100) {
        QLabel *label = new QLabel(widget);
        label->setPixmap(pixmapz);
        label->move(1,y);
    }

    QPixmap pixmapx(1,578);
    pixmapx.fill(QColor("#aaa"));
    for (int x=0;x<1000;x+=50) {
        QLabel *label = new QLabel(widget);
        label->setPixmap(pixmapx);
        label->move(x,1);
    }
    QPixmap pixmapq(2,578);
    pixmapq.fill(QColor("#666"));
    for (int x=0;x<1000;x+=100) {
        QLabel *label = new QLabel(widget);
        label->setPixmap(pixmapq);
        label->move(x,1);
    }

}

void Ranger::loadTestData() {

    Thermostat t;
    t.setRoom("LIVING ROOM");
    thermostats.insert("thermo1", t);

    t.setRoom("MASTER BEDROOM");
    thermostats.insert("thermo2", t);

    t.setRoom("OFFICE");
    thermostats.insert("thermo3", t);

    t.setRoom("");
    thermostats.insert("thermo4", t);

    t.setRoom("");
    //t.setRoom("MAN CAVE");
    thermostats.insert("thermo5", t);

    t.setRoom("");
    thermostats.insert("thermo6", t);

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    appPath = a.applicationDirPath();
    //qDebug() << appPath;

    QFontDatabase fontDB;
    fontDB.addApplicationFont(":/fonts/DroidSans.ttf");
    fontDB.addApplicationFont(":/fonts/DroidSans-Bold.ttf");
//    foreach(QString s, fontDB.families()) {
//        //qDebug() << s;
//    }

    if (argc > 1) {
        QTranslator translator;
        translator.load("../ranger/ranger_es");
        a.installTranslator(&translator);
    }

    Ranger window;
    window.showWindow();

    return a.exec();
}
