#include <Windows.h>
#include "scan_buffer.h"

scan_buffer ScanBufferCreate(u32 Height) {
	scan_buffer ScanBuffer;
	ScanBuffer.Height = Height;

	u32 Size = sizeof(u32) * Height * 2;
	ScanBuffer.ScanBuffer = (u32*)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
	return ScanBuffer;
}

void ScanBufferDestroy(scan_buffer* ScanBuffer) {
	if (ScanBuffer->ScanBuffer) {
		VirtualFree(ScanBuffer->ScanBuffer, 0, MEM_RELEASE);
	}
	ScanBuffer = {};
}

void ScanBufferAddScanLine(scan_buffer *ScanBuffer, u32 Y_Cord, u32 X_Min, u32 X_Max) {
	if (Y_Cord < 0 || Y_Cord >= ScanBuffer->Height) return;
	u32 Y_Pos = Y_Cord * 2;
	ScanBuffer->ScanBuffer[Y_Pos] = X_Min;
	ScanBuffer->ScanBuffer[Y_Pos + 1] = X_Max;
}

void ScanBufferFillBitmap(bitmap *Bitmap, scan_buffer *ScanBuffer, u32 Y_Min, u32 Y_Max) {
	for (i32 J = Y_Min; J < Y_Max; J++) {
		if (J < 0) continue;
		if (J >= ScanBuffer->Height || J >= Bitmap->Height) break;

		u32 Y_Pos = J * 2;
		i32 X_Min = ScanBuffer->ScanBuffer[Y_Pos];
		i32 X_Max = ScanBuffer->ScanBuffer[Y_Pos + 1];

		for (i32 I = X_Min; I < X_Max; I++) {
			BitmapDrawPixel(Bitmap, I, J, 0xFF, 0xFF, 0xFF);
		}
	}
}

void ScanBufferFillTriangle(bitmap* Bitmap, scan_buffer* ScanBuffer, vertex* V1, vertex* V2, vertex* V3) {
	vertex* MinYVert = V1;
	vertex* MidYVert = V2;
	vertex* MaxYVert = V3;

	if (MaxYVert->Y < MidYVert->Y) {
		vertex* Tmp = MaxYVert;
		MaxYVert = MidYVert;
		MidYVert = Tmp;
	}

	if (MidYVert->Y < MinYVert->Y) {
		vertex* Tmp = MidYVert;
		MidYVert = MinYVert;
		MinYVert = Tmp;
	}

	if (MaxYVert->Y < MidYVert->Y) {
		vertex* Tmp = MaxYVert;
		MaxYVert = MidYVert;
		MidYVert = Tmp;
	}

	f32 Area = TriangleArea(MinYVert, MaxYVert, MidYVert);
	u32 Handedness = Area >= 0 ? 1 : 0;

	ScanBufferConvertTriangle(ScanBuffer, MinYVert, MidYVert, MaxYVert, Handedness);
	ScanBufferFillBitmap(Bitmap, ScanBuffer, MinYVert->Y, MaxYVert->Y);
}


void ScanBufferConvertTriangle(scan_buffer* ScanBuffer, vertex* MinYVert, vertex* MidYVert, vertex* MaxYVert, u32 Handedness) {
	ScanBufferConvertLine(ScanBuffer, MinYVert, MaxYVert, 0 + Handedness);
	ScanBufferConvertLine(ScanBuffer, MinYVert, MidYVert, 1 - Handedness);
	ScanBufferConvertLine(ScanBuffer, MidYVert, MaxYVert, 1 - Handedness);
}

void ScanBufferConvertLine(scan_buffer* ScanBuffer, vertex* MinYVert, vertex* MaxYVert, u32 WhichSide) {
	i32 Y_Start = MinYVert->Y;
	if (Y_Start < 0) return;

	i32 Y_End = MaxYVert->Y;
	if (Y_End >= ScanBuffer->Height || Y_End < 0) return;

	i32 X_Start = MinYVert->X;
	if (X_Start < 0) return;

	i32 X_End = MaxYVert->X;
	if (X_End < 0) return;

	i32 Y_Dist = Y_End - Y_Start;
	i32 X_Dist = X_End - X_Start;

	if (Y_Dist <= 0) {
		return;
	}

	f32 X_Step = (f32)X_Dist / (f32)Y_Dist;
	f32 Cur_X = X_Start;

	for (i32 I = Y_Start; I < Y_End; I++) {
		ScanBuffer->ScanBuffer[I * 2 + WhichSide] = (u32)Cur_X;
		Cur_X += X_Step;
	}
}
