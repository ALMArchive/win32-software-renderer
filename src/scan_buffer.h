#ifndef SCAN_BUFFER_H
#define SCAN_BUFFER_H

#include "defines.h"
#include "bitmap.h"
#include "vertex.h"

typedef struct scan_buffer {
	u32 Height;
	u32* ScanBuffer;
} scan_buffer;

scan_buffer ScanBufferCreate(u32 Height);

void ScanBufferDestroy(scan_buffer* ScanBuffer);

void ScanBufferAddScanLine(scan_buffer* ScanBuffer, u32 Y_Cord, u32 X_Min, u32 X_Max);

void ScanBufferFillBitmap(bitmap* Bitmap, scan_buffer* ScanBuffer, u32 Y_Min, u32 Y_Max);

void ScanBufferFillTriangle(bitmap* Bitmap, scan_buffer* ScanBuffer, vertex* V1, vertex* v2, vertex* V3);

void ScanBufferConvertTriangle(scan_buffer* ScanBuffer, vertex* MinYVert, vertex* MidYVert, vertex* MaxYVert, u32 Handedness);

void ScanBufferConvertLine(scan_buffer* ScanBuffer, vertex* MinYVert, vertex* MaxYVert, u32 WhichSide);

#endif
