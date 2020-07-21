#include <windows.h>
#include "bitmap.h"


bitmap BitmapCreate(u32 Width, u32 Height) {
	bitmap Bitmap;

	Bitmap.Width = Width;
	Bitmap.Height = Height;
	Bitmap.Size = (Height * Width) * BytesPerPixel;
	Bitmap.Memory = VirtualAlloc(0, Bitmap.Size, MEM_COMMIT, PAGE_READWRITE);

	return Bitmap;
}

void BitmapDestroy(bitmap* Bitmap) {
	if (Bitmap->Memory) {
		VirtualFree(Bitmap->Memory, 0, MEM_RELEASE);
	}
	Bitmap = {};
}

void BitmapClear(bitmap* Bitmap, u8 Shade) {
	if (!Bitmap->Memory) return;
	u32 Width = Bitmap->Width;
	u32 Height = Bitmap->Height;

	i32 Pitch = Width * BytesPerPixel;
	u8* Row = (u8*)Bitmap->Memory;
	for (u32 Y = 0;
		Y < Height;
		++Y) {
		u32* Pixel = (u32*)Row;
		for (i32 X = 0;
			X < Width;
			++X) {
			*Pixel++ = (Shade << 16) | (Shade << 8) | Shade;
		}
		Row += Pitch;
	}
}

void BitmapDrawPixel(bitmap* Bitmap, u32 X, u32 Y, u8 R, u8 G, u8 B) {
	if (!Bitmap->Memory) return;
	u32 Width = Bitmap->Width;
	u32 Height = Bitmap->Height;
	u8* Start = (u8*)Bitmap->Memory;
	u32 Pitch = Width * BytesPerPixel;
	u32 Position = X * BytesPerPixel + (Y * Pitch);
	if (Position > Bitmap->Size) return;
	u32* Pixel = (u32*)(Start + Position);
	*Pixel = (R << 16) | (G << 8) | B;
}


void BitmapFillFunc(bitmap* Bitmap, u32 f(u8, u8, u8)) {
	if (!Bitmap->Memory) return;
	u32 Width = Bitmap->Width;
	u32 Height = Bitmap->Height;

	i32 Pitch = Width * BytesPerPixel;
	u8* Row = (u8*)Bitmap->Memory;
	for (u32 Y = 0;
		Y < Height;
		++Y) {
		u32* Pixel = (u32*)Row;
		for (i32 X = 0;
			X < Width;
			++X) {
			*Pixel++ = f(0, X, Y);
		}
		Row += Pitch;
	}
}
