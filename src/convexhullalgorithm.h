#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <QObject>
#include <QVector>
#include <QList>
#include <disc.h>
#include <edge.h>

typedef QVector<Disc*> ConvexHullList;
typedef QList<Edge*> ConvexHullEdges;
class ConvexHullAlgorithm;

struct ConvexHull
{
    ConvexHull();
    ConvexHull(ConvexHullList&, ConvexHullEdges&);
    ConvexHull(ConvexHullList&&, ConvexHullEdges&&);
    ~ConvexHull();
    ConvexHullList list;
    ConvexHullEdges edges;
};

class ConvexHullAlgorithm : public QObject
{
    Q_OBJECT

public:
    explicit ConvexHullAlgorithm(QObject *parent = 0, const QVector<Disc*>& = {});
    ConvexHull find(int,int);
    static QLineF default_Lstar;
    ~ConvexHullAlgorithm();

    void setDiscs(const QVector<Disc*>&);

signals:
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

private:
    QVector<Disc*> _discs;
    ConvexHull merge(ConvexHull&, ConvexHull&);
    ConvexHull merge_two_discs(Disc*, Disc*);
    bool dom(const QLineF&, const QLineF&);
    void advance(QLineF&, int&, Disc*&, int&, Disc*&,
                 ConvexHullList&, ConvexHullEdges&,
                 ConvexHullList&, ConvexHullList&);
    Disc* succ(ConvexHullList&, int);
    bool add(ConvexHullList&, Disc*);
    qreal triangleOrientation(const QPointF&, const QPointF&, const QPointF&);
};

#endif // CONVEXHULL_H
