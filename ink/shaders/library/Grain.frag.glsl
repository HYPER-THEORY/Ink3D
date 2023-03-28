#include <common>

uniform sampler2D map;

uniform float intensity;
uniform float seed;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	float grain = rand(v_uv * seed);
	out_color = textureLod(map, v_uv, 0);
	out_color.xyz *= 1. + grain * intensity;
}
