#include "algorithmview.h"
#include "disc.h"
#include "edge.h"

AlgorithmView::AlgorithmView(QWidget *parent)
    :QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    scene->setSceneRect(-360, -240, 720, 480);
    setScene(scene);

    setMinimumSize(720, 500);
    setRenderHint(QPainter::Antialiasing);

}

void AlgorithmView::drawBackground(QPainter *painter, const QRectF &rect1)
{
    Q_UNUSED(rect1);
    QRectF rect = this->sceneRect();

    painter->drawRect(rect);

    qreal x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    QLinearGradient lg(x1, y1, x2, y2);

    lg.setColorAt(0.0, Qt::gray);
    lg.setColorAt(1, Qt::gray);

    painter->fillRect(rect, lg);
}


QGraphicsScene* AlgorithmView::getScene()
{
    return scene;
}

void AlgorithmView::delayUpdateView()
{
    QTime dieTime= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    scene->update();
}

void AlgorithmView::highlightParts(ConvexHullList* lch, ConvexHullList* rch)
{
    for(int i = 0; i < lch->size() ; i++)
        (*lch)[i]->setColor(Qt::blue);

    for(int i = 0; i < rch->size() ; i++)
        (*rch)[i]->setColor(Qt::red);
}
void AlgorithmView::unhighlightParts(ConvexHullList* lch, ConvexHullList* rch)
{
    for(int i = 0; i < lch->size() ; i++)
        (*lch)[i]->setColor(Qt::black);

    for(int i = 0; i < rch->size() ; i++)
        (*rch)[i]->setColor(Qt::black);
}
void AlgorithmView::highlightDisc(Disc* d)
{
    d->setLineWidth(3);
}
void AlgorithmView::unhighlightDisc(Disc* d)
{
    d->setLineWidth(1);
}

void AlgorithmView::highlightEdge(Edge* e)
{
    e->setWidth(3);
}
void AlgorithmView::unhighlightEdge(Edge* e)
{
    e->setWidth(1);
}

void AlgorithmView::addLstar(Edge* e)
{
    e->setColor(Qt::yellow);
    e->setWidth(2);
    scene->addItem(e);
}
void AlgorithmView::addLp(Edge* e)
{
    e->setColor(Qt::yellow);
    scene->addItem(e);
}
void AlgorithmView::addLq(Edge* e)
{
    e->setColor(Qt::yellow);
    scene->addItem(e);
}

void AlgorithmView::addEgde(Edge* e)
{
    scene->addItem(e);
}
void AlgorithmView::removeEdge(Edge* e)
{
    scene->removeItem(e);
}
void AlgorithmView::removeEdges(ConvexHullEdges* edges)
{
    for (int i = 0; i < edges->size(); ++i)
        scene->removeItem((*edges)[i]);
}


AlgorithmView::~AlgorithmView()
{

}

