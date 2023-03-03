#include <common>

uniform sampler2D map;

uniform float lod;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(map, v_uv, lod);
}
