#ifndef DISPLAYTHEME_H
#define DISPLAYTHEME_H

#include <QString>
#include <QWidget>

class RangerTheme
{
    QString buttonStyle;
    QString buttonStylePressed;
    QString textColor;
    QString borderColor;
    QString borderColorPressed;
    QString headerStyle;
    QString buttonIcon;
    QStringList rgb;

public:
    RangerTheme();
    QString getButtonStyle();
    QString getButtonStylePressed();
    QString getTextColor();
    QString getBorderColor();
    QString getBorderColorPressed();
    QString getHeaderStyle();
    QString getButtonIcon();
    QStringList getRgb();

    void setButtonStyle(QString s);
    void setButtonStylePressed(QString s);
    void setTextColor(QString s);
    void setBorderColor(QString s);
    void setBorderColorPressed(QString s);
    void setHeaderStyle(QString s);
    void setButtonIcon(QString i);
    void setRgb(QStringList r);
};

#endif // DISPLAYTHEME_H
