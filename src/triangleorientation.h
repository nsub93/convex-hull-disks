#ifndef TRIANGLEORIENTATION
#define TRIANGLEORIENTATION

// A, B, C  QPointF*
#define TRIANGLE_ORIENTATION(A,B,C) ((B->x()- A->x()) * (C->y() - A->y()) - (C->x()- A->x()) * (B->y() - A->y()))

#endif // TRIANGLEORIENTATION

