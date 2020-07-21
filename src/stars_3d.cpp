#include <windows.h>
#include <math.h>
#include "stars_3d.h"

f32 FOV = 90.0;
f32 TanHalfFOV = tan(to_radians(FOV / 2.0f));

star_field StarFieldCreate(u32 NumStars, f32 Spread, f32 Speed) {
	star_field StarField = {};
	StarField.NumStars = NumStars;
	StarField.Spread = Spread;
	StarField.Speed = Speed;

	u32 Size = sizeof(f32) * NumStars;

	StarField.StarsX = (f32 *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
	StarField.StarsY = (f32 *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
	StarField.StarsZ = (f32 *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);

	return StarField;
}

void StarFieldDestroy(star_field* StarField) {
	if (StarField->StarsX) {
		VirtualFree(StarField->StarsX, 0, MEM_RELEASE);
	}

	if (StarField->StarsY) {
		VirtualFree(StarField->StarsY, 0, MEM_RELEASE);
	}

	if (StarField->StarsZ) {
		VirtualFree(StarField->StarsZ, 0, MEM_RELEASE);
	}
	StarField = {};
}

void StarFieldInitStar(star_field* StarField, u32 i) {
	if (StarField->StarsX && StarField->StarsY && StarField->StarsZ) {
		StarField->StarsX[i] = 2 * (rand_float() - 0.5f)* StarField->Spread;
		StarField->StarsY[i] = 2 * (rand_float() - 0.5f)* StarField->Spread;
		StarField->StarsZ[i] = (rand_float() + 0.00001f) * StarField->Spread;
	}
}

void StarFieldUpdateAndRender(bitmap* Bitmap, star_field* StarField, f32 delta) {
	BitmapClear(Bitmap, 0x00);
	i32 Width = Bitmap->Width;
	i32 Height = Bitmap->Height;
	f32 HalfWidth = Width / 2.0f;
	f32 HalfHeight = Height / 2.0f;
	for (i32 i = 0; i < StarField->NumStars; i++)
	{
		//Update the Star.
		//Move the star towards the camera which is at 0 on Z.
		StarField->StarsZ[i] -= delta * StarField->Speed;

		//If star is at or behind the camera, generate a new position for
		//it.
		if (StarField->StarsZ[i] <= 0)
		{
			StarFieldInitStar(StarField, i);
		}

		//Render the Star.

		//Multiplying the position by (size/2) and then adding (size/2)
		//remaps the positions from range (-1, 1) to (0, size)
		f32 Z_DIV = StarField->StarsZ[i] / TanHalfFOV;
		i32 X = (i32)((StarField->StarsX[i] / Z_DIV) * HalfWidth + HalfWidth);
		i32 Y = (i32)((StarField->StarsY[i] / Z_DIV) * HalfHeight + HalfHeight);

		//If the star is not within range of the screen, then generate a
		//new position for it.
		if (X < 0 || X >= Width ||
			(Y < 0 || Y >= Height))
		{
			StarFieldInitStar(StarField, i);
		}
		else
		{
			//Otherwise, it is safe to draw this star to the screen.
			BitmapDrawPixel(Bitmap, X, Y, 0xFF, 0xFF, 0xFF);
		}
	}
}
