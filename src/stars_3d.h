#ifndef STARS_3D_H
#define STARS_3D_H

#include "defines.h"
#include "utils.h"
#include "bitmap.h"

typedef struct star_field {
    u32 NumStars;
    f32 Spread;
    f32 Speed;
    f32 *StarsX;
    f32 *StarsY;
    f32 *StarsZ;
} star_field;

star_field StarFieldCreate(u32 NumStars, f32 Spread, f32 Speed);

void StarFieldDestroy(star_field* StarField);

void StarFieldInitStar(star_field* StarField, u32 i);

void StarFieldUpdateAndRender(bitmap* Bitmap, star_field* StarField, f32 delta);

#endif
