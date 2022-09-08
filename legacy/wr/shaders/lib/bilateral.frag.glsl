#include <common>

uniform sampler2D map;

uniform int width;
uniform int height;

void main() {
	
	for (float x = 0.; x < width; ++x) {
		for (float y = 0.; y < height; ++y) {
			texture(map, vec2(x, y));
		}
	}
}
