#include <cstdlib>
#include "utils.h"

f32 rand_float() {
    return (f32)rand() / RAND_MAX;
}

f32 to_radians(f32 degrees) {
	return degrees* PI / 180.0f;
}
