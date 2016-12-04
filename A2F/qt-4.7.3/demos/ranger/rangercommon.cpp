#include "rangercommon.h"

RangerCommon::RangerCommon(QWidget *parent) : QMainWindow(parent)
{
}

void RangerCommon::initialize() {
    saveShift = NULL;
    shiftOn = false;
    degreeCode = "\u00B0";
    screenSaverCount = 0;
    screenSaverTime = 100;
    prefsPage = 0;
    selectedDay = "M";
    windowSize = 1;
    firstTime = 0;
    screenLocked = false;

    months << "XXX" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun"
           << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";

    offButton = "QPushButton#off {border: 2px solid #ccc;color: white;outline:0;padding: 3px 6px 5px 6px;border-radius: 10px;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #d8c4df, stop: .25 #a3a3a3, stop: .5 #b2b2b2, stop: .75 #cbcbcb,stop: 1 #dfdfdf);};";
    offButton += "QPushButton#off::pressed {border: 2px solid white;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #d8c4df, stop: .25 #a3a3a3, stop: .5 #b2b2b2, stop: .75 #cbcbcb,stop: 1 #dfdfdf);}";

    currentDateTime =  QDateTime::currentDateTime();
    currentDate =  QDate::currentDate();
    currentTime =  QTime::currentTime();

    currentHour = currentTime.hour();
    currentMinute = currentTime.minute();
    currentAmPm = currentHour < 12 ? "AM" : "PM";

    currentDay = currentDate.day();
    currentMonth = currentDate.month();
    currentYear = currentDate.year();

    periodCounter = 0;
    periodList << "WAKE" << "LEAVE" << "RETURN" << "SLEEP";

    QString font = "Arial Black";
    font = "Simplified Arabic";
    font = "Arial";
    font = "Droid Sans";

    labelFont.setFamily(font);
    labelFont.setPointSize(10*windowSize);
    labelFont.setWeight(QFont::DemiBold);
    labelFont.setLetterSpacing(QFont::AbsoluteSpacing,2);

    headerFont.setFamily(font);
    headerFont.setPointSize(17*windowSize);
    headerFont.setWeight(QFont::Normal);
    headerFont.setLetterSpacing(QFont::AbsoluteSpacing,2);
}

void RangerCommon::buildThemes() {
    QStringList greenRgb;
    greenRgb <<"#6c8648"<<"#9ab262";

    QStringList blueRgb;
    blueRgb <<"#14517a"<<"#2d84c1";

    QStringList lightBlueRgb;
    lightBlueRgb <<"#91a4b7"<<"#bdd6ed";

    QStringList pinkRgb;
    pinkRgb <<"#c294b7"<<"#d8c4df";

    QStringList grayRgb;
    grayRgb <<"#d8c4df"<<"#dfdfdf";

    QStringList yellowRgb;
    yellowRgb <<"#cbb485"<<"#fde6a1";

    QStringList redRgb;
    redRgb <<"#802727"<<"#bf4343";

    QStringList grayBarRgb;
    grayBarRgb <<"#838383"<<"#cacaca";

    QString butCommon = "QPushButton {outline:0;padding: 1px;border-radius: 10px;";
    QString butColor = "border: 2px solid green;color:white;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #6c8648,stop: 1 #9ab262);};";

    RangerTheme theme;
    theme.setTextColor("white");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("green");
    theme.setButtonStyle(butCommon + butColor);
    theme.setRgb(greenRgb);
    rangerThemes.insert("green", theme);

    butColor = "border: 2px solid blue;color:#0410b1;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #91a4b7,stop: 1 #bdd6ed);};";
    theme.setTextColor("#0410b1");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("blue");
    theme.setButtonStyle(butCommon + butColor);
    theme.setRgb(lightBlueRgb);
    rangerThemes.insert("lightBlue", theme);

    butColor = "border: 2px solid red;color:white;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #c294b7,stop: 1 #d8c4df);};";
    theme.setTextColor("white");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("firebrick");
    theme.setButtonStyle(butCommon + butColor);
     theme.setRgb(pinkRgb);
    rangerThemes.insert("pink", theme);

    butColor = "border: 2px solid brown;color:#5f2525;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #cbb485,stop: 1 #fde6a1);};";
    theme.setTextColor("#5f2525");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("brown");
    theme.setButtonStyle(butCommon + butColor);
    theme.setRgb(yellowRgb);
    rangerThemes.insert("yellow", theme);

    butColor = "border: 2px solid blue;color:white;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #14517a,stop: 1 #2d84c1);};";
    theme.setTextColor("white");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("black");
    theme.setButtonStyle(butCommon + butColor);
    theme.setRgb(blueRgb);
    rangerThemes.insert("blue", theme);

    butColor = "border: 2px solid brown;color:white;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #d8c4df,stop: 1 #dfdfdf);};";
    theme.setTextColor("white");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("gray");
    theme.setButtonStyle(butCommon + butColor);
    theme.setRgb(grayRgb);
    rangerThemes.insert("gray", theme);

    theme.setTextColor("white");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("red");
    theme.setRgb(redRgb);
    rangerThemes.insert("red", theme);

    butColor = "border: 2px solid #ccc;color:black;background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0 #d8c4df,stop: 1 #dfdfdf);};";
    theme.setTextColor("black");
    theme.setBorderColor("#ccc");
    theme.setBorderColorPressed("black");
    theme.setButtonStyle(butCommon + butColor);
    theme.setRgb(blueRgb);
    rangerThemes.insert("keyboard", theme);
}

void RangerCommon::buildButtonIcons() {
    int y1 = (int)((float)44*windowSize);
    int x2 = (int)((float)25*windowSize);
    int x3 = (int)((float)49*windowSize);
    int y3 = (int)((float)44*windowSize);
    trianglePoly.clear();
    trianglePoly << QPoint(0,y1) << QPoint(x2,0) << QPoint(x3,y3);

    upIcon = getButtonIcon(0., 1., "#dbdada");
    upIconPressed = getButtonIcon(0., 1., rangerThemes[currentScheme].getBorderColorPressed());

    dnIcon = getButtonIcon(180., 1., "#dbdada");
    dnIconPressed = getButtonIcon(180., 1., rangerThemes[currentScheme].getBorderColorPressed());

    upNumIcon = getButtonIcon(0., .5, "#dbdada");
    upNumIconPressed = getButtonIcon(0., .5, rangerThemes[currentScheme].getBorderColorPressed());

    dnNumIcon = getButtonIcon(180., .5, "#dbdada");
    dnNumIconPressed = getButtonIcon(180., .5, rangerThemes[currentScheme].getBorderColorPressed());
}

QCheckBox *RangerCommon::addCheckBox(QString lbl,int x, int y, QWidget *parent) {
    QCheckBox *but = new QCheckBox(lbl, parent);
    but->move(x,y);
    but->setFont(labelFont);

    return but;
}

QPushButton *RangerCommon::addButton(QString lbl,int x, int y, QWidget *parent) {
    QPushButton *but = new QPushButton(lbl, parent);
    but->move(x,y);
    but->setFont(labelFont);

    return but;
}

QPushButton *RangerCommon::addImageButton(QString image,int x, int y, float sc, QWidget *parent) {
    QPixmap pix(":/images/" + image);
    pix = pix.scaledToWidth(pix.rect().width()*sc*windowSize);

    QIcon qi = QIcon(pix);
    QSize qs(pix.rect().width(),pix.rect().height());

    QPushButton *but = new QPushButton(parent);
    but->move(x,y);
    but->setIcon(qi);
    but->setIconSize(qs);
    but->setObjectName("pixmap");

    return but;
}

QPushButton *RangerCommon::addBaseButton(QString lbl,int x, int y, QWidget *parent) {
    QPushButton *but = new QPushButton(lbl, parent);
    but->move(x,y);
    but->setFont(labelFont);
    but->setObjectName("keyboard");

    return but;
}

QPushButton *RangerCommon::addUpButton(int x, int y, QString outline, QWidget *parent) {
    QPushButton *but = getPolyButton(0., 1., outline, parent);
    but->move(x,y);

    return but;
}

QPushButton *RangerCommon::addDownButton(int x, int y, QString outline, QWidget *parent) {
    QPushButton *but = getPolyButton(180., 1., outline, parent);
    but->move(x,y);

    return but;
}

QPushButton *RangerCommon::addUpNumButton(int x, int y, QString outline, QWidget *parent) {
    QPushButton *but = getPolyButton(0, .5, outline, parent);
    but->move(x,y);

    return but;
}

QPushButton *RangerCommon::addDownNumButton(int x, int y, QString outline, QWidget *parent) {
    QPushButton *but = getPolyButton(180., .5, outline, parent);
    but->move(x,y);

    return but;
}

QPushButton *RangerCommon::getPolyButton(float rot, float scale, QString color, QWidget *parent) {
    QPixmap pix = getPixmap(rot, scale, color);
    QIcon icon(pix);
    QPushButton *but = new QPushButton(parent);
    but->setObjectName("upButton");
    but->setIcon(icon);
    but->setIconSize(pix.rect().size());
    but->setFixedSize(pix.rect().size());

    return but;
}

QIcon *RangerCommon::getButtonIcon(float rot, float scale, QString color) {
    QIcon *icon = new QIcon(getPixmap(rot, scale, color));

    return icon;
}

QPixmap RangerCommon::getPixmap(float rot, float scale, QString color) {
    QPixmap pix(50*windowSize, 45*windowSize);
    pix.fill(QColor(Qt::transparent));

    QPainter painter(&pix);
    painter.rotate(rot);
    painter.scale(scale, scale);
    if (rot == 90.) {
        painter.translate(-50*windowSize,-45*windowSize);
    } else if (rot == 180.) {
        painter.translate(-50*windowSize,-45*windowSize);
    } else if (rot == 270. || rot == -90.) {
        painter.translate(-50*windowSize,-45*windowSize);
    }
    QLinearGradient linearGradient(0,0,0,50);
    painter.setRenderHint(QPainter::Antialiasing, true);
    linearGradient.setColorAt(1.0, QColor(rangerThemes[currentScheme].getRgb()[1]));
    linearGradient.setColorAt(0.0, QColor(rangerThemes[currentScheme].getRgb()[0]));

    painter.setPen(QPen(QColor(color), 2*windowSize));
    painter.setBrush(linearGradient);
    painter.drawPath(trianglePoly.GetPath());

    return pix;
}

QLabel *RangerCommon::addHeaderLabel(QString lbl, QWidget *parent) {
    QLabel *tp = new QLabel(lbl, parent);
    tp->setFont(headerFont);
    tp->setObjectName("header");
    tp->setAlignment(Qt::AlignLeft);

    return tp;
}

QLabel *RangerCommon::addHeaderLabel(QString lbl, int x, int y, QWidget *parent) {
    QLabel *tp = new QLabel(lbl, parent);
    tp->setFont(headerFont);
    tp->setObjectName("header");
    tp->move(x,y);

    return tp;
}

QLabel *RangerCommon::addLabel(QString lbl, int x, int y, QWidget *parent) {
    QLabel *tp = new QLabel(lbl, parent);
    tp->setFont(labelFont);
    tp->move(x,y);

    return tp;
}

QLabel *RangerCommon::addLabels(QString lbl, int x, int y, QString s,QWidget *parent) {
    QLabel *tp = new QLabel(lbl, parent);
    tp->setFont(labelFont);
    tp->setStyleSheet(s);
    tp->move(x,y);

    return tp;
}

QLineEdit *RangerCommon::addText(QString val, int x, int y, QString s, QWidget *parent) {
    QFontMetrics fm(labelFont);

    QLineEdit *le = new QLineEdit(parent);
    le->setText(val);
    le->move(x,y);
    le->setFont(labelFont);
    le->setReadOnly(true);
    le->setStyleSheet(s);
    le->setFixedWidth(fm.width(val)*1.15);

    return le;
}

QGroupBox *RangerCommon::backgroundBox() {
    QGroupBox *box = new QGroupBox(this);
    box->setGeometry(windowXpos, windowYpos, windowWidth, windowHeight);
    headerBox = new QGroupBox(box);
    headerBox->setObjectName("header");
    headerBox->setGeometry(windowXpos,windowYpos,windowWidth,50*windowSize);

    QPixmap pix(":images/watermark.png");
    QIcon qi = QIcon(pix);
    QLabel *water = new QLabel(box);
    water->move(windowXpos,windowYpos+100*(windowSize/2));
    water->setFixedSize(pix.width()*(windowSize/2),pix.height()*(windowSize/2));
    water->setPixmap(qi.pixmap(pix.width(),pix.height()));
    water->setScaledContents(true);

    return box;
}

