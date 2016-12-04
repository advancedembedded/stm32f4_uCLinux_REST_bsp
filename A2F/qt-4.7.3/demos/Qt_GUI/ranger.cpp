#include "ranger.h"
#include "gui.h"

QString appPath;

Ranger::Ranger(QWidget *parent)
    : QMainWindow(parent)
{
    border_radius = 6;
    border_width = 3;
    border_color = "#cacaca";
    background_color = "#679967";
    highlight_color = "#677767";
    statControl = 60;
    fan = false;
    mode = false;

    propertyFile = appPath + "/../ranger/ranger.props";

    readProperties(propertyFile);

    //watcher = new QFileSystemWatcher();
    //watcher->addPath(propertyFile);

    //QObject::connect(watcher, SIGNAL(fileChanged(QString)), this,
    //                 SLOT(configFileChanged(QString)));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(60000);

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    setFixedWidth(432);
    setFixedHeight(310);
    widget->setStyleSheet("background-color: #e0e0e0;");

    createSettingsBox();
    widget->show();
}

Ranger::~Ranger()
{
}

void Ranger::showTime()
{
    qDebug() << "TIme: ";
    QDate date = QDate::currentDate();
    QString day = date.toString("ddd M/d/yy");
    QTime time = QTime::currentTime();
    QString tm = time.toString("h:mm A");

    dayLabel->setText(day);
    timeLabel->setText(tm);
}

void Ranger::configFileChanged(QString file)
{
    qDebug() << "Changed: " << file ;
    readProperties(propertyFile);
    outsideLabel->setText(props["outsideTemp"] + " \u00B0  |  " + props["outsideHumidity"] + "%");
    indoorLabel->setText("INDOOR " + props["insideHumidity"] + "%");
}

void Ranger::readProperties(QString path) {
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)) {
        //QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    props.insert("outsideTemp","0");
    props.insert("outsideHumidity","0");
    props.insert("insideHumidity","0");
    props.insert("insideTemp","0");

    while(!in.atEnd()) {
        QString line = in.readLine();
        //qDebug() << "J" << line;
        QStringList fields = line.split(":");
        if (fields.length() != 2) continue;
        props.insert(fields[0], fields[1]);
        //qDebug() << fields[1];
    }

    file.close();
}

void Ranger::createSettingsBox() {
    QGroupBox *box = new QGroupBox(this);
    box->setGeometry(1,1,430,50);
    box->setStyleSheet("QGroupBox {background-color: " + background_color +";border: 0 none;border-radius: 0;}");

    QDate date = QDate::currentDate();
    QString day = date.toString("ddd M/d/yy");
    QTime time = QTime::currentTime();
    QString tm = time.toString("h:mm A");

    dayLabel = addLabel(day, 20,13,100,18, this);
    timeLabel = addLabel(tm, 140,13,100,18, this);
    outsideLabel = addLabel(props["outsideTemp"] + " \u00B0  |  " + props["outsideHumidity"] + "%",
            330,8,100,18, this);
    addLabel("OUTSIDE", 340,28,100,18, this);

    upButton = new QPushButton(this);
    upButton->setGeometry(20,80,74,60);
    upButton->setText("UP");
    upButton->setStyleSheet(styleSheet(40, "normal", "white", background_color));

    dnButton = new QPushButton(this);
    dnButton->setGeometry(20,150,74,60);
    dnButton->setText("DN");
    dnButton->setStyleSheet(styleSheet(40, "normal", "white", background_color));

    QButtonGroup *cg = new QButtonGroup;
    cg->addButton(upButton);
    cg->addButton(dnButton);

    connect(cg , SIGNAL(buttonClicked(QAbstractButton*)),
            this,SLOT(cgButtonClicked(QAbstractButton*)));
    connect(cg , SIGNAL(buttonPressed(QAbstractButton*)),
            this,SLOT(cgButtonPressed(QAbstractButton*)));
    connect(cg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(cgButtonReleased(QAbstractButton*)));

    QFont setFont("Tahoma",70, QFont::Normal, false);
    statusLabel = new QLabel(QString::number(statControl), this);
    statusLabel->setFont(setFont);
    statusLabel->setStyleSheet("color: black;");
    statusLabel->setGeometry(160, 80, 120, 120);

    setupButton = addButton("SETUP", 340, 80, 70, 30);

    QFont hmFont("Tahoma",10, QFont::Normal, false);
    QLabel *hm = new QLabel("HUMIDITY", this);
    hm->setFont(hmFont);
    hm->setStyleSheet("color: black;");
    hm->setGeometry(340, 122, 100, 20);

    indoorLabel = new QLabel("INDOOR " + props["insideHumidity"] + "%", this);
    indoorLabel->setFont(hmFont);
    indoorLabel->setStyleSheet("color: black;");
    indoorLabel->setGeometry(334, 142, 100, 20);

    testButton = addButton("TEST", 346, 175, 60, 30);

    QGroupBox *statBox = new QGroupBox(this);
    statBox->setGeometry(1,230,430,28);
    statBox->setStyleSheet("QGroupBox {background-color: " + background_color +";border: 0 none;border-radius: 0;}");

    modeLabel = addLabel(" AUTO", 20, 3, 100, 20, statBox);
    fanLabel  = addLabel(" AUTO", 102, 3, 100, 20, statBox);
    holdLabel = addLabel("", 195, 3, 100, 20, statBox);
    heatLabel = addLabel("68 \u00B0", 288, 3, 100, 20, statBox);
    coolLabel = addLabel("78 \u00B0", 370, 3, 100, 20, statBox);

    modeButton = addButton("MODE", 10, 270, 70, 26);
    fanButton  = addButton("FAN", 95, 270, 70, 26);
    holdButton = addButton("HOLD", 180, 270, 70, 26);
    heatButton = addButton("HEAT", 265, 270, 70, 26);
    heatButton->setStyleSheet(styleSheet(14, "bold", "firebrick", background_color));
    coolButton = addButton("COOL", 350, 270, 70, 26);
    coolButton->setStyleSheet(styleSheet(14, "bold", "blue", background_color));

    QButtonGroup *bg = new QButtonGroup;
    bg->addButton(modeButton);
    bg->addButton(fanButton);
    bg->addButton(holdButton);
    bg->addButton(heatButton);
    bg->addButton(coolButton);
    bg->addButton(setupButton);
    bg->addButton(testButton);

    connect(bg , SIGNAL(buttonClicked(QAbstractButton*)),
            this,SLOT(buttonClicked(QAbstractButton*)));
    connect(bg , SIGNAL(buttonPressed(QAbstractButton*)),
            this,SLOT(buttonPressed(QAbstractButton*)));
    connect(bg , SIGNAL(buttonReleased(QAbstractButton*)),
            this,SLOT(buttonReleased(QAbstractButton*)));
}

void Ranger::cgButtonClicked(QAbstractButton* button) {
    //qDebug() << "cgButtonClicked" << button->text();
    if (button->text() == "UP") {
        statusLabel->setText(QString::number(++statControl));
    } else if (button->text() == "DN") {
        statusLabel->setText(QString::number(--statControl));
    }
}

void Ranger::cgButtonPressed(QAbstractButton* button) {
    //qDebug() << "cgButtonPressed" << button->text();
    button->setStyleSheet(styleSheet(40, "normal", "white", highlight_color));
}

void Ranger::cgButtonReleased(QAbstractButton* button) {
    //qDebug() << "cgButtonReleased" << button->text();
    button->setStyleSheet(styleSheet(40, "normal", "white", background_color));
}

void Ranger::buttonClicked(QAbstractButton* button)
{
    //qDebug() <<"You clicked " << button->text();
    if (button->text() == "MODE") {
        mode = !mode;
        modeLabel->setText(mode ? "MANUAL" : " AUTO");
    } else if (button->text() == "FAN") {
        fan = !fan;
        fanLabel->setText(fan ? "MANUAL" : " AUTO");
    } else if (button->text() == "HOLD") {
        holdLabel->setText("HOLD");
    } else if (button->text() == "HEAT") {
        heatLabel->setText("HEAT");
    } else if (button->text() == "COOL") {
        coolLabel->setText("COOL");
    } else if (button->text() == "SETUP") {
        qDebug() << "SETUP";
    } else if (button->text() == "TEST") {
        qDebug() << "TEST";
    }
}

void Ranger::buttonPressed(QAbstractButton* button) {
    //qDebug() << "buttonPressed" << button->text();
    if (button->text() == "HEAT") {
        button->setStyleSheet(styleSheet(14, "bold", "firebrick", highlight_color));
    } else if (button->text() == "COOL") {
        button->setStyleSheet(styleSheet(14, "bold", "blue", highlight_color));
    } else {
        button->setStyleSheet(styleSheet(14, "bold", "white", highlight_color));
    }
}

void Ranger::buttonReleased(QAbstractButton* button) {
    //qDebug() << "buttonReleased" << button->text();
    if (button->text() == "HEAT") {
        button->setStyleSheet(styleSheet(14, "bold", "firebrick", background_color));
    } else if (button->text() == "COOL") {
        button->setStyleSheet(styleSheet(14, "bold", "blue", background_color));
    } else {
        button->setStyleSheet(styleSheet(14, "bold", "white", background_color));
    }
}

QPushButton *Ranger::addButton(QString lbl,int x, int y, int w, int h) {
    QPushButton *but = new QPushButton(this);
    but->setGeometry(x, y, w, h);
    but->setText(lbl);
    but->setStyleSheet(styleSheet(14, "bold", "white", background_color));

    return but;
}

QLabel *Ranger::addLabel(QString lbl, int x, int y, int w, int h, QWidget *parent) {
    //QFont boxFont("Times",11, QFont::Bold, false);
    QLabel *tp = new QLabel(lbl, parent);
    tp->setStyleSheet("color: white;font-weight:bold;font-size:14px;");
    //QFontMetrics fm(boxFont);
    tp->setGeometry(x,y,w,h);
    //qDebug() << "F" << fm.width(lbl);

    return tp;
}

QString Ranger::styleSheet(int fs, QString fw, QString fc, QString bc) {
    QString style="padding-bottom: 2px;";
    style += "border-radius:" + QString::number(border_radius) + ";";
    style += "border:" + QString::number(border_width) + "px solid " + border_color + ";";
    style += "font-size:" + QString::number(fs) + "px;";
    style += "font-weight:" + fw + ";";
    style += "color: " + fc + ";";
    style += "background-color: " + bc + ";";

    return style;
}

//int main(int argc, char *argv[])
int rest_gui_entry()
{
    char *argv[] = { "rest_gui", "Linuxfb:/dev/fb0","-geometry", "480x320" };
	int argc = 4;
    QApplication a(argc, argv);
    appPath = a.applicationDirPath();
    qDebug() << appPath;

    Ranger w;
    w.show();

    return a.exec();
}

