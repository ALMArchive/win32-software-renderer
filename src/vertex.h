#ifndef VERTEX_H
#define VERTEX_H

#include "defines.h"

typedef struct vertex {
    f32 X;
    f32 Y;
} vertex;

vertex VertexCreate(f32 X, f32 Y);

f32 TriangleArea(vertex* A, vertex* B, vertex* C);

#endif
