#ifndef RANGER_H
#define RANGER_H

#include <pthread.h>
#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QDebug>

class Ranger : public QMainWindow
{
    Q_OBJECT

public:
    Ranger(QWidget *parent = 0);
    ~Ranger();

    void readProperties(QString);
    void createSettingsBox();
    QString styleSheet(int fs, QString fw, QString fc, QString bc);
    QLabel *addLabel(QString lbl, int x, int y, int w, int h, QWidget *parent);
    QPushButton *addButton(QString lbl, int x, int y, int w, int h);
    bool mode;
    bool fan;
    int statControl;
    QMap<QString, QString> props;
    int border_radius;
    int border_width;
    QString border_color;
    QString background_color;
    QString highlight_color;
    QString propertyFile;

    //QFileSystemWatcher *watcher;
    QGridLayout *mviLayout;

    QLabel *dayLabel;
    QLabel *timeLabel;
    QLabel *outsideLabel;
    QLabel *statusLabel;
    QLabel *indoorLabel;
    QLabel *modeLabel;
    QLabel *fanLabel;
    QLabel *holdLabel;
    QLabel *heatLabel;
    QLabel *coolLabel;

    QPushButton *upButton;
    QPushButton *dnButton;
    QPushButton *setupButton;
    QPushButton *testButton;
    QPushButton *modeButton;
    QPushButton *fanButton;
    QPushButton *holdButton;
    QPushButton *heatButton;
    QPushButton *coolButton;

public slots:
    void buttonPressed(QAbstractButton* button);
    void buttonReleased(QAbstractButton* button);
    void buttonClicked(QAbstractButton* button);

    void cgButtonPressed(QAbstractButton* button);
    void cgButtonReleased(QAbstractButton* button);
    void cgButtonClicked(QAbstractButton* button);
    void configFileChanged(QString);
    void showTime();

signals:

};

#endif // RANGER_H
