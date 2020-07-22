#ifndef TRIANGLE_FIELD_H
#define TRIANGLE_FIELD_H

#include "defines.h"
#include "utils.h"
#include "bitmap.h"
#include "scan_buffer.h"

typedef struct triangle_field {
    u32 NumStars;
    f32 Spread;
    f32 Speed;
    f32 *StarsX;
    f32 *StarsY;
    f32 *StarsZ;
} triangle_field;

triangle_field TriangleFieldCreate(u32 NumStars, f32 Spread, f32 Speed);

void TriangleFieldDestroy(triangle_field* TriangleField);

void TriangleFieldInitStar(triangle_field* TriangleField, u32 i);

void TriangleFieldUpdateAndRender(bitmap* Bitmap, scan_buffer* ScanBuffer, triangle_field* TriangleField, f32 delta);

#endif
