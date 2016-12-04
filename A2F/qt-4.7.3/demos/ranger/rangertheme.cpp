#include "rangertheme.h"

RangerTheme::RangerTheme()
{
    QString buttonStyle;
    QString buttonStylePressed;
    QString textColor;
    QString borderColor;
    QString borderColorPressed;
    QString headerStyle;
    QString buttonIcon;
    QStringList rgb;
}

void RangerTheme::setButtonStyle(QString s) {
    buttonStyle = s;
}
QString RangerTheme::getButtonStyle() {
    return buttonStyle;
}
void RangerTheme::setButtonStylePressed(QString s) {
    buttonStylePressed = s;
}
QString RangerTheme::getButtonStylePressed() {
    return buttonStylePressed;
}
void RangerTheme::setHeaderStyle(QString s) {
    headerStyle = s;
}
QString RangerTheme::getHeaderStyle() {
    return headerStyle;
}
void RangerTheme::setTextColor(QString s) {
    textColor = s;
}
QString RangerTheme::getTextColor() {
    return textColor;
}
void RangerTheme::setBorderColor(QString s) {
    borderColor = s;
}
QString RangerTheme::getBorderColor() {
    return borderColor;
}
void RangerTheme::setBorderColorPressed(QString s) {
    borderColorPressed = s;
}
QString RangerTheme::getBorderColorPressed() {
    return borderColorPressed;
}
void RangerTheme::setButtonIcon(QString s) {
    buttonIcon = s;
}
QString RangerTheme::getButtonIcon() {
    return buttonIcon;
}
void RangerTheme::setRgb(QStringList s) {
    rgb = s;
}
QStringList RangerTheme::getRgb() {
    return rgb;
}



