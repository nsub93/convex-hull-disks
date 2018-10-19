#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Edge : public QGraphicsItem
{
public:
    Edge(const QLineF&);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    ~Edge();

    void setColor(QColor);
    void setWidth(int);

private:
    QLineF _line;
    QColor _color;
    int _width;
};

#endif // EDGE_H
