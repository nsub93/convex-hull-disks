#include "disc.h"
#include <QtMath>
#include <QPainter>

Disc::Disc()
{

}
Disc::Disc(const QPointF& p1, const QPointF& p2) : _center(p1)
{
    _r = QLineF(p1,p2).length();
    _color = Qt::black;
    _lineWidth = 1;
}

inline qreal Disc::triangleOrientation(const QPointF& A, const QPointF& B, const QPointF& C)
{
  return (B.x()- A.x()) * (C.y() - A.y()) - (C.x()- A.x()) * (B.y() - A.y());
}

bool Disc::operator == (const Disc& other) const
{
    return _r == other._r && _center == other._center;
}

bool Disc::operator != (const Disc& other) const
{
    return !(*this == other);
}

bool Disc::operator > (const Disc& other) const
{
    return this->_r > other._r;
}
bool Disc::operator < (const Disc& other) const
{
    return this->_r < other._r;
}

const QPointF& Disc::center() const
{
    return _center;
}
const qreal&  Disc::r() const
{
    return _r;
}

bool Disc::common_support_line_exists(const Disc* disc) const
{
    qreal x1 = _center.x();
    qreal y1 = _center.y();
    qreal r1 =  _r;
    qreal x2 = disc->_center.x();
    qreal y2 = disc->_center.y();
    qreal r2 = disc->_r;

    qreal d_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    return d_sq > (r1-r2)*(r1-r2);

}

QPointF Disc::leftmost_point() const
{
    return _center - QPointF(_r, 0);
}


QLineF Disc::compute_common_support_line_to(const Disc* disc) const
{
    // implementacija preuzeta sa :
    // https://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Tangents_between_two_circles

    qreal x1 = _center.x();
    qreal y1 = _center.y();
    qreal r1 =  _r;
    qreal x2 = disc->_center.x();
    qreal y2 = disc->_center.y();
    qreal r2 = disc->_r;

    qreal d_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    qreal d = qSqrt(d_sq);
    qreal vx = (x2 - x1) / d;
    qreal vy = (y2 - y1) / d;

    qreal c = (r1 - r2) / d;
    qreal h = qSqrt(qMax(0.0, 1.0 - c*c));

    qreal nx = vx * c - h * vy;
    qreal ny = vy * c + h * vx;

    return QLineF(
                    x1 + r1 * nx,
                    y1 + r1 * ny,
                    x2 + r2 * nx,
                    y2 + r2 * ny
                 );

}

QLineF Disc::compute_parallel_support_line(const QLineF& Lstar) const
{
    //centar za duzinu poluprecnija pomeramo u suprotnom pravcu normale
    //na Lstar i dobijamo P
    QLineF n = Lstar.normalVector().unitVector();
    QPointF move(n.dx(),n.dy());
    QPointF p = _center - _r*move;

    QLineF od = QLineF(p,_center).normalVector();

    return QLineF(od.p2(),od.p1());
    //return od;
}

QRectF Disc::boundingRect() const
{
    return QRectF(_center + QPoint(-_r,_r), _center + QPoint(_r, -_r));
}

void Disc::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen p;
    p.setWidth(_lineWidth);
    p.setColor(_color);
    painter->setPen(p);
    painter->drawEllipse(_center, _r, _r);
}

void Disc::setColor(QColor color)
{
    _color = color;
}
void Disc::setLineWidth(int lineWidth)
{
    _lineWidth = lineWidth;
}


QString Disc::to_qstring()
{
    QString x, y, x1;
    x.setNum(_center.x());
    y.setNum(_center.y());
    x1.setNum(_center.x() + _r);
    return x + " " + y + " " + x1 + " " + y;
}

Disc::~Disc()
{}
