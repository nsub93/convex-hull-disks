#include "edge.h"
#include <QPainter>

Edge::Edge(const QLineF& line) : _line(line)
{
    _color = Qt::black;
    _width = 1;
}


QRectF Edge::boundingRect() const
{
    return QRectF(_line.p1(), _line.p2());
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen p;
    p.setWidth(_width);
    p.setColor(_color);
    painter->setPen(p);
    painter->drawLine(_line);
}


void Edge::setColor(QColor color)
{
    _color = color;
}
void Edge::setWidth(int w)
{
    _width = w;
}
Edge::~Edge()
{

}

