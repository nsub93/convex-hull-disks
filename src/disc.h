#ifndef DISC_H
#define DISC_H

#include <QPointF>
#include <QLineF>
#include <QGraphicsItem>
#include <QString>

class Disc : public QGraphicsItem
{
public:
    Disc();
    Disc(const QPointF&, const QPointF&);
    bool operator == (const Disc& other) const;
    bool operator != (const Disc& other) const;
    bool operator > (const Disc& other) const;
    bool operator < (const Disc& other) const;

    const QPointF& center() const;
    const qreal&  r() const;

    QPointF leftmost_point() const;
    bool common_support_line_exists(const Disc*) const;
    QLineF compute_common_support_line_to(const Disc*) const;
    QLineF compute_parallel_support_line(const QLineF&) const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;

    void setColor(QColor);
    void setLineWidth(int);

    QString to_qstring();


    ~Disc();

private:
    QPointF _center;
    qreal _r;
    QColor _color;
    int _lineWidth;
    qreal triangleOrientation(const QPointF&, const QPointF&, const QPointF&);
};

#endif // DISC_H
