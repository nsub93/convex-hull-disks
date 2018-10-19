#ifndef ALGORITHMVIEW_H
#define ALGORITHMVIEW_H

#include <QGraphicsView>
#include <convexhullalgorithm.h>
#include <QTime>
#include <QCoreApplication>

class AlgorithmView : public QGraphicsView
{
    Q_OBJECT
public:
    AlgorithmView(QWidget* = 0);
    ~AlgorithmView();

    QGraphicsScene* getScene();

public slots:
    void delayUpdateView();

    void highlightParts(ConvexHullList*, ConvexHullList*);
    void unhighlightParts(ConvexHullList*, ConvexHullList*);
    void highlightDisc(Disc*);
    void unhighlightDisc(Disc*);

    void highlightEdge(Edge*);
    void unhighlightEdge(Edge*);

    void addLstar(Edge*);
    void addLp(Edge*);
    void addLq(Edge*);

    void addEgde(Edge*);
    void removeEdge(Edge*);
    void removeEdges(ConvexHullEdges*);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

private:
    QGraphicsScene* scene;


};

#endif // ALGORITHMVIEW_H
