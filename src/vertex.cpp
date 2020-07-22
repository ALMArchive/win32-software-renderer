#include "vertex.h"

vertex VertexCreate(f32 X, f32 Y) {
    vertex Vertex;
    Vertex.X = X;
    Vertex.Y = Y;
    return Vertex;
}

f32 TriangleArea(vertex* A, vertex* B, vertex* C) {
    f32 X1 = B->X - A->X;
    f32 Y1 = B->Y - A->Y;

    f32 X2 = C->X - A->X;
    f32 Y2 = C->Y - A->Y;

    return (X1 * Y2 - X2 * Y1) / 2.0f;
}
