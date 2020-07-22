#include <windows.h>
#include <math.h>
#include "triangle_field.h"

#define FOV 90.0f
internal f32 TanHalfFOV = tan(to_radians(FOV / 2.0f));

triangle_field TriangleFieldCreate(u32 NumStars, f32 Spread, f32 Speed) {
	triangle_field TriangleField = {};
	TriangleField.NumStars = NumStars;
	TriangleField.Spread = Spread;
	TriangleField.Speed = Speed;

	u32 Size = sizeof(f32) * NumStars;

	TriangleField.StarsX = (f32 *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
	TriangleField.StarsY = (f32 *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);
	TriangleField.StarsZ = (f32 *)VirtualAlloc(0, Size, MEM_COMMIT, PAGE_READWRITE);

	return TriangleField;
}

void TriangleFieldDestroy(triangle_field* TriangleField) {
	if (TriangleField->StarsX) {
		VirtualFree(TriangleField->StarsX, 0, MEM_RELEASE);
	}

	if (TriangleField->StarsY) {
		VirtualFree(TriangleField->StarsY, 0, MEM_RELEASE);
	}

	if (TriangleField->StarsZ) {
		VirtualFree(TriangleField->StarsZ, 0, MEM_RELEASE);
	}
	TriangleField = {};
}

void TriangleFieldInitStar(triangle_field* TriangleField, u32 i) {
	if (TriangleField->StarsX && TriangleField->StarsY && TriangleField->StarsZ) {
		TriangleField->StarsX[i] = 2 * (rand_float() - 0.5f)* TriangleField->Spread;
		TriangleField->StarsY[i] = 2 * (rand_float() - 0.5f)* TriangleField->Spread;
		TriangleField->StarsZ[i] = (rand_float() + 0.00001f) * TriangleField->Spread;
	}
}

void TriangleFieldUpdateAndRender(bitmap* Bitmap, scan_buffer* ScanBuffer, triangle_field* TriangleField, f32 delta) {
	BitmapClear(Bitmap, 0x00);
	i32 Width = Bitmap->Width;
	i32 Height = Bitmap->Height;
	f32 HalfWidth = Width / 2.0f;
	f32 HalfHeight = Height / 2.0f;
	u32 triangleBuilderCounter = 0;

	u32 X1 = 0;
	u32 Y1 = 0;
	u32 X2 = 0;
	u32 Y2 = 0;

	for (i32 i = 0; i < TriangleField->NumStars; i++)
	{
		//Update the Star.
		//Move the star towards the camera which is at 0 on Z.
		TriangleField->StarsZ[i] -= delta * TriangleField->Speed;

		//If star is at or behind the camera, generate a new position for
		//it.
		if (TriangleField->StarsZ[i] <= 0)
		{
			TriangleFieldInitStar(TriangleField, i);
		}

		//Render the Star.

		//Multiplying the position by (size/2) and then adding (size/2)
		//remaps the positions from range (-1, 1) to (0, size)
		f32 Z_DIV = TriangleField->StarsZ[i] / TanHalfFOV;
		i32 X = (i32)((TriangleField->StarsX[i] / Z_DIV) * HalfWidth + HalfWidth);
		i32 Y = (i32)((TriangleField->StarsY[i] / Z_DIV) * HalfHeight + HalfHeight);

		//If the star is not within range of the screen, then generate a
		//new position for it.
		if (X < 0 || X >= Width ||
			(Y < 0 || Y >= Height))
		{
			TriangleFieldInitStar(TriangleField, i);
			continue;
		}

		triangleBuilderCounter++;
		if (triangleBuilderCounter == 1)
		{
			X1 = X;
			Y1 = Y;
		}
		else if (triangleBuilderCounter == 2)
		{
			X2 = X;
			Y2 = Y;
		}
		else if (triangleBuilderCounter == 3)
		{
			triangleBuilderCounter = 0;
			vertex V1 = VertexCreate(X1, Y1);
			vertex V2 = VertexCreate(X2, Y2);
			vertex V3 = VertexCreate(X, Y);

			ScanBufferFillTriangle(Bitmap, ScanBuffer, &V1, &V2, &V3);
		}
	}
}
