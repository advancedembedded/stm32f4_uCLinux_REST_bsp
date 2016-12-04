#ifndef ROUNDEDPOLYGON_H
#define ROUNDEDPOLYGON_H

#include <QWidget>
#include <QDebug>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtMath>
#else
#include <QtCore/qmath.h>
#endif

class RoundedPolygon : public QPolygon
{
public:
    RoundedPolygon();
    void SetRadius(unsigned int iRadius);
    const QPainterPath& GetPath();

private:
    QPointF GetLineStart(int i) const;
    QPointF GetLineEnd(int i) const;
    float GetDistance(QPoint pt1, QPoint pt2) const;
    QPainterPath m_path;
    unsigned int m_iRadius;
    float m_uiRadius;
};


#endif // ROUNDEDPOLYGON_H
