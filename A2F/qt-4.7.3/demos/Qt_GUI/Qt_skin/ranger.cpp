#include "ranger.h"


QString appPath;
QTimer *screenTimer;

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

    screenTimer = new QTimer(this);
    connect(screenTimer, SIGNAL(timeout()), this, SLOT(screenSaver()));
    screenTimer->start(60000);

    setColorScheme(currentScheme);

    buildThemes();
    buildButtonIcons();
}

Ranger::~Ranger()
{}

void Ranger::showWindow()
{
    windowXpos = 0;
    windowYpos = 0;
    windowWidth = 480*windowSize;
    windowHeight = 270*windowSize;

    setFixedSize(windowWidth, windowHeight);

    loadTestData();
    //writeProperties(configFile);
    writeStats(statsFile);

    customerNameText = "Frank";
    thermostatMACText = "00:50:56:8A:E3:C5";
    zipCodeText = "123456";
    thermostatIpText = "192.168.1.30";
    thermostatStatusText = "Working";
    thermostatCRCText ="3453";
    wifiNetworkText = "My Wifi Network";

    mainPanel = createMainPanel();
    mainPanel->setVisible(false);
    if (firstTime) {
        languagesPanel = createLanguagesPanel();
        languagesPanel->setVisible(true);
//        wifiSetupPanel = createWifiSetupPanel();
//        wifiSetupPanel->setVisible(true);
//        connectionSuccesfulPanel = createConnectionSuccesfulPanel();
//        connectionSuccesfulPanel->setVisible(true);
//        currentPanel = languagesPanel;
    } else {
        currentPanel = mainPanel;
        mainPanel->setVisible(true);
   }
    //idlePanel = createIdlePanel();
    //settingsPanel = createSettingsPanel();

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
        idlePanel = createIdlePanel();
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
int currentIndex;

void Ranger::changeSize() {
    qDebug() << "changeSize";

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
    writeConfig(statsFile);
}

void Ranger::statsFileChanged(QString file) {
    qDebug() << "Changed: " << file ;
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
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(0, "error", file.errorString());
        return;
    }

    QString js = file.readAll();
    file.close();

    configJson = QJsonDocument::fromJson(js.toUtf8());
    configObject = configJson.object();

    serialNumber = configObject["serial"].toString();
    dateTime =  configObject["date"].toString();
    currentScheme = configObject["themeScheme"].toString();
    statControl = (int)configObject["tempSetPoint"].toDouble();
    holdMode = configObject["tempHold"].toBool();
    selectedFanMode = configObject["fanMode"].toString();
    selectedMode = configObject["hvacMode"].toString();
    language = configObject["language"].toString();
    tempuratureScale = configObject["metric"].toBool();
    lockCode = configObject["lock_code"].toString();
    firstTime = (int)configObject["first_time"].toDouble();
    firstNameText = configObject["ownersName"].toString();
    dst = configObject["dst"].toBool();
    keyboardLockout = configObject["keyboard_lock"].toString();
    selectedSchedule = configObject["selectedSchedule"].toString();

    heatToDegrees = (int)configObject["heatToDegrees"].toDouble();
    coolToDegrees = (int)configObject["coolToDegrees"].toDouble();
    schedulingOption = configObject["scheduling_option"].toString();
    clockFormat = (int)configObject["clockFormat"].toDouble();
    systemsChangeOver = configObject["change_over"].toString();
    schedulePeriods = (int)configObject["schedule_periods"].toDouble();
    systemType = configObject["system_type"].toString();
    fanControl = configObject["fan_control"].toString();
    backupHeat = (configObject["backup_heat"].toString() == "yes") ? "backupHeatYes" : "backupHeatNo" ;
    coolingStages = (int)configObject["cooling_stage"].toDouble();
    heatingStages = (int)configObject["heating_stage"].toDouble();
    thermostatControls = (int)configObject["thermo_controls"].toDouble();
    coolToTime = (int)configObject["coolToTime"].toDouble();
    heatToTime = (int)configObject["heatToTime"].toDouble();
    heatToTimeText = QDateTime::fromTime_t(heatToTime).toString("h:mm A");
    coolToTimeText = QDateTime::fromTime_t(coolToTime).toString("h:mm A");

    QJsonObject hvac = configObject["hvacConfig"].toObject();
    hvacConfig.setBackupHeatingType(hvac["backupHeatingType"].toString());
    hvacConfig.setCoolingStages((int)hvac["coolingStages"].toDouble());
    hvacConfig.setHeatingStages((int)hvac["heatingStages"].toDouble());
    hvacConfig.setHvacType(hvac["hvacType"].toString());

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
        //qDebug() <<"S" << s.getLabel();
        QList<Day> days = s.getDays();
        foreach (Day day,days) {
            //qDebug() <<"  D" << day.getLabel();
            QList<Period> pl = day.getPeriods();
            schedulesMap.clear();
            foreach (Period p,pl) {
                //qDebug() <<"    P" << p.getLabel().toUpper()<<p.getCoolTemp()<<p.getStartTime();
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

    selectedDay = selectedSchedule;
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

     outsideTemp = statsObject["temp_out"].toString().toInt();
     outsideHumidity = statsObject["hum_out"].toString().toInt();
     insideTemp = statsObject["temp_in"].toString().toInt();
     insideHumidity = statsObject["hum_in"].toString().toInt();
}

void Ranger::writeConfig(QString path) {
    QFile jsonFile(path);
    if (jsonFile.open(QFile::WriteOnly | QFile::Truncate)) {
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
                //qDebug() <<"  D"<<day.getLabel().toUpper();
                QMap<QString, DailySchedule> daySched = dailySchedules[day.getLabel().toUpper()];
                QList<Period> pl = day.getPeriods();
                QList<Period> plt;
                foreach (Period p,pl) {
                    DailySchedule ds = daySched[p.getLabel().toUpper()];
                    //qDebug() <<"   P"<<p.getLabel().toUpper()<<ds.getPeriod()<<ds.getStartTime();
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
        jsonFile.write(saveDoc.toJson());

        jsonFile.close();
    }
}

void Ranger::writeStats(QString path) {
    QFile jsonFile(path);
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
    dayLabel = addLabel(currentDate.toString("ddd M/d/yy"), 0,y, box);
    dayLabel->setAlignment(Qt::AlignRight);
    dayLabel->setFixedWidth(136*windowSize);

    QLabel *bar = addLabel("|", 154*windowSize, y, box);
    bar->setFixedWidth(10*windowSize);

    timeLabel = addLabel(currentTime.toString("hh:mm A"), 175*windowSize,y, box);
    timeLabel->setAlignment(Qt::AlignLeft);
    timeLabel->setFixedWidth(156*windowSize);

    QPushButton *wifi = addImageButton("wifi3_"+currentScheme+".png", 320*windowSize,8*windowSize, .8, box);
    wifi->setFixedWidth(50*windowSize);

    connect(wifi , SIGNAL(clicked()), this,SLOT(changeSize()));

    labelFont.setPointSize(12*windowSize);
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

    QPushButton *up = addUpButton(390*windowSize, 70*windowSize, "#ccc", box);
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
    statBox->setGeometry(0,201*windowSize,480*windowSize,20*windowSize);
    statBox->setObjectName("grayBar");

    int x = 5*windowSize;
    int xi = 96*windowSize;
    int w = 90*windowSize;
#ifdef Q_OS_WIN
    y = 203*windowSize;
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

QGroupBox *Ranger::createRegisterPanel(QString name, bool abc) {
    QGroupBox *box = backgroundBox();
    box->setVisible(false);

    QFontMetrics f1(headerFont);
    int w1 = f1.width("REGISTER:");
    QLabel *reg = addHeaderLabel(tr("REGISTER:"), box);

    headerFont.setWeight(QFont::DemiBold);
    QFontMetrics f2(headerFont);
    QString txt;
    if (name == "customerName") {
        txt = tr("FIRST NAME");
    } else {
        txt = tr("ZIP CODE");
    }
    int w2 = f2.width(txt);
    QLabel *nm = addHeaderLabel(txt, box);

    int xw = (windowWidth/2)-((w1+w2)/2);
    reg->move(xw, 11*windowSize);
    nm->move(xw+w1 + 5*windowSize, 11*windowSize);

    labelFont.setPointSize(14*windowSize);
    labelFont.setWeight(QFont::Normal);

    keyboardEdit = new QLineEdit(box);
    keyboardEdit->setText(keyboardText);
    keyboardEdit->move(10*windowSize, 56*windowSize);
    keyboardEdit->setFixedSize(460*windowSize, 32*windowSize);
    keyboardEdit->setAlignment(Qt::AlignHCenter);
    keyboardEdit->setFont(labelFont);
    keyboardEdit->setReadOnly(false);
    keyboardEdit->setStyleSheet("border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;");
    keyboardEdit->setCursorPosition(keyboardText.length());
    keyboardEdit->end(false);
    keyboardEdit->setFocus();
    keyboardEdit->setDisabled(true);

    labelFont.setPointSize(14*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    shiftOn = false;

    alphaBox = alphaKeyboard(box, name);
    numBox = numericKeyboard(box, name);

    alphaBox->setVisible(abc);
    numBox->setVisible(!abc);

    return box;
}

QGroupBox *Ranger::alphaKeyboard(QGroupBox *bx, QString name) {
    QGroupBox *box = new QGroupBox(bx);
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

    QPushButton *keyButtons[keys[0].length()+keys[1].length()+keys[2].length()+2];

    int x = 10*windowSize;
    int y = 0;
    int xi = 47*windowSize;
    int yi = 40*windowSize;
    int row = 0;
    int key = 0;
    for (int i=0;i<keys[row].length();i++) {
        QString s = shiftOn ? keys[row].at(i).toUpper() : keys[row].at(i);
        keyButtons[key] = addBaseButton(s, x, y, box);
        keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    x = 34*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        QString s = shiftOn ? keys[row].at(i).toUpper() : keys[row].at(i);
        keyButtons[key] = addBaseButton(s, x, y, box);
        keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    x = 82*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        QString s = shiftOn ? keys[row].at(i).toUpper() : keys[row].at(i);
        keyButtons[key] = addBaseButton(s, x, y, box);
        keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    // SHIFT
    x = 10*windowSize;
    int ys = 32*windowSize;
    labelFont.setPointSize(11*windowSize);
    keyButtons[key] = new QPushButton("SHIFT", box);
    keyButtons[key]->move(x,y);
    keyButtons[key]->setFont(labelFont);
    keyButtons[key]->setProperty("button", name);
    if (shiftOn) {
        keyButtons[key]->setObjectName("keyshift");
    } else {
        keyButtons[key]->setObjectName("keyspecial");
    }
    keyButtons[key++]->setFixedSize(65*windowSize, ys);

    // left arrow key
    x = 410*windowSize;
    QPixmap pix(":images/backspace_" + rangerThemes["keyboard"].getTextColor() + ".png");
    QIcon qi = QIcon(pix);
    QSize qs(pix.rect().width()*(windowSize/2),pix.rect().height()*(windowSize/2));
    QPushButton *back  = addBaseButton("", x, y, box);
    back->setProperty("button", name);
    back->setGeometry(x,y, 60*windowSize, ys);
    back->setIcon(qi);
    back->setIconSize(qs);
    back->setObjectName("keyspecial");

    connect(back , SIGNAL(released()), this,SLOT(keyBackReleased()));

    // SPACE bar
    y += yi;
    keyButtons[key] = addBaseButton("SPACE", 200*windowSize, y, box);
    keyButtons[key]->setObjectName("keyspecial");
    keyButtons[key++]->setGeometry(200*windowSize, y, 190*windowSize, ys);

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<key;j++) {
        keyButtons[j]->setProperty("keyboard", name);
        cg->addButton(keyButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                this,SLOT(keyButtonReleased(QAbstractButton*)));

    QPushButton *cancel = addBaseButton(tr("CANCEL"), 28*windowSize, y, box);
    cancel->setProperty("button", name);
    cancel->setFixedSize(85*windowSize, ys);
    cancel->setObjectName("keyspecial");

    numberButton = addBaseButton(".?123", 122*windowSize, y, box);
    numberButton->setProperty("button", "numButton");
    numberButton->setFixedSize(70*windowSize, ys);
    numberButton->setObjectName("keyspecial");
    connect(numberButton , SIGNAL(released()), this,SLOT(buttonReleased()));

    QPushButton *done = addBaseButton(tr("DONE"), 400*windowSize, y, box);
    done->setFixedSize(70*windowSize, ys);
    done->setProperty("button", name);
    done->setObjectName("keyspecial");

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(done , SIGNAL(released()), this,SLOT(doneReleased()));

    return box;
}

QGroupBox *Ranger::numericKeyboard(QGroupBox *bx, QString name) {
    QGroupBox *box = new QGroupBox(bx);
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

    QPushButton *keyButtons[keys[0].length()+keys[1].length()+keys[2].length()+3];

    int x = 10*windowSize;
    int y = 0;
    int xi = 47*windowSize;
    int yi = 40*windowSize;
    int row = 0;
    int key = 0;

    for (int i=0;i<keys[row].length();i++) {
        keyButtons[key] = addBaseButton(keys[row].at(i), x, y, box);
        keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    x = 34*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        keyButtons[key] = addBaseButton(keys[row].at(i), x, y, box);
        if (name == "zipCode" && i != 0) {
           keyButtons[key]->setDisabled(true);
        }
        keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    // Handle ampersand (&)
    keyButtons[key] = addBaseButton("&&", x, y, box);
    if (name == "zipCode") {
       keyButtons[key]->setDisabled(true);
    }
    keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);

    x = 82*windowSize;
    y += yi;
    row++;
    for (int i=0;i<keys[row].length();i++) {
        keyButtons[key] = addBaseButton(keys[row].at(i), x, y, box);
        if (name == "zipCode") {
           keyButtons[key]->setDisabled(true);
        }
        keyButtons[key++]->setFixedSize(35*windowSize,32*windowSize);
        x += xi;
    }

    // SHIFT key
    x = 10*windowSize;
    int ys = 32*windowSize;
    labelFont.setPointSize(11*windowSize);
    keyButtons[key] = new QPushButton("SHIFT", box);
    keyButtons[key]->move(x,y);
    keyButtons[key]->setFont(labelFont);
    keyButtons[key]->setDisabled(true);
    keyButtons[key]->setObjectName("keyspecial");
    keyButtons[key++]->setFixedSize(65*windowSize, ys);


    // .COM key
    x += 305*windowSize;
    keyButtons[key] = addBaseButton(".COM", x, y, box);
    keyButtons[key]->setObjectName("keyspecial");
    if (name == "zipCode") {
       keyButtons[key]->setDisabled(true);
    }
    keyButtons[key++]->setFixedSize(82*windowSize, ys);

    // left arrow key
    x = 410*windowSize;
    QPixmap pix(":images/backspace_" + rangerThemes["keyboard"].getTextColor() + ".png");
    QIcon qi = QIcon(pix);
    QSize qs(pix.rect().width()*(windowSize/2),pix.rect().height()*(windowSize/2));
    QPushButton *back  = addBaseButton("", x, y, box);
    back->setProperty("button", name+"name");
    back->setGeometry(x,y, 60*windowSize, ys);
    back->setIcon(qi);
    back->setIconSize(qs);
    back->setObjectName("keyspecial");

    connect(back , SIGNAL(released()), this,SLOT(keyBackReleased()));

    // SPACE bar
    y += yi;
    keyButtons[key] = addBaseButton(tr("SPACE"), 200*windowSize, y, box);
    keyButtons[key]->setProperty("button","space");
    keyButtons[key]->setObjectName("keyspecial");
    if (name == "zipCode") {
       keyButtons[key]->setDisabled(true);
    }
    keyButtons[key++]->setGeometry(200*windowSize, y, 190*windowSize, ys);

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<key;j++) {
        keyButtons[j]->setProperty("button", name);
        cg->addButton(keyButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                this,SLOT(keyButtonReleased(QAbstractButton*)));

    QPushButton *cancel = addBaseButton(tr("CANCEL"), 28*windowSize, y, box);
    cancel->setProperty("button", name);
    cancel->setFixedSize(85*windowSize, ys);
    cancel->setObjectName("keyspecial");

    abcButton = addBaseButton("ABC", 122*windowSize, y, box);
    abcButton->setProperty("button", "abcButton");
    abcButton->setFixedSize(70*windowSize, ys);
    if (name == "zipCode") {
       abcButton->setDisabled(true);
    }
    abcButton->setObjectName("keyspecial");

    QPushButton *done = addBaseButton(tr("DONE"), 400*windowSize, y, box);
    done->setProperty("button", name);
    done->setFixedSize(70*windowSize, ys);
    done->setObjectName("keyspecial");

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
    connect(abcButton , SIGNAL(released()), this,SLOT(buttonReleased()));
    connect(done , SIGNAL(released()), this,SLOT(doneReleased()));

    return box;
}

QGroupBox *Ranger::createEditSchedulePanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *edit = addHeaderLabel(tr("EDIT SCHEDULE:"), 0, 11*windowSize, box);
    edit->setFixedWidth(280*windowSize);
    edit->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ql = addHeaderLabel(selectedDay, 286*windowSize, 11*windowSize, box);
    ql->setFixedWidth(180*windowSize);
    ql->setAlignment(Qt::AlignLeft);

    currentSchedule = dailySchedules[selectedDay];
    qDebug() << "V" << selectedDay;
    DailySchedule sched = currentSchedule["WAKE"];
    currentDailySchedule =  currentSchedule["WAKE"];

    int x = 15*windowSize;
    int y = 60*windowSize;
    int w = 90*windowSize;
    int xi = 92*windowSize;

    labelFont.setPointSize(11*windowSize);
    labelFont.setWeight(QFont::DemiBold);

    periodLabel = addLabels(sched.getPeriod(), (x), y, "color:#999;", box);
    periodLabel->setFixedWidth(w);
    periodLabel->setAlignment(Qt::AlignHCenter);

    startLabel = addLabels(sched.getStartTime(), (x+=xi), y, "color:#999;", box);
    startLabel->setFixedWidth(w);
    startLabel->setAlignment(Qt::AlignHCenter);

    stopLabel = addLabels(sched.getStopTime(), (x+=xi), y, "color:#999;", box);
    stopLabel->setFixedWidth(w);
    stopLabel->setAlignment(Qt::AlignHCenter);

    heatToLabel = addLabels(QString::number(sched.getHeatTo())+degreeCode.at(degreeCode.length()-1),
                            (x+=xi), y, "color:#cd4744;", box);
    heatToLabel->setFixedWidth(w);
    heatToLabel->setAlignment(Qt::AlignHCenter);

    coolToLabel = addLabels(QString::number(sched.getCoolTo())+degreeCode.at(degreeCode.length()-1),
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
        if (i == sched.getIndex()) {
            editScheduleButtons[i].setObjectName("");
        } else {
            editScheduleButtons[i].setObjectName("butoff");
        }
        editScheduleButtons[i].show();
        x += xi;
    }

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<days.length();j++) {
        cg->addButton(&editScheduleButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                this,SLOT(editScheduleButtonReleased(QAbstractButton*)));

    x = 35*windowSize + (sched.getIndex()*92*windowSize);
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
    QPushButton *week;

    if (selectedDay == "WEEKEND") {
        week = addButton(tr("WEEKDAY"), 200*windowSize, 230*windowSize, box);
        week->setProperty("button","weekday");
        connect(week , SIGNAL(released()), this,SLOT(buttonReleased()));
    } else if (selectedDay == "WEEKDAY") {
        week = addButton(tr("WEEKEND"), 200*windowSize, 230*windowSize, box);
        week->setProperty("button","weekend");
        connect(week , SIGNAL(released()), this,SLOT(buttonReleased()));
    }

    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
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

QGroupBox *Ranger::createDailySchedulePanel() {
    QGroupBox *box = backgroundBox();

    QStringList days;
    days << "M" << "T" << "W" << "TH" << "F" << "S" << "SU";

    QStringList weekdays;
    weekdays << "MONDAY" << "TUESDAY" << "WEDNESDAY" << "THURSDAY"
             << "FRIDAY" << "SATURDAY" << "SUNDAY";

    dailyButtons = new QPushButton[days.length()];

    int x = 12*windowSize;
    int y = 10*windowSize;
    labelFont.setPointSize(18*windowSize);
    labelFont.setWeight(QFont::Normal);
    for (int i=0;i<7;i++) {
        dailyButtons[i].setParent(box);
        dailyButtons[i].setText(days[i]);
        dailyButtons[i].setProperty("button",weekdays[i]);
        dailyButtons[i].setFont(labelFont);
        dailyButtons[i].move(x,y);
        dailyButtons[i].setFixedWidth(48*windowSize);
        if (weekdays[i] == selectedDay) {
            dailyButtons[i].setStyleSheet("font-weight:bold;color:white;border:0 none;background-image: none;background-color: transparent;");
        } else {
            dailyButtons[i].setStyleSheet("color:#ddd;border:0 none;background-image: none;background-color: transparent;");
        }
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
    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(11*windowSize);

    QString st = "color:#aaa;";
    QPixmap pixmap(480*windowSize,2*windowSize);
    pixmap.fill(QColor("#eaeaea"));

    for (int i=0;i<periodList.length();i++) {
        DailySchedule ds = selectedDaySchedule[periodList[i]];
        addLabels(ds.getPeriod(), x , y, st, box);
        addLabels(ds.getStartTime() + " - " + ds.getStopTime(), x + 110*windowSize, y, st, box);
        addLabels(QString::number(ds.getHeatTo()) + degreeCode.at(degreeCode.length()-1),
                  x + 312*windowSize, y, "color:#cd4744;", box);
        addLabels(QString::number(ds.getCoolTo()) + degreeCode.at(degreeCode.length()-1),
                  x + 390*windowSize, y, "color:#2d89c6;", box);
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

    QPushButton *upTemp = addUpButton(158*windowSize,100*windowSize, "#ccc", box);
    upTemp->setProperty("button","heatToDegrees");

    QPushButton *dnTemp = addDownButton(158*windowSize,150*windowSize, "#ccc", box);
    dnTemp->setProperty("button","heatToDegrees");

    connect(upTemp , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upTemp , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnTemp , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnTemp , SIGNAL(released()), this,SLOT(downReleased()));

    QStringList tm = heatToTimeText.split(" ");

#ifdef Q_OS_WIN
    labelFont.setPointSize(50*windowSize);
    heatToTimeLabel = addLabel(tm[0], 225*windowSize, 110*windowSize, box);
#else
    labelFont.setPointSize(60*windowSize);
    heatToTimeLabel = addLabel(tm[0], 228*windowSize, 100*windowSize, box);
#endif

    heatToTimeLabel->setStyleSheet("color:#757575;");
    heatToTimeLabel->setAlignment(Qt::AlignRight);
    heatToTimeLabel->setFixedWidth(170*windowSize);

    QLabel *ampm = new QLabel(tm[1], box);
    ampm->setStyleSheet("color:#757575;font-size:"+
                        QString::number(15*windowSize)+"px;font-weight:bold;");

#ifdef Q_OS_WIN
   ampm->setStyleSheet("color:#757575;font-size:"+
                        QString::number(15*windowSize)+"px;font-weight:bold;");
   ampm->move(360*windowSize, 175*windowSize);
#else
   ampm->setStyleSheet("color:#757575;font-size:"+
                        QString::number(15*windowSize)+"px;font-weight:bold;");
   ampm->move(360*windowSize, 184*windowSize);
#endif

    QPushButton *upTime = addUpButton(400*windowSize,100*windowSize, "#ccc", box);
    upTime->setProperty("button","heatToTime");

    QPushButton *dnTime = addDownButton(400*windowSize,150*windowSize, "#ccc", box);
    dnTime->setProperty("button","heatToTime");

    connect(upTime , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upTime , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnTime , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnTime , SIGNAL(released()), this,SLOT(downReleased()));

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
    coolToDegreesLabel = addLabel(QString::number(heatToDegrees),
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
    QPushButton *upDeg = addUpButton(158*windowSize,100*windowSize, "#ccc", box);
    upDeg->setProperty("button","coolToDegrees");

    QPushButton *dnDeg = addDownButton(158*windowSize,150*windowSize, "#ccc", box);
    dnDeg->setProperty("button","coolToDegrees");

    connect(upDeg , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upDeg , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnDeg , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnDeg , SIGNAL(released()), this,SLOT(downReleased()));

    QStringList tm = coolToTimeText.split(" ");

#ifdef Q_OS_WIN
    labelFont.setPointSize(50*windowSize);
    coolToTimeLabel = addLabel(tm[0], 225*windowSize, 110*windowSize, box);
#else
    labelFont.setPointSize(60*windowSize);
    coolToTimeLabel = addLabel(tm[0], 228*windowSize, 100*windowSize, box);
#endif

    coolToTimeLabel->setStyleSheet("color:#757575;");
    coolToTimeLabel->setAlignment(Qt::AlignRight);
    coolToTimeLabel->setFixedWidth(170*windowSize);

    QLabel *ampm = new QLabel(tm[1], box);

#ifdef Q_OS_WIN
    ampm->setStyleSheet("color:#757575;font-size:"+
                         QString::number(15*windowSize)+"px;font-weight:bold;");
    ampm->move(360*windowSize, 175*windowSize);
#else
   ampm->setStyleSheet("color:#757575;font-size:"+
                        QString::number(15*windowSize)+"px;font-weight:bold;");
   ampm->move(360*windowSize, 184*windowSize);
#endif

    QPushButton *upTime = addUpButton(400*windowSize,100*windowSize, "#ccc", box);
    upTime->setProperty("button","coolToTime");

    QPushButton *dnTime = addDownButton(400*windowSize,150*windowSize, "#ccc", box);
    dnTime->setProperty("button","coolToTime");

    connect(upTime , SIGNAL(pressed()), this,SLOT(upPressed()));
    connect(upTime , SIGNAL(released()), this,SLOT(upReleased()));
    connect(dnTime , SIGNAL(pressed()), this,SLOT(downPressed()));
    connect(dnTime , SIGNAL(released()), this,SLOT(downReleased()));

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
    props << "all_days" << "weekday_weekend" << "each_day" << "vacation";
    scheduleButtons = new QPushButton[buts.length()];

    labelFont.setPointSize(9*windowSize);
    int x = 25*windowSize;
    for (int i=0;i<buts.length();i++) {
        scheduleButtons[i].setParent(box);
        scheduleButtons[i].setText(buts[i]);
        scheduleButtons[i].setProperty("button", props[i]);
        scheduleButtons[i].setFont(labelFont);
        scheduleButtons[i].setGeometry(x, 125*windowSize, 100*windowSize, 50*windowSize);
        if (selectedSchedule == props[i]) {
            scheduleButtons[i].setObjectName("");
        } else {
            scheduleButtons[i].setObjectName("butoff");
        }
        scheduleButtons[i].show();
        x += 115*windowSize;
    }


    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<buts.length();j++) {
        cg->addButton(&scheduleButtons[j]);
    }

    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(scheduleButtonReleased(QAbstractButton*)));

    box->setVisible(false);
    return box;
}

QGroupBox *Ranger::createEditThermoNamePanel(QString name) {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    QLabel *ed = addHeaderLabel(tr("EDIT:"), 0, 11*windowSize, box);
    ed->setFixedWidth(150*windowSize);
    ed->setAlignment(Qt::AlignRight);

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ql = addHeaderLabel(tr("THERMOSTAT NAME"), 160*windowSize, 11*windowSize, box);
    ql->setFixedWidth(270*windowSize);
    ql->setAlignment(Qt::AlignLeft);

    labelFont.setPointSize(13*windowSize);
    labelFont.setWeight(QFont::Normal);

    keyboardEdit = new QLineEdit(box);
    keyboardEdit->setText(keyboardText);
    keyboardEdit->move(10*windowSize, 56*windowSize);
    keyboardEdit->setFixedSize(460*windowSize, 32*windowSize);
    keyboardEdit->setAlignment(Qt::AlignHCenter);
    keyboardEdit->setFont(labelFont);
    keyboardEdit->setReadOnly(false);
    keyboardEdit->setStyleSheet("border: 2px solid #aaa;border-radius:10px;background-color:#dbdbdb;");
    keyboardEdit->setCursorPosition(keyboardText.length());
    keyboardEdit->end(false);
    keyboardEdit->setFocus();

    shiftOn = false;

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(14*windowSize);
    alphaBox = alphaKeyboard(box, name);
    numBox = numericKeyboard(box, name);

    alphaBox->setVisible(true);
    numBox->setVisible(false);

    box->setVisible(false);

    return box;
}

QGroupBox *Ranger::createWifiPasswordPanel(QString name) {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *ed = addHeaderLabel(tr("ENTER WIFI PASSWORD"), 0, 11*windowSize, box);
    ed->setFixedWidth(windowWidth);
    ed->setAlignment(Qt::AlignHCenter);

    labelFont.setPointSize(13*windowSize);
    labelFont.setWeight(QFont::Normal);

    keyboardEdit = new QLineEdit(box);
    keyboardEdit->setText(keyboardText);
    keyboardEdit->move(10*windowSize, 56*windowSize);
    keyboardEdit->setFixedSize(460*windowSize, 32*windowSize);
    keyboardEdit->setAlignment(Qt::AlignHCenter);
    keyboardEdit->setFont(labelFont);
    keyboardEdit->setReadOnly(false);
    keyboardEdit->setStyleSheet("border: 2px solid #aaa;border-radius:10px;background-color:#dbdbdb;");
    keyboardEdit->setCursorPosition(keyboardText.length());
    keyboardEdit->end(false);
    keyboardEdit->setFocus();

    shiftOn = false;

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(14*windowSize);
    alphaBox = alphaKeyboard(box, name);
    numBox = numericKeyboard(box, name);

    alphaBox->setVisible(true);
    numBox->setVisible(false);

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
    addHeaderLabel(tr("THERMOSTAT LOCATION"), 85*windowSize, 11*windowSize, box);

    if (thermostats.size() != 0) {
        locationButtons = new QPushButton[thermostats.size()];

        QMap<QString, Thermostat>::iterator i;
        QMap<QString, Thermostat> thermos = thermostats;
        labelFont.setPointSize(9*windowSize);

        QFontMetrics fm(labelFont);

        int x = 20*windowSize;
        int y = 100*windowSize;
        int k = 0;
        for (i=thermos.begin(); i != thermos.end(); ++i) {
            Thermostat t = i.value();
            if (t.getRoom().length() == 0) continue;
            locationButtons[k].setParent(box);
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
            //locationButtons[k].setObjectName("");
            locationButtons[k].show();
            //if (weekdays[k++] == selectedDay) md = i;
            k++;
            if (k == 3) {
                x = 20*windowSize;
                y += 55*windowSize;
            } else {
               x += 155*windowSize;
            }
        }

        //locationButtons[md].setObjectName("");

        QButtonGroup *cg = new QButtonGroup;
        for (int j=0;j<thermostats.size();j++) {
            cg->addButton(&locationButtons[j]);
        }

        connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
                    this, SLOT(locationButtonReleased(QAbstractButton*)));
    }

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);
        QPushButton *addNew = addButton(tr("ADD NEW"), 190*windowSize, 230*windowSize, box);
        addNew->setProperty("button","addNewLocation");

        connect(addNew , SIGNAL(released()), this,SLOT(buttonReleased()));

        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "thermostatLocationBack");

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "thermostatLocationNext");
        next->setFixedWidth(75*windowSize);

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        labelFont.setPointSize(10*windowSize);
        QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
        cancel->setProperty("button","locationCancel");
        QPushButton *addNew = addButton(tr("ADD NEW"), 355*windowSize, 230*windowSize, box);
        addNew->setProperty("button","addNewLocation");

        connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
        connect(addNew , SIGNAL(released()), this,SLOT(buttonReleased()));
    }

    box->setVisible(false);

    return box;
}

// SETTINGS
QGroupBox *Ranger::createSettingsPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    addHeaderLabel(tr("SETTINGS"), 180*windowSize, 11*windowSize, box);

    QPushButton *ret = addImageButton("return.png", 20*windowSize, 10*windowSize, .5, box);
    ret->setProperty("button","settings");
    connect(ret , SIGNAL(pressed()), this,SLOT(returnPressed()));

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(11*windowSize);

    QStringList buts;
    buts << "time_date.png" << "lock.png" << "schedule.png" << "languages.png"
         << "colors.png" << "profile.png" << "preferences.png" << "settings.png";

    QStringList labels;
    labels << tr("Time/Date") << tr("Lock") << tr("Schedule") << tr("Languages")
           << tr("Colors") <<  tr("Profile") << tr("Preferences") << tr("Setup");

    QStringList props;
    props << "time_date" << "lock" << "schedule" << "languages"
           << "colors" << "profile" << "preferences" << "setup";

    QPushButton *buttons[buts.length()];
    int x = 30*windowSize;
    int y = 75*windowSize;
    for (int i=0;i<buts.length();i++) {
        buttons[i] = addImageButton(props[i]+"_"+currentScheme, x +(10*windowSize),y, .5, box);
        buttons[i]->setProperty("button",props[i]);

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
    buttons[3]->move(x+3*115*windowSize, 80*windowSize);
    buttons[5]->move(x+126*windowSize, 170*windowSize);
    buttons[6]->move(x+2*115*windowSize, 155*windowSize);
    buttons[7]->move(x+3*115*windowSize, 160*windowSize);

    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<buts.length();j++) {
        cg->addButton(buttons[j]);
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
        colorButtons[i].setGeometry(x, 118*windowSize, 74*windowSize, 47*windowSize);
        colorButtons[i].setObjectName(props[i]+"But");
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
    qDebug() << "createEditRoomPanel"<<thermoNameText;

    headerFont.setWeight(QFont::Normal);
    QFontMetrics f1(headerFont);

    int w1 = f1.width("EDIT: ");
    QLabel *ed = addHeaderLabel(tr("EDIT:"), 0, 11*windowSize, box);
    ed->setAlignment(Qt::AlignLeft);

    headerFont.setWeight(QFont::DemiBold);
    QFontMetrics f2(headerFont);
    int w2 = f2.width(thermoNameText);
    QLabel *ql = addHeaderLabel(thermoNameText,
                                180*windowSize, 11*windowSize, box);
    ql->setAlignment(Qt::AlignLeft);

    int xw = (windowWidth/2)-((w1+w2)/2);
    ed->move(xw, 11*windowSize);
    ql->move(xw+w1, 11*windowSize);

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

QGroupBox *Ranger::createLockPanel(bool locked) {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::Normal);
    if (locked) {
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
    }

    labelFont.setPointSize(40*windowSize);

    QPushButton *upButs[4];
    QPushButton *dnButs[4];

    int x = 28*windowSize;
    int y = 100*windowSize;

    QSize qs(36*windowSize,36*windowSize);
    for (int i=0;i<4;i++) {
        lockNums[i] = addText(QString::number(lockCodes[i]),x, y,
                  "border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;",
                  box);
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

    if (locked) {
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
    yearText = addText(QString::number(currentYear),x, y,
                        style,
                        box);
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
    int y = 60* windowSize;
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
            buts[i]->setFixedWidth(225*windowSize);
            connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));

            if (i == 3) {
                x = 242*windowSize;
                y = 60*windowSize;
            } else {
                y += 40*windowSize;
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

    createCancelDone("preferences", box);

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
    buts << tr("OFF") << tr("AUTO") << tr("HEAT") << tr("COOL") << tr("EMER.\nHEAT");

    QStringList props;
    props << "off" << "auto" << "heat" << "cool" << "emer_heat";
    selectModeButtons = new QPushButton[buts.length()];

    int x = 20*windowSize;
    int xi = (windowWidth/buts.length()) * .94;
    for (int i=0;i<buts.length();i++) {
        selectModeButtons[i].setParent(box);
        selectModeButtons[i].setText(buts[i]);
        selectModeButtons[i].setProperty("button",props[i]);
        selectModeButtons[i].setFont(labelFont);
        selectModeButtons[i].setGeometry(x, 125*windowSize, 80*windowSize, 50*windowSize);
        x += xi;
        if (selectedMode == "heat" && i == 2) {
            selectModeButtons[i].setObjectName("heat");
        } else if (selectedMode == "cool" && i == 3) {
            selectModeButtons[i].setObjectName("cool");
        } else if (selectedMode == "off" && i == 0) {
            selectModeButtons[i].setObjectName("");
        } else if (selectedMode == "auto" && i == 1) {
            selectModeButtons[i].setObjectName("");
        } else if (selectedMode == "emer_heat" && i == 4) {
            selectModeButtons[i].setObjectName("");
        } else {
            selectModeButtons[i].setObjectName("butoff");
        }
        selectModeButtons[i].show();
    }


    QButtonGroup *cg = new QButtonGroup;
    for (int j=0;j<buts.length();j++) {
        cg->addButton(&selectModeButtons[j]);
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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 120*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(250*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        y += 75*windowSize;
    }
    if (schedulingOption == "programmable") {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        y += 75*windowSize;
    }

    if (!tempuratureScale) {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        y += 75*windowSize;
    }
    if (clockFormat == 12) {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }
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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 158*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(160*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        y += 75*windowSize;
    }
    if (dst) {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }
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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        y += 75*windowSize;
    }
    if (systemsChangeOver == "manualChangeover") {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }
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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 125*windowSize;
    int y = 65*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(250*windowSize, 44*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        y += 58*windowSize;
    }
    qDebug() <<keyboardLockout;
    if (keyboardLockout == "keyboardUnlocked") {
        buts[0]->setObjectName("");
    } else if (keyboardLockout == "keyboardPartiallyUnlocked") {
        buts[1]->setObjectName("");
    } else {
        buts[2]->setObjectName("");
    }

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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(prefsReleased()));
        //x += 210*windowSize;
        y += 75*windowSize;
    }
    if (schedulePeriods == 2) {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

    createCancelSave("schedulePeriods", box);

    box->setVisible(false);

    return box;
}

// SETUP PANELS
QGroupBox *Ranger::createSetupPanel() {
    QGroupBox *box = backgroundBox();

    headerFont.setWeight(QFont::DemiBold);
    QLabel *cl = addHeaderLabel(tr("SYSTEM SETUP"), 140*windowSize, 11*windowSize, box);
    cl->setAlignment(Qt::AlignLeft);

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(16*windowSize);
    QFontMetrics fm(labelFont);
    //tr("Change Over Valve") << "changeOverValve"
    QStringList prefs;
    prefs << tr("Language") << tr("System Type") << tr("Thermostat Controls") << tr("Fan Control")
          << tr("WIFI Setup") << tr("Backup Heat") << tr("Cooling Stages") << tr("Heating Stages");

    QStringList props;
    props << "language" << "systemType" << "thermostatControls" << "fanControl"
           << "wifiSetup" << "backupHeat" << "coolingStages" << "heatingStages";

    QPushButton *buts[prefs.size()];

    int x = 10*windowSize;
    int y = 60* windowSize;
    labelFont.setPointSize(11.5*windowSize);
    for (int i=0;i<prefs.size();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setProperty("button",props[i]);
        buts[i]->setFixedSize(220*windowSize, 34*windowSize);
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));

        if (i == 3) {
            x = 242*windowSize;
            y = 60*windowSize;
        } else {
            y += 40*windowSize;
        }
    }

    createCancelDone("setup", box);

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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 75*windowSize;
    }
    qDebug()<<"L"<<language;
    if (language == "english") {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

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

    labelFont.setWeight(QFont::DemiBold);
    labelFont.setPointSize(18*windowSize);
    QFontMetrics fm(labelFont);

    QStringList prefs;
    prefs << tr("Forced Air") << tr("Heat Pump") << tr("Hot Water or Steam");

    QStringList props;
    props << "forcedAir" << "heatPump" << "waterOrSteam";

    QPushButton *buts[prefs.size()];
qDebug() << "ST"<<systemType;
    int x = 115*windowSize;
    int y = 75* windowSize;
    labelFont.setPointSize(11.5*windowSize);
    for (int i=0;i<prefs.size();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setProperty("button",props[i]);
        buts[i]->setObjectName("butoff");
        buts[i]->setFixedSize(260*windowSize, 36*windowSize);
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 50*windowSize;
    }

    if (systemType == "forcedAir") {
        buts[0]->setObjectName("");
    } else if (systemType == "heatPump") {
        buts[1]->setObjectName("");
    } else {
        buts[2]->setObjectName("");
    }

    if (firstTime) {
        labelFont.setPointSize(10*windowSize);
        QPushButton *back = addButton("BACK",20*windowSize, 230*windowSize, box);
        back->setProperty("button", "systemTypeBack");

        QPushButton *next = addButton("NEXT",390*windowSize, 230*windowSize, box);
        next->setProperty("button", "systemTypeNext");
        next->setFixedWidth(75*windowSize);

        connect(back , SIGNAL(released()), this,SLOT(backReleased()));
        connect(next , SIGNAL(released()), this,SLOT(nextReleased()));
    } else {
        createCancelSave("systemType", box);
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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 134*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 75*windowSize;
    }

    if (fanControl == "thermostat") {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 160*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(180*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 75*windowSize;
    }

    if (backupHeat == "backupHeatYes") {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }


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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 75*windowSize;
    }
    if (coolingStages == 1) {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

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

    QPushButton *buts[props.length()];

    labelFont.setPointSize(15*windowSize);
    int x = 130*windowSize;
    int y = 80*windowSize;
    for (int i=0;i<prefs.length();i++) {
        buts[i] = addButton(prefs[i], x, y, box);
        buts[i]->setText(prefs[i]);
        buts[i]->setProperty("button", props[i]);
        buts[i]->setFixedSize(230*windowSize, 50*windowSize);
        buts[i]->setObjectName("butoff");
        connect(buts[i] , SIGNAL(released()), this, SLOT(setupReleased()));
        y += 75*windowSize;
    }
    if (heatingStages == 1) {
        buts[0]->setObjectName("");
    } else {
        buts[1]->setObjectName("");
    }

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

    QPushButton *cancel = addButton(tr("CANCEL"), 20*windowSize, 230*windowSize, box);
    cancel->setProperty("button", prop);

    QPushButton *done = addButton(tr("DONE"), 390*windowSize, 230*windowSize, box);
    done->setProperty("button", prop);
    done->setFixedWidth(75*windowSize);

    connect(cancel , SIGNAL(released()), this,SLOT(cancelReleased()));
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
    qDebug() << "ssBoxChanged"<<ss;
    if (ss == "off") {
        screenTimer->stop();
    } else {
        screenSaverTime = ss.toInt();
        qDebug() << "ssBoxChanged:active"<<screenTimer->isActive();
        screenTimer->start(60000);
    }
}

void Ranger::calendarChanged() {}

void Ranger::dateChanged(QDate dt) {
    qDebug() << "dateChanged" << dt.toString();
    dayLabel->setText(dt.toString("ddd M/d/yy"));
    currentDate = dt;
}

void Ranger::timeChanged(QTime tm) {
    qDebug() << "timeChanged" << tm.toString();
    timeLabel->setText(tm.toString("hh:mm A"));
    currentTime = tm;
}

void Ranger::idleClicked() {
    qDebug() << "idleClicked"<<keyboardLockout <<lockCode;
    screenSaverCount = 0;
    idlePanel->setVisible(false);
    if (keyboardLockout == "keyboardLocked") {
        lockPanel = createLockPanel(true);
        lockPanel->setVisible(true);
    } else {
        currentPanel->setVisible(true);
        screenTimer->start(60000);
    }
}

void Ranger::restartTimer() {
    screenTimer->start(60000);
}

void Ranger::buttonReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() <<"buttonReleased" << p<<selectedRoom;

    if (p == "abcButton") {
        if (saveShift != NULL) {
            saveShift->setStyleSheet(offButton);
        }
        numberButton->setObjectName("key");
        numBox->setVisible(false);
        alphaBox->setVisible(true);
        currentPanel = alphaBox;
    } else if (p == "numButton"){
        if (saveShift != NULL) {
            saveShift->setStyleSheet(offButton);
        }
        abcButton->setObjectName("key");
        alphaBox->setVisible(false);
        numBox->setVisible(true);
        currentPanel = numBox;
    } else if (p == "addNewLocation"){
        addNewLocation = true;
        thermoNameText = "";
        keyboardText = "";
        thermostatLocationPanel->setVisible(false);
        editThermoNamePanel = createEditThermoNamePanel("newThermostat");
        editThermoNamePanel->setVisible(true);
        currentPanel = editThermoNamePanel;
    } else if (p == "weekend" || p == "weekday"){
        settingsSchedulePanel->setVisible(false);
        editSchedulePanel->setVisible(false);
        selectedDay = p.toUpper();
        editSchedulePanel = createEditSchedulePanel();
        editSchedulePanel->setVisible(true);
        currentPanel = editSchedulePanel;
    } else if (p == "changeName"){
        keyboardText = thermostats[selectedRoom].getRoom().simplified();
        editThermoNamePanel = createEditThermoNamePanel(selectedRoom);
        editThermoNamePanel->setVisible(true);
        currentPanel = editThermoNamePanel;
        addNewLocation = false;
    } else if (p == "delete_thermostat"){
        editRoomPanel->setVisible(false);
        deleteThermoPanel = createDeleteThermoPanel();
        deleteThermoPanel->setVisible(true);
        currentPanel = deleteThermoPanel;
    } else if (p == "settings"){
        mainPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "deleteNo"){
        deleteThermoPanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "deleteYes"){
        thermostats.remove(selectedRoom);
        deleteThermoPanel->setVisible(false);
        thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "myNetwork"){
        keyboardText =  wifiPasswordText;
        wifiPanel->setVisible(false);
        wifiPasswordPanel = createWifiPasswordPanel("wifiPassword");
        wifiPasswordPanel->setVisible(true);
        currentPanel = wifiPasswordPanel;
    } else if (p == "disconnect"){
        wifiSetupPanel->setVisible(false);
        disconnectPanel = createDisconnectPanel();
        disconnectPanel->setVisible(true);
        currentPanel = disconnectPanel;
    } else if (p == "disconnectYes"){
        disconnectPanel->setVisible(false);
        wifiPanel = createWifiPanel();
        wifiPanel->setVisible(true);
        currentPanel = wifiPanel;
    } else if (p == "disconnectNo"){
        disconnectPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "later"){
        firstTime = 0;
        wifiPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        configObject["first_time"] = QString("0");
        writeConfig(configFile);
    } else if (p == "lockCodeYes"){
        lockErrorPanel->setVisible(false);
        lockPanel = createLockPanel(true);
        lockPanel->setVisible(true);
    } else if (p == "lockCodeNo"){
        lockErrorPanel->setVisible(false);
        idlePanel = createIdlePanel();
        idlePanel->setVisible(true);
     }

   restartTimer();
}

void Ranger::prefsReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() <<"prefsReleased"<<p;
    QString style = "padding-left:4px;padding-right:4px;border: 2px solid #aaa;border-radius:10px;background-color:#e2e2e2;";

    if (p == "schedulingOptions") {
        preferencesPanel->setVisible(false);
        schedulingOptionsPanel = createSchedulingOptionsPanel();
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
    } else if (p == "systemType") {
        setupPanel->setVisible(false);
        systemTypePanel = createSystemTypePanel();
        systemTypePanel->setVisible(true);
        currentPanel = systemTypePanel;
    } else if (p == "thermostatControls") {
        setupPanel->setVisible(false);
        thermostatControlsPanel = createThermostatControlsPanel();
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "schedulePeriods") {
        preferencesPanel->setVisible(false);
        schedulePeriodsPanel = createSchedulePeriodsPanel();
        schedulePeriodsPanel->setVisible(true);
        currentPanel = schedulePeriodsPanel;
    } else if (p.startsWith("keyboardLockout")) {
        preferencesPanel->setVisible(false);
        keyboardLockoutPanel = createKeyboardLockoutPanel();
        keyboardLockoutPanel->setVisible(true);
        currentPanel = keyboardLockoutPanel;
    } else if (p == "tempuratureLimits") {
        preferencesPanel->setVisible(false);
        tempuratureLimitsPanel = createTempuratureLimitsPanel();
        tempuratureLimitsPanel->setVisible(true);
        currentPanel = tempuratureLimitsPanel;
    } else if (p == "systemsChangeOver") {
        preferencesPanel->setVisible(false);
        systemsChangeOverPanel = createSytemsChangeOverPanel();
        systemsChangeOverPanel->setVisible(true);
        currentPanel = systemsChangeOverPanel;
    } else if (p == "dst") {
        preferencesPanel->setVisible(false);
        dstPanel = createDstPanel();
        dstPanel->setVisible(true);
        currentPanel = dstPanel;
    } else if (p == "clockFormat") {
        preferencesPanel->setVisible(false);
        clockFormatPanel = createClockFormatPanel();
        clockFormatPanel->setVisible(true);
        currentPanel = clockFormatPanel;
    } else if (p == "tempuratureScale") {
        preferencesPanel->setVisible(false);
        tempuratureScalePanel = createTempuratureScalePanel();
        tempuratureScalePanel->setVisible(true);
        currentPanel = tempuratureScalePanel;
    } else if (p == "non-programmable" || p == "programmable") {
        schedulingOption = p;
        schedulingOptionsPanel->setVisible(false);
        schedulingOptionsPanel = createSchedulingOptionsPanel();
        schedulingOptionsPanel->setVisible(true);
        currentPanel = schedulingOptionsPanel;
    } else if (p == "F" || p == "C") {
        tempuratureScale = (p == "C");
        tempuratureScalePanel->setVisible(false);
        tempuratureScalePanel = createTempuratureScalePanel();
        tempuratureScalePanel->setVisible(true);
        currentPanel = tempuratureScalePanel;
    } else if (p == "12hour" || p == "24hour") {
        if (p == "12hour") {
            clockFormat = 12;
        } else {
            clockFormat = 24;
         }
        clockFormatPanel->setVisible(false);
        clockFormatPanel = createClockFormatPanel();
        clockFormatPanel->setVisible(true);
        currentPanel = clockFormatPanel;
    } else if (p == "dstOn" || p == "dstOff") {
        if (p == "dstOff") {
            dst = false;
        } else {
            dst = true;
        }
        dstPanel->setVisible(false);
        dstPanel = createDstPanel();
        dstPanel->setVisible(true);
        currentPanel = dstPanel;
    } else if (p == "manualChangeover" || p == "automaticChangeover") {
        if (p == "manualChangeover") {
            systemsChangeOver = "manual";
        } else  {
            systemsChangeOver = "automatic";
        }
        systemsChangeOverPanel->setVisible(false);
        systemsChangeOverPanel = createSytemsChangeOverPanel();
        systemsChangeOverPanel->setVisible(true);
        currentPanel = systemsChangeOverPanel;
    } else if (p == "keyboardUnlocked" || p == "keyboardPartiallyUnlocked" || p == "keyboardLocked") {
        if (p == "keyboardUnlocked") {
           keyboardLockout = "unlocked";
        } else if (p == "keyboardPartiallyUnlocked") {
            keyboardLockout = "partially_unlocked";
         } else {
            keyboardLockout = "locked";
        }

        keyboardLockout = p;
        keyboardLockoutPanel->setVisible(false);
        keyboardLockoutPanel = createKeyboardLockoutPanel();
        keyboardLockoutPanel->setVisible(true);
        currentPanel = keyboardLockoutPanel;
    } else if (p == "2perday" || p == "4perday") {
        if (p == "2perday") {
            schedulePeriods = 2;
        } else {
            schedulePeriods = 4;
         }
        schedulePeriodsPanel->setVisible(false);
        schedulePeriodsPanel = createSchedulePeriodsPanel();
        schedulePeriodsPanel->setVisible(true);
        currentPanel = schedulePeriodsPanel;
    }

    restartTimer();
}

void Ranger::setupReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() <<"setupReleased"<<p;

    if (p == "language") {
        setupPanel->setVisible(false);
        languagesPanel = createLanguagesPanel();
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "english" || p == "spanish") {
        language = p;
        languagesPanel = createLanguagesPanel();
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "systemType") {
        setupPanel->setVisible(false);
        systemTypePanel = createSystemTypePanel();
        systemTypePanel->setVisible(true);
        currentPanel = systemTypePanel;
    } else if (p == "forcedAir" || p == "heatPump" || p == "waterOrSteam") {
        if (p  == "forcedAir") {
            systemType = "air";
        } else if (p  == "heatPump") {
            systemType = "heat";
        } else {
            systemType = "water";
        }
        systemTypePanel->setVisible(false);
        systemTypePanel = createSystemTypePanel();
        systemTypePanel->setVisible(true);
        currentPanel = systemTypePanel;
    } else if (p == "wifiSetup") {
        setupPanel->setVisible(false);
        wifiSetupPanel = createWifiSetupPanel();
        wifiSetupPanel->setVisible(true);
        currentPanel = wifiSetupPanel;
    } else if (p == "fanControl") {
        setupPanel->setVisible(false);
        fanControlPanel = createFanControlPanel();
        fanControlPanel->setVisible(true);
        currentPanel = fanControlPanel;
    } else if (p == "thermostat" || p == "heating") {
        fanControl = p;
        fanControlPanel->setVisible(false);
        fanControlPanel = createFanControlPanel();
        fanControlPanel->setVisible(true);
        currentPanel = fanControlPanel;
    } else if (p == "backupHeat") {
        setupPanel->setVisible(false);
        backupHeatPanel = createBackupHeatPanel();
        backupHeatPanel->setVisible(true);
        currentPanel = backupHeatPanel;
    } else if (p == "backupHeatYes" || p == "backupHeatNo") {
        backupHeat = (p == "backupHeatYes") ? "yes" : "no";
        backupHeatPanel->setVisible(false);
        backupHeatPanel = createBackupHeatPanel();
        backupHeatPanel->setVisible(true);
        currentPanel = backupHeatPanel;
    } else if (p == "coolingStages") {
        setupPanel->setVisible(false);
        coolingStagesPanel = createCoolingStagesPanel();
        coolingStagesPanel->setVisible(true);
        currentPanel = coolingStagesPanel;
    } else if (p == "cooling1stage" || p == "cooling2stage") {
        coolingStages = (p == "cooling1stage") ? 1 : 2;
        coolingStagesPanel->setVisible(false);
        coolingStagesPanel = createCoolingStagesPanel();
        coolingStagesPanel->setVisible(true);
        currentPanel = coolingStagesPanel;
    } else if (p == "heatingStages") {
        setupPanel->setVisible(false);
        heatingStagesPanel = createHeatingStagesPanel();
        heatingStagesPanel->setVisible(true);
        currentPanel = heatingStagesPanel;
    } else if (p == "heating1stage" || p == "heating2stage") {
        heatingStages = (p == "heating1stage") ? 1 : 2;
        heatingStagesPanel->setVisible(false);
        heatingStagesPanel = createHeatingStagesPanel();
        heatingStagesPanel->setVisible(true);
        currentPanel = heatingStagesPanel;
    } else if (p == "changeOverValve") {
        setupPanel->setVisible(false);
        changeOverValvePanel = createChangeOverValvePanel();
        changeOverValvePanel->setVisible(true);
        currentPanel = changeOverValvePanel;
    } else if (p == "changeoverValveCooling" || p == "changeoverValveHeating") {
        changeOverValve = (p == "changeoverValveCooling") ? "cool" : "heat";
        changeOverValvePanel->setVisible(false);
        changeOverValvePanel = createChangeOverValvePanel();
        changeOverValvePanel->setVisible(true);
        currentPanel = changeOverValvePanel;
    } else if (p == "thermostatControls") {
        setupPanel->setVisible(false);
        thermostatControlsPanel = createThermostatControlsPanel();
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
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
    }

    restartTimer();
}

void Ranger::profileReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() <<"profileReleased" << p;

    if (p == "firstNameEdit"){
        profilePanel->setVisible(false);
        registerPanel = createRegisterPanel("profile", true);
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
        profilePanel->setVisible(false);
        registerPanel = createRegisterPanel("customerName", true);
        registerPanel->setVisible(true);
        currentPanel = registerPanel;
    } else if (p == "zipCode"){
        keyboardText = zipCodeText;
        profilePanel->setVisible(false);
        registerPanel = createRegisterPanel("zipCode", false);
        registerPanel->setVisible(true);
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

void Ranger::returnPressed() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "returnPressed"<<p;

    if (p == "settings") {
        settingsPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "schedule") {
        settingsSchedulePanel->setVisible(false);
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
    qDebug() << "upReleased"<<p;

    if (p == "heatToDegrees") {
        heatToDegrees++;
        heatToDegreesLabel->setText(QString::number(heatToDegrees));
        heatLabel->setText(QString::number(heatToDegrees) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "heat") {
            statControl = heatToDegrees;
        }
    } else if (p == "heatToTime"){
        heatToTime += 60;
        heatToTimeText = QDateTime::fromTime_t(heatToTime).toString("h:mm A");
        QStringList tm = heatToTimeText.split(" ");
        heatToTimeLabel->setText(tm[0]);
    } else if (p == "coolToDegrees"){
        coolToDegrees++;
        coolToDegreesLabel->setText(QString::number(coolToDegrees));
        coolLabel->setText(QString::number(coolToDegrees) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "cool") {
            statControl = coolToDegrees;
        }
    } else if (p == "coolToTime"){
        coolToTime += 60;
        coolToTimeText = QDateTime::fromTime_t(coolToTime).toString("h:mm A");
        QStringList tm = coolToTimeText.split(" ");
        coolToTimeLabel->setText(tm[0]);
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
        configObject["temp_set"] = statControl;
        writeConfig(configFile);
    } else if (p == "editSchedule"){
        //qDebug() << "I"<<currentIndex;
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
    qDebug() << "downReleased"<<p;
    ((QPushButton*)sender())->setIcon(*dnIcon);

    if (p == "heatToDegrees") {
        heatToDegrees--;
        heatToDegreesLabel->setText(QString::number(heatToDegrees));
        heatLabel->setText(QString::number(heatToDegrees) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "head") {
            statControl = heatToDegrees;
        }
    } else if (p == "heatToTime"){
        heatToTime -= 60;
        heatToTimeText = QDateTime::fromTime_t(heatToTime).toString("h:mm A");
        QStringList tm = heatToTimeText.split(" ");
        heatToTimeLabel->setText(tm[0]);
    } else if (p == "coolToDegrees"){
        coolToDegrees --;
        coolToDegreesLabel->setText(QString::number(coolToDegrees));
        coolLabel->setText(QString::number(coolToDegrees) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "head") {
            statControl = coolToDegrees;
        }
    } else if (p == "sched"){
        if (schedText->text() == "Automatic") {
            schedText->setText("Manual");
        } else {
            schedText->setText("Automatic");
         }
    } else if (p == "coolToTime"){
        coolToTime -= 60;
        coolToTimeText = QDateTime::fromTime_t(coolToTime).toString("h:mm A");
        QStringList tm = coolToTimeText.split(" ");
        coolToTimeLabel->setText(tm[0]);
    } else if (p == "main"){
        tempLabel->setText(QString::number(--statControl) + degreeCode.at(degreeCode.length()-1));
        if (selectedMode == "cool") {
            coolToDegrees = statControl;
            coolLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        } else if (selectedMode == "heat") {
            heatToDegrees = statControl;
            heatLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
         }
        configObject["temp_set"] = QString::number(statControl);
        writeConfig(configFile);
    } else if (p == "editSchedule"){
        if (currentIndex == 0) {
            if (periodCounter <= 0) periodCounter = 4;
            periodLabel->setText(periodList[--periodCounter]);
            currentDailySchedule = currentSchedule[periodLabel->text()];
            startLabel->setText(currentDailySchedule.getStartTime());
            heatToLabel->setText(QString::number(currentDailySchedule.getHeatTo())+degreeCode.at(degreeCode.length()-1));
            coolToLabel->setText(QString::number(currentDailySchedule.getCoolTo())+degreeCode.at(degreeCode.length()-1));
        }
        if (currentIndex == 1) {
            QDateTime dt = QDateTime::fromString(startLabel->text(),"h:mma").addSecs(-60);
            startLabel->setText(dt.toString("h:mma"));
            currentDailySchedule.setStartTime(dt.toString("h:mma"));
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
         }
        if (currentIndex == 2) {
            QDateTime dt = QDateTime::fromString(stopLabel->text(),"h:mma").addSecs(-60);
            stopLabel->setText(dt.toString("h:mma"));
            currentDailySchedule.setStopTime(dt.toString("h:mma"));
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
        }
        if (currentIndex == 3) {
            int d = currentDailySchedule.getHeatTo() - 1;
            heatToLabel->setText(QString::number(d) + degreeCode.at(degreeCode.length()-1));
            currentDailySchedule.setHeatTo(d);
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
        }
        if (currentIndex == 4) {
            int d = currentDailySchedule.getCoolTo() - 1;
            coolToLabel->setText(QString::number(d) + degreeCode.at(degreeCode.length()-1));
            currentDailySchedule.setCoolTo(d);
            currentSchedule[periodLabel->text()] = currentDailySchedule;
            dailySchedules[selectedDay] = currentSchedule;
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
    qDebug() << "upNumReleased"<<p;

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
    qDebug() << "downNumReleased"<<p;
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
    qDebug() << "backReleased"<<p;

    if (p == "profileBack") {
        profilePanel->setVisible(false);
        languagesPanel = createLanguagesPanel();
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "thermostatLocationBack") {
        thermostatLocationPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "thermostatControlsBack") {
        thermostatControlsPanel->setVisible(false);
        thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "systemTypeBack") {
        systemTypePanel->setVisible(false);
        thermostatControlsPanel = createThermostatControlsPanel();
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "wifiBack") {
        wifiPanel->setVisible(false);
        systemTypePanel = createSystemTypePanel();
        systemTypePanel->setVisible(true);
        currentPanel = systemTypePanel;
    }

   restartTimer();
}

void Ranger::nextReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "nextReleased"<<p;

    if (p == "languagesNext1") {
        languagesPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "languagesNext") {
        languagesPanel->setVisible(false);
        thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "thermostatLocationNext") {
        thermostatLocationPanel->setVisible(false);
        thermostatControlsPanel = createThermostatControlsPanel();
        thermostatControlsPanel->setVisible(true);
        currentPanel = thermostatControlsPanel;
    } else if (p == "thermostatControlsNext") {
        thermostatControlsPanel->setVisible(false);
        systemTypePanel = createSystemTypePanel();
        systemTypePanel->setVisible(true);
        currentPanel = systemTypePanel;
    } else if (p == "systemTypeNext") {
        thermostatControlsPanel->setVisible(false);
        wifiPanel = createWifiPanel();
        wifiPanel->setVisible(true);
        currentPanel = wifiPanel;
    }

    restartTimer();
}

void Ranger::cancelReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "cancelReleased"<< p;

    if (p == "editRoomCancel") {
        editRoomPanel->setVisible(false);
        thermostatLocationPanel  = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "selectMode") {
        selectModePanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "fanMode") {
        fanModePanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "heatTo") {
        heatPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "coolTo") {
        coolPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "editSchedule") {
        editSchedulePanel->setVisible(false);
        if (selectedDay == "ALL DAYS" || selectedDay.startsWith("WEEK") ||
                selectedDay == "VACATION") {
            settingsSchedulePanel = createSettingsSchedulePanel();
            settingsSchedulePanel->setVisible(true);
            currentPanel = settingsSchedulePanel;
        } else {
            dailySchedulePanel = createDailySchedulePanel();
            dailySchedulePanel->setVisible(true);
            currentPanel = dailySchedulePanel;
        }
    } else if (p == "locationCancel") {
        thermostatLocationPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "colors") {
        setColorScheme(currentScheme);
        colorsPanel->setVisible(false);
        settingsPanel  = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "language") {
        languagesPanel->setVisible(false);
        settingsPanel  = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "profile") {
        profilePanel->setVisible(false);
        settingsPanel  = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "timeDate") {
        timeDatePanel->setVisible(false);
        settingsPanel  = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "editRoom") {
        editRoomPanel->setVisible(false);
        if (firstTime) {
            thermostatLocationPanel = createThermostatLocationPanel();
            thermostatLocationPanel->setVisible(true);
            currentPanel = thermostatLocationPanel;
        } else {
            profilePanel = createProfilePanel();
            profilePanel->setVisible(true);
            currentPanel = profilePanel;
        }
    } else if (p == "lock") {
        lockPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = lockPanel;
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "systemType") {
        systemTypePanel->setVisible(false);
        setupPanel->setVisible(true);
        currentPanel = systemTypePanel;
    } else if (p == "setup") {
        setupPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "dailySchedule") {
        dailySchedulePanel->setVisible(false);
        settingsSchedulePanel = createSettingsSchedulePanel();
        settingsSchedulePanel->setVisible(true);
        currentPanel = settingsSchedulePanel;
    } else if (p == "cancelEdit") {
        thermostatLocationPanel->setVisible(false);
        registerPanel = createRegisterPanel("name", true);
        registerPanel->setVisible(true);
        currentPanel = registerPanel;
    } else if (p == "deleteThermo") {
        deleteThermoPanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "schedulingOptions") {
        schedulingOptionsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "schedulePeriods") {
        schedulePeriodsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "keyboardLockout") {
        keyboardLockoutPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "tempuratureLimits") {
        tempuratureLimitsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "systemsChangeover") {
        systemsChangeOverPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "dst") {
        dstPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "clockFormat") {
        clockFormatPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "tempuratureScale") {
        tempuratureScalePanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "systemType") {
        systemTypePanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "thermostatControls") {
        thermostatControlsPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p.startsWith("thermo")) {
        editThermoNamePanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "fanControl") {
        fanControlPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "backupHeat") {
        backupHeatPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "coolingStages") {
        coolingStagesPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "heatingStages") {
        heatingStagesPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "changeOverValve") {
        changeOverValvePanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "customerName") {
        registerPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "zipCode") {
        registerPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "wifiSetup") {
        wifiSetupPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "wifi") {
        wifiSetupPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "wifiPassword") {
        wifiPasswordPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    }

    restartTimer();
}

void Ranger::editReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "editPressed"<< p;

    if (p == "editRoomCancel") {
        editRoomPanel->setVisible(false);
        thermostatLocationPanel->setVisible(true);
        currentPanel = thermostatLocationPanel;
    } else if (p == "selectModeCancel") {
        selectModePanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "fanModeCancel") {
        fanModePanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "heatCancel") {
        heatPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "coolCancel") {
        coolPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "dailySchedule") {
        dailySchedulePanel->setVisible(false);
        editSchedulePanel = createEditSchedulePanel();
        editSchedulePanel->setVisible(true);
        currentPanel = editSchedulePanel;
    }

   restartTimer();
}

void Ranger::doneReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "doneReleased"<<p<<lockCode;

    if (p.startsWith("thermo")) {
        thermoNameText = keyboardEdit->text();
        thermostats[selectedRoom].setRoom(keyboardEdit->text());
        editThermoNamePanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "selectMode") {
        if (selectedModeTemp != NULL) selectedMode = selectedModeTemp;
        modeLabel->setText(selectedMode);
        selectModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "fanMode") {
        if (selectedFanModeTemp != NULL)  selectedFanMode = selectedFanModeTemp;
        fanLabel->setText(selectedFanMode);
        fanModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "editSchedule") {
        editSchedulePanel->setVisible(false);
        if (selectedDay == "ALL DAYS" || selectedDay.startsWith("WEEK") ||
                selectedDay == "VACATION") {
            settingsSchedulePanel = createSettingsSchedulePanel();
            settingsSchedulePanel->setVisible(true);
            currentPanel = settingsSchedulePanel;
        } else {
            selectedDaySchedule = currentSchedule;
            dailySchedules[selectedDay] = currentSchedule;
            dailySchedulePanel = createDailySchedulePanel();
            dailySchedulePanel->setVisible(true);
            currentPanel = dailySchedulePanel;
        }
    } else if (p == "settingsSchedule") {
        settingsSchedulePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "languages") {
        languagesPanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "profile") {
        profilePanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "timeDate") {
        timeDatePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "setup") {
        setupPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "deleteThermo") {
        deleteThermoPanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "editRoom") {
        editRoomPanel->setVisible(false);
        if (firstTime) {
            thermostatLocationPanel = createThermostatLocationPanel();
            thermostatLocationPanel->setVisible(true);
            currentPanel = thermostatLocationPanel;
        } else {
            profilePanel = createProfilePanel();
            profilePanel->setVisible(true);
            currentPanel = profilePanel;
        }

        //props.insert(selectedRoom, thermoName->text());
        //writeProperties(propertyOutFile);
    } else if (p == "newThermostat") {
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
    } else if (p == "customerName") {
        customerNameText = keyboardEdit->text();
        registerPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "zipCode") {
        zipCodeText = keyboardEdit->text();
        registerPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "wifiPassword") {
        wifiPasswordText = keyboardEdit->text();
        wifiPasswordPanel->setVisible(false);
        connectionSuccesfulPanel = createConnectionSuccesfulPanel();
        connectionSuccesfulPanel->setVisible(true);
        currentPanel = connectionSuccesfulPanel;
    } else if (p == "connectionSuccesful") {
        firstTime = 0;
        connectionSuccesfulPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        configObject["first_time"] = QString("0");
        writeConfig(configFile);
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
    } else if (p == "wifi") {
        wifiSetupPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
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
    }

    restartTimer();
}

void Ranger::saveReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "saveReleased"<<p;

    if (p == "thermo") {
        editThermoNamePanel->setVisible(false);
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "selectMode") {
        if (selectedModeTemp != NULL) selectedMode = selectedModeTemp;
        modeLabel->setText(selectedMode);
        selectModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    } else if (p == "fanMode") {
        if (selectedFanModeTemp != NULL)  selectedFanMode = selectedFanModeTemp;
        fanLabel->setText(selectedFanMode);
        fanModePanel->setVisible(false);
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        configObject["fan_mode"] = selectedFanMode;
    } else if (p == "heatTo") {
        heatPanel->setVisible(false);
        if (selectedMode == "heat") {
            statControl = heatToDegrees;
            tempLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        }
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        configObject["heat_to"] = heatToDegrees;
        configObject["heat_to_time"] = heatToTime;
    } else if (p == "coolTo") {
        coolPanel->setVisible(false);
        if (selectedMode == "cool") {
            statControl = coolToDegrees;
            tempLabel->setText(QString::number(statControl) + degreeCode.at(degreeCode.length()-1));
        }
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
        configObject["cool_to"] = coolToDegrees;
        configObject["cool_to_time"] = coolToTime;
    } else if (p == "editSchedule") {
        editSchedulePanel->setVisible(false);
        if (selectedDay == "ALL DAYS" || selectedDay.startsWith("WEEK") ||
                selectedDay == "VACATION") {
            settingsSchedulePanel = createSettingsSchedulePanel();
            settingsSchedulePanel->setVisible(true);
            currentPanel = settingsSchedulePanel;
        } else {
            selectedDaySchedule = currentSchedule;
            dailySchedules[selectedDay] = currentSchedule;
            dailySchedulePanel = createDailySchedulePanel();
            dailySchedulePanel->setVisible(true);
            currentPanel = dailySchedulePanel;
        }
    } else if (p == "settingsSchedule") {
        settingsSchedulePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "colors") {
        currentScheme = currentSchemeTemp;
        upIcon = getButtonIcon(0., 1., "#ccc");
        upIconPressed = getButtonIcon(0., 1., rangerThemes[currentScheme].getBorderColorPressed());

        dnIcon = getButtonIcon(180., 1., "#ccc");
        dnIconPressed = getButtonIcon(180., 1., rangerThemes[currentScheme].getBorderColorPressed());

        colorsPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;

        configObject["theme"] = currentScheme;
    } else if (p == "language") {
        languagesPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
        configObject["language"] = language;
    } else if (p == "profile") {
        profilePanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
        configObject["zip_code"] = zipCodeText;
        configObject["first_name"] = customerNameText;
        writeConfig(configFile);
    } else if (p == "timeDate") {
        timeDatePanel->setVisible(false);
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "alphaname" || p == "numericname") {
        registerPanel->setVisible(false);
        thermostatLocationPanel = createThermostatLocationPanel();
        thermostatLocationPanel->setVisible(true);
        customerNameText = firstName->text();
        currentPanel = thermostatLocationPanel;
        configObject["first_name"] = customerNameText;
        writeConfig(configFile);
    } else if (p == "alphaprofile" || p == "numericprofile") {
        registerPanel->setVisible(false);
        customerNameText = firstName->text();
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
        configObject["first_name"] = customerNameText;
        writeConfig(configFile);
    } else if (p == "alphathermo" || p == "numericthermo") {
        editThermoNamePanel->setVisible(false);
        qDebug() << "I2="<<thermostats.size();
        thermoNameText = thermoName->text();
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "lock") {
        lockPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = lockPanel;
        lockCode = lockNums[0]->text() + lockNums[1]->text() + lockNums[2]->text() + lockNums[3]->text();
        configObject["lockCode"] = lockCode;
        writeConfig(configFile);
    } else if (p == "preferences") {
        preferencesPanel->setVisible(false);
        settingsPanel = createSettingsPanel();
        settingsPanel->setVisible(true);
        currentPanel = settingsPanel;
    } else if (p == "deleteThermo") {
        deleteThermoPanel->setVisible(false);
        editRoomPanel = createEditRoomPanel();
        editRoomPanel->setVisible(true);
        currentPanel = editRoomPanel;
    } else if (p == "thermostatControls") {
        thermostatControlsPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
        configObject["thermo_controls"] = thermostatControls;
    } else if (p == "editRoom") {
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
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;

        //props.insert(selectedRoom, thermoName->text());
        //writeProperties(propertyOutFile);
    } else if (p == "schedulingOptions") {
        schedulingOptionsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
        configObject["scheduling_option"] = schedulingOption;
    } else if (p == "schedulePeriods") {
        schedulePeriodsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
        configObject["schedule_periods"] = schedulePeriods;
    } else if (p == "keyboardLockout") {
        keyboardLockoutPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
        configObject["keyboard_lock"] = keyboardLockout;
    } else if (p == "tempuratureLimits") {
        tempuratureLimitsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "systemsChangeover") {
        systemsChangeOverPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "dst") {
        dstPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
        configObject["dst"] = dst;
    } else if (p == "clockFormat") {
        clockFormatPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
        configObject["clock_format"] = clockFormat;
    } else if (p == "tempuratureScale") {
        tempuratureScalePanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
        configObject["metric"] = tempuratureScale;
        writeConfig(configFile);
    } else if (p == "systemType") {
        systemTypePanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
        configObject["system_type"] = systemType;
        writeConfig(configFile);
    } else if (p == "fanControl") {
        fanControlPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
        configObject["fan_control"] = fanControl;
    } else if (p == "backupHeat") {
        backupHeatPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
        configObject["backup_heat"] = QString((backupHeat == "backupHeatYes") ? "yes" : "no");
    } else if (p == "coolingStages") {
        coolingStagesPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
        configObject["cooling_stage"] = coolingStages;
    } else if (p == "heatingStages") {
        heatingStagesPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
        configObject["heating_stage"] = heatingStages;
    } else if (p == "changeOverValve") {
        changeOverValvePanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    } else if (p == "connectionSuccesful") {
        connectionSuccesfulPanel->setVisible(false);
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        currentPanel = mainPanel;
    }

    writeConfig(configFile);
    restartTimer();
}

void Ranger::scheduleButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    qDebug() << "scheduleButtonReleased" << p;
    selectedSchedule = p;

    for (int i=0;i<4;i++) {
        scheduleButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");

    if (p == "all_days") {
        selectedDay = "ALL DAYS";
        settingsSchedulePanel->setVisible(false);
        editSchedulePanel = createEditSchedulePanel();
        editSchedulePanel->setVisible(true);
        currentPanel = editSchedulePanel;
    } else if (p == "weekday_weekend") {
        selectedDay = "WEEKEND";
        settingsSchedulePanel->setVisible(false);
        editSchedulePanel = createEditSchedulePanel();
        editSchedulePanel->setVisible(true);
        currentPanel = editSchedulePanel;
    } else if (p == "each_day") {
        selectedDay = "MONDAY";
        selectedDaySchedule = dailySchedules[selectedDay];
        settingsSchedulePanel->setVisible(false);
        dailySchedulePanel = createDailySchedulePanel();
        dailySchedulePanel->setVisible(true);
        currentPanel = dailySchedulePanel;
    } else if (p == "vacation") {
        selectedDay = "VACATION";
        settingsSchedulePanel->setVisible(false);
        editSchedulePanel = createEditSchedulePanel();
        editSchedulePanel->setVisible(true);
        currentPanel = editSchedulePanel;
    }

   configObject["schedule"] = selectedSchedule;
   writeConfig(configFile);
   restartTimer();
}

void Ranger::settingsButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    qDebug() << "settingsButtonReleased" << p;

    if (p == "time_date") {
        settingsPanel->setVisible(false);
        timeDatePanel = createTimeDatePanel();
        timeDatePanel->setVisible(true);
        currentPanel = timeDatePanel;
    } else if (p == "lock") {
        settingsPanel->setVisible(false);
        lockPanel = createLockPanel(false);
        lockPanel->setVisible(true);
        currentPanel = lockPanel;
    } else if (p == "schedule") {
        settingsPanel->setVisible(false);
        settingsSchedulePanel = createSettingsSchedulePanel();
        settingsSchedulePanel->setVisible(true);
        currentPanel = settingsSchedulePanel;
    } else if (p == "languages") {
        settingsPanel->setVisible(false);
        languagesPanel = createLanguagesPanel();
        languagesPanel->setVisible(true);
        currentPanel = languagesPanel;
    } else if (p == "colors") {
        settingsPanel->setVisible(false);
        colorsPanel = createColorsPanel();
        colorsPanel->setVisible(true);
        currentPanel = colorsPanel;
    } else if (p == "preferences") {
        settingsPanel->setVisible(false);
        preferencesPanel = createPreferencesPanel();
        preferencesPanel->setVisible(true);
        currentPanel = preferencesPanel;
    } else if (p == "profile") {
        settingsPanel->setVisible(false);
        profilePanel = createProfilePanel();
        profilePanel->setVisible(true);
        currentPanel = profilePanel;
    } else if (p == "setup") {
        settingsPanel->setVisible(false);
        setupPanel = createSetupPanel();
        setupPanel->setVisible(true);
        currentPanel = setupPanel;
    }

    restartTimer();
}

void Ranger::editScheduleButtonReleased(QAbstractButton* button) {
    qDebug() << "editScheduleButtonReleased" << button->text();
    for (int i=0;i<5;i++) {
        editScheduleButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setColorScheme(currentScheme);

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
    qDebug() << "locationButtonReleased" << p;

    thermoNameText = thermostats[p].getRoom().simplified();
    selectedRoom = p;
    if (firstTime) {
        thermostatLocationPanel->setVisible(false);
    } else {
        profilePanel->setVisible(false);
    }
    editRoomPanel = createEditRoomPanel();
    editRoomPanel->setVisible(true);
    currentPanel = editRoomPanel;

   restartTimer();
}

void Ranger::keyBackReleased() {
    QString p = ((QPushButton*)sender())->property("button").toString();
    qDebug() << "keyBackReleased"<<p;

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
    qDebug() << "keyButtonReleased" << button->text()<<p;

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

        alphaBox->setVisible(false);
        if (p.startsWith("thermo")) {
            alphaBox = alphaKeyboard(editThermoNamePanel, p);
        } else if (p == "wifiPassword") {
            alphaBox = alphaKeyboard(wifiPasswordPanel, p);
        } else if (p == "customerName") {
            alphaBox = alphaKeyboard(registerPanel, p);
        }
        alphaBox->setVisible(true);
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

void Ranger::colorButtonReleased(QAbstractButton* button) {
    qDebug() << "colorButtonReleased" << button->property("button").toString();
    setColorScheme(button->property("button").toString());

    restartTimer();
}

void Ranger::fanButtonReleased(QAbstractButton* button) {
    qDebug() << "fanButtonReleased" << button->text();

    selectedFanModeTemp = button->text();
    for (int i=0;i<2;i++) {
        fanModeButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");
    setColorScheme(currentScheme);

    configObject["fan_mode"] = selectedFanModeTemp;
    writeConfig(configFile);

    restartTimer();
}

void Ranger::dayButtonPressed(QAbstractButton* button) {
    button->setStyleSheet("font-weight:bold;color:white;border:0 none;background-image: none;background-color: transparent;");
}

void Ranger::dayButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    button->setStyleSheet("font-weight:normal;color:#ddd;border:0 none;background-image: none;background-color: transparent;");
    qDebug() << "dayButtonReleased" << p;

    selectedDay = p;
    selectedDayTemp = p;
    dailySchedulePanel->setVisible(false);
    selectedDaySchedule = dailySchedules[selectedDay];
    dailySchedulePanel = createDailySchedulePanel();
    dailySchedulePanel->setVisible(true);
    currentPanel = dailySchedulePanel;

    restartTimer();
}

void Ranger::modeButtonReleased(QAbstractButton* button) {
    selectedModeTemp = button->property("button").toString();
    qDebug() << "modeButtonReleased" << selectedModeTemp;

    for (int i=0;i<5;i++) {
        selectModeButtons[i].setObjectName("butoff");
    }
    button->setObjectName("");

    if (selectedModeTemp != NULL) selectedMode = selectedModeTemp;

    if (selectedMode == "heat") {
        statControl = heatToDegrees;
    } else {
        statControl = coolToDegrees;
    }
    modeLabel->setText(selectedMode);
    selectModePanel->setVisible(false);
    mainPanel = createMainPanel();
    mainPanel->setVisible(true);
    currentPanel = mainPanel;

    configObject["hvac_mode"] = selectedMode;
    writeConfig(configFile);

    restartTimer();
}

void Ranger::mainButtonReleased(QAbstractButton* button) {
    QString p = button->property("button").toString();
    qDebug() << "mainButtonReleased"<<p;

    if (p == "mode") {
        mainPanel->setVisible(false);
        selectModePanel = createSelectModePanel();
        selectModePanel->setVisible(true);
        currentPanel = selectModePanel;
    } else if (p == "fan") {
        mainPanel->setVisible(false);
        fanModePanel = createFanModePanel();
        fanModePanel->setVisible(true);
        currentPanel = fanModePanel;
    } else if (p == "hold" && selectedMode != "off") {
        mainPanel->setVisible(false);
        if (!holdMode) {
            holdMode = true;
            holdLabel->setText(tr("ON HOLD"));
        } else {
            holdLabel->setText(tr("ON SCHED"));
            holdMode = false;
        }
        mainPanel = createMainPanel();
        mainPanel->setVisible(true);
        configObject["tempHold"] = holdMode;
        writeConfig(configFile);
    } else if (p == "heat") {
        mainPanel->setVisible(false);
        heatPanel = createHeatPanel();
        heatPanel->setVisible(true);
        currentPanel = heatPanel;
    } else if (p == "cool") {
        mainPanel->setVisible(false);
        coolPanel = createCoolPanel();
        coolPanel->setVisible(true);
        currentPanel = coolPanel;
    }

    restartTimer();
}

void Ranger::setColorScheme(QString color) {
    QString stylePath;
    currentSchemeTemp = color;
#ifdef Q_OS_WIN
    stylePath = appPath + "/../../ranger/qss/"+color+".qss";
#else
    stylePath = appPath + "/../ranger/qss/"+color+".qss";
#endif
    qDebug() << stylePath;
    QFile file(stylePath);
    bool bOpened = file.open(QFile::ReadOnly);

    QString styleSheet = QLatin1String(file.readAll());

    setStyleSheet(styleSheet);
 }

void Ranger::getConfig() {
    QUrl url("http://166.20.101.69/rest/api/tstat/configs/1234");
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
        qDebug() << "ConfigUtils::getInfo timeout:";
    }
    timer.stop();

    qDebug() << reply->readAll().simplified();
    reply->deleteLater();
}

void Ranger::putConfig() {
    QUrl configUrl("http://166.20.101.69/rest/api/tstat/configs");
    QByteArray postData = configJson.toJson();

    networkManager = new QNetworkAccessManager(this);
    QNetworkRequest rqst(configUrl);
    rqst.setRawHeader("Content-Type", "application/json");
    qDebug() << "0";
    QByteArray strSize = QByteArray().append(QString::number(postData.size()));
    rqst.setRawHeader("Content-Length", strSize);
    qDebug() << "1";

     // connect 'SSL errors' signal with consumer
//    connect(pNetworkReply, SIGNAL(sslErrors(const QList<QSslError> &)),
//            this, SLOT(onSSLError(const QList<QSslError> &)));
    connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                               this, SLOT(loadConfig(QNetworkReply*)));
    QNetworkReply* pNetworkReply = networkManager->post(rqst, postData);
    pNetworkReply->readAll();
}

void Ranger::loadConfig(QNetworkReply *reply) {
    qDebug() << "loadConfig:" << " host:" << reply->url().host();
    if (reply->error() == 0) {
        //configReply->deleteLater();
        qDebug() << "REPLY"<<reply->readAll();
    } else {
        qDebug() << "putConfig reply error:" << reply->errorString();
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
    qDebug() << appPath;

    QFontDatabase fontDB;
    fontDB.addApplicationFont(":/fonts/DroidSans.ttf");
    fontDB.addApplicationFont(":/fonts/DroidSans-Bold.ttf");
//    foreach(QString s, fontDB.families()) {
//        qDebug() << s;
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
