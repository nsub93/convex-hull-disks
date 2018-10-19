#include "convexhullalgorithm.h"
#include <algorithm>

#define MAX_ANGLE 361.0

ConvexHullAlgorithm::ConvexHullAlgorithm(QObject *parent, const QVector<Disc*>& discs)
    :QObject(parent), _discs(discs) {}
ConvexHull::ConvexHull(ConvexHullList& l, ConvexHullEdges& e)
    : list(l), edges(e){}
ConvexHull::ConvexHull(ConvexHullList&& l, ConvexHullEdges&& e)
    : list(l), edges(e){}
ConvexHull::ConvexHull() {}

QLineF ConvexHullAlgorithm::default_Lstar(0.0, 0.0, 0.0, 100.0);
inline qreal ConvexHullAlgorithm::triangleOrientation(const QPointF& A, const QPointF& B, const QPointF& C)
{
  return (B.x()- A.x()) * (C.y() - A.y()) - (C.x()- A.x()) * (B.y() - A.y());
}

bool ConvexHullAlgorithm::dom(const QLineF& Lp, const QLineF& Lq)
{
    qreal to = triangleOrientation(Lp.p1(),Lq.p1(),Lp.p2());

    if( to < 0)
        return false;
    else if ( to > 0 )
       return true;
    // kolinearne tacke
    else
    {
        qreal direction = Lp.p2().y() - Lp.p1().y();
        if(direction > 0.0)
            return Lp.p1().y() > Lq.p2().y() ? true : false;
        else if (direction < 0.0)
            return Lp.p1().y() > Lq.p2().y() ? false : true;
        else // jednake y koordinate
        {
            qreal x_direction = Lp.p2().x() - Lp.p1().x();
            if(x_direction > 0.0)
                return Lp.p1().x() > Lq.p2().x() ? true : false;
            else
                return Lp.p1().x() > Lq.p2().x() ? false : true;
        }
    }
}

Disc* ConvexHullAlgorithm::succ(ConvexHullList& list, int curr_index)
{
    if(++ curr_index < list.length())
        return list[curr_index];
    else if(list.length() > 1)
        return list[1];
    return list.first();
}

inline bool ConvexHullAlgorithm::add(ConvexHullList& list, Disc* disc)
{
    if( list.isEmpty() || *(list.last()) != *disc)
    {
        list.append(disc);
        return true;
    }
    return false;
}

ConvexHull ConvexHullAlgorithm::merge_two_discs(Disc* x, Disc* y)
{
    ConvexHullList x_list = {x};
    ConvexHullList y_list = {y};
    emit highlightParts(&x_list, &y_list);
    emit delayUpdateView();

    if(! x->common_support_line_exists(y))
    {
        emit delayUpdateView();
        emit unhighlightParts(&x_list, &y_list);
        emit delayUpdateView();
        return ( *x > *y) ? ConvexHull(ConvexHullList{x}, ConvexHullEdges{}) :
                          ConvexHull(ConvexHullList{y}, ConvexHullEdges{});

    }

    qreal p1 = x->leftmost_point().x();
    qreal p2 = y->leftmost_point().x();

    Edge *e1 = new Edge(x->compute_common_support_line_to(y));
    Edge *e2 = new Edge(y->compute_common_support_line_to(x));
    emit addEgde(e1);
    emit addEgde(e2);
    emit delayUpdateView();

    emit unhighlightParts(&x_list, &y_list);
    emit delayUpdateView();

    if(p1 < p2)        
    {
        return ConvexHull(ConvexHullList{x, y, x}, ConvexHullEdges{e1, e2});
    }

    else if (p1 > p2)
    {

        return ConvexHull(ConvexHullList{y, x, y}, ConvexHullEdges{e2,e1});
    }
    else
        return (x->center().y() < y->center().y()) ?
                ConvexHull(ConvexHullList{x, y, x}, ConvexHullEdges{e1,e2}) :
                ConvexHull(ConvexHullList{y, x, y}, ConvexHullEdges{e2,e1});
}

ConvexHull ConvexHullAlgorithm::find(int startIndex, int endIndex)
{
    int length = endIndex - startIndex + 1;

    if(length == 1)
        return ConvexHull(ConvexHullList{_discs[startIndex]}, ConvexHullEdges{});

    if(length == 2)
        return merge_two_discs(_discs[startIndex], _discs[endIndex]);

    int s = startIndex + length / 2 - 1;
    ConvexHull P = find(startIndex, s);
    ConvexHull Q = find(s + 1, endIndex);

    if(P.list.length() == 1 && Q.list.length() == 1)
      return merge_two_discs(P.list.first(), Q.list.first());

    return merge(P, Q);
}

ConvexHull ConvexHullAlgorithm::merge(ConvexHull& lch, ConvexHull& rch)
{
    // naglasavamo skupove koje spajamo
    emit highlightParts(&(lch.list), &(rch.list));

    // bisemo sve ivice iz delova koje spajamo
    emit removeEdges(&(lch.edges));
    for(Edge* e : lch.edges)
        delete e;
    emit removeEdges(&(rch.edges));
    for(Edge* e : rch.edges)
        delete e;

    emit delayUpdateView();

    ConvexHullList convexHullList = ConvexHullList{};
    ConvexHullEdges convexHullEdges = ConvexHullEdges{};

    QLineF Lstar = default_Lstar;

    int end_lch = lch.list.length();
    int end_rch = rch.list.length();

    int i = 0, j = 0;
    Disc *p = lch.list.first(), *q = rch.list.first();
    QLineF Lp = p->compute_parallel_support_line(Lstar);
    QLineF Lq = q->compute_parallel_support_line(Lstar);
    Edge *eLstar, *eLp, *eLq;

    // naglasavamo p i q
    emit highlightDisc(p);
    emit highlightDisc(q);
    emit delayUpdateView();

    do
    {
        // iscrtavamo Lstar, Lp i Lq
        eLstar = new Edge(Lstar);
        eLp = new Edge(Lp);
        eLq = new Edge(Lq);

        emit addLstar(eLstar);
        emit addLp(eLp);
        emit addLq(eLq);


        emit delayUpdateView();

        if(dom(Lp,Lq))
        {
            if(add(convexHullList, p) && convexHullList.size() != 1)
            {
                Disc* s = convexHullList[convexHullList.size() - 2];
                Edge *e = new Edge(s->compute_common_support_line_to(p));
                convexHullEdges.append(e);
                emit addEgde(e);
                emit highlightEdge(e);
                emit delayUpdateView();
                emit unhighlightEdge(e);

            }
            advance(Lstar, i, p, j, q, convexHullList, convexHullEdges, lch.list, rch.list);
        }
        else
        {
            if(add(convexHullList, q) && convexHullList.size() != 1)
            {
                Disc* s = convexHullList[convexHullList.size() - 2];
                Edge *e = new Edge(s->compute_common_support_line_to(q));
                convexHullEdges.append(e);
                emit addEgde(e);
                emit highlightEdge(e);
                emit delayUpdateView();
                emit unhighlightEdge(e);
            }
            advance(Lstar, j, q, i, p, convexHullList, convexHullEdges, rch.list, lch.list);
        }

        // brisemo Lstar, Lp i Lq
        emit removeEdge(eLstar);
        emit removeEdge(eLp);
        emit removeEdge(eLq);

        delete eLstar;
        delete eLp;
        delete eLq;

        Lp = p->compute_parallel_support_line(Lstar);
        Lq = q->compute_parallel_support_line(Lstar);

    }
    while (i < end_lch || j < end_rch);


    emit unhighlightDisc(p);
    emit unhighlightDisc(q);
    emit unhighlightParts(&(lch.list), &(rch.list));

    emit delayUpdateView();


    return  ConvexHull(convexHullList, convexHullEdges);
}

void ConvexHullAlgorithm::advance(QLineF& Lstar, int& x_index, Disc*& x, int& y_index, Disc*& y,
                                  ConvexHullList& list, ConvexHullEdges& edges,
                                  ConvexHullList& x_list, ConvexHullList& y_list)
{
    QLineF Lxy, Lyx, Lx_succx, Ly_succy;
    Edge *eLxy = nullptr, *eLyx = nullptr;
    Edge *eLx_succx = nullptr, *eLy_succy = nullptr;
    qreal a1 = MAX_ANGLE, a4 = MAX_ANGLE;
    qreal a2 = MAX_ANGLE, a3 = MAX_ANGLE;

    bool exists_support_line = false;
    bool added_Lxy = false;
    bool added_Lyx = false;

    if(x->common_support_line_exists(y))
    {
        exists_support_line = true;
        Lxy = x->compute_common_support_line_to(y);
        Lyx = y->compute_common_support_line_to(x);
        a1 = Lstar.angleTo(Lxy);
        a4 = Lstar.angleTo(Lyx);

        eLxy = new Edge(Lxy);
        eLyx = new Edge(Lyx);
        emit addEgde(eLxy);
        emit addEgde(eLyx);
    }
    emit delayUpdateView();

    Disc* x_succ = succ(x_list, x_index);
    Disc* y_succ = succ(y_list, y_index);

    if (x_list.length() == 1 && x_index == 0)
        x_index++;
    else if(x_index < x_list.length())
    {
        Lx_succx = x->compute_common_support_line_to(x_succ);
        a2 = Lstar.angleTo(Lx_succx);

        eLx_succx = new Edge(Lx_succx);
        emit addEgde(eLx_succx);
    }

    if(y_list.length() == 1 && y_index == 0)
         y_index++;
    else if(y_index < y_list.length())
    {
        Ly_succy = y->compute_common_support_line_to(y_succ);
        a3 = Lstar.angleTo(Ly_succy);

        eLy_succy = new Edge(Ly_succy);
        emit addEgde(eLy_succy);
    }
    emit delayUpdateView();

    if (a1 == std::min({a1, a2, a3}) && a1 != MAX_ANGLE)
    {
        if(add(list, y))
        {
            added_Lxy = true;
            edges.append(eLxy);
            emit highlightEdge(eLxy);
            emit delayUpdateView();
        }

        if( (a4 == std::min({a4, a2, a3})) && add(list, x))
        {
            added_Lyx = true;
            edges.append(eLyx);
            emit highlightEdge(eLxy);
            emit delayUpdateView();
        }
    }

    if (a2 < a3)
    {
        Lstar = Lx_succx;
        if(x_index < x_list.length())
        {
            emit unhighlightDisc(x);
            x = x_list[++x_index % x_list.length()];
            emit highlightDisc(x);
        }
    }
    else if(a2 > a3 || (a2 == a3 && a2 != MAX_ANGLE))
    {
        Lstar = Ly_succy;
        if (y_index < y_list.length())
        {
            emit unhighlightDisc(y);
            y = y_list[++y_index % y_list.length()];
            emit highlightDisc(y);
        }
    }
    emit delayUpdateView();

    // brisemo linije koje ne ulaze u omotac
    if(added_Lxy)
        emit unhighlightEdge(eLxy);
    else if(exists_support_line)
    {
        emit removeEdge(eLxy);
        delete eLxy;
    }

    if(added_Lyx)
        emit unhighlightEdge(eLyx);
    else if(exists_support_line)
    {
        emit removeEdge(eLyx);
        delete eLyx;
    }

    if(eLx_succx != nullptr)
    {
        emit removeEdge(eLx_succx);
        delete eLx_succx;
    }

    if(eLy_succy != nullptr)
    {
        emit removeEdge(eLy_succy);
        delete eLy_succy;
    }

    emit delayUpdateView();
}

 void ConvexHullAlgorithm::setDiscs(const QVector<Disc*>& discs)
 {
     _discs = discs;
 }

ConvexHull::~ConvexHull()
{
}

ConvexHullAlgorithm::~ConvexHullAlgorithm()
{
}
