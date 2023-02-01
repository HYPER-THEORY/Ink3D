#include <common>

uniform sampler2D map;

uniform float lod;
uniform vec2 direction;
uniform int radius;

in vec2 v_uv;

layout(location = 0) out TYPE out_color;

void main() {
	TYPE blur_sum = TYPE(0.);
	
	/* sampling color along the direction */
	for (float i = -radius + 1; i < radius; ++i) {
		vec2 offset = direction * i;
		blur_sum += textureLod(map, v_uv + offset, lod) SWIZZLE;
	}
	
	/* output filtered color */
	out_color = blur_sum / (float(radius) * 2. - 1.);
}
