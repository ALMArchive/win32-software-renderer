#ifndef BITMAP_H
#define BITMAP_H

#include "defines.h"

global_variable i32 BytesPerPixel = 4;

typedef struct bitmap {
    u32 Width;
    u32 Height;
    u32 Size;
    void* Memory;
} bitmap;

bitmap BitmapCreate(u32 Width, u32 Height);

void BitmapDestroy(bitmap* Bitmap);

void BitmapClear(bitmap* Bitmap, u8 Shade);

void BitmapDrawPixel(bitmap* Bitmap, u32 X, u32 Y, u8 R, u8 G, u8 B);

void BitmapFillFunc(bitmap* Bitmap, u32 f(u8, u8, u8));

#endif
