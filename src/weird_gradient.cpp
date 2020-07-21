#include "weird_gradient.h"

void RenderWeirdGradient(bitmap* Bitmap, i32 BlueOffset, i32 GreenOffset) {
	i32 Width = Bitmap->Width;
	i32 Height = Bitmap->Height;

	i32 Pitch = Width * BytesPerPixel;
	u8* Row = (u8*)Bitmap->Memory;
	for (i32 Y = 0;
		Y < Height;
		++Y) {
		u32* Pixel = (u32*)Row;
		for (i32 X = 0;
			X < Width;
			++X) {
			u8 Blue = (X + BlueOffset);
			u8 Green = (Y + GreenOffset);
			*Pixel++ = ((Green << 8) | Blue);
		}
		Row += Pitch;
	}
}
