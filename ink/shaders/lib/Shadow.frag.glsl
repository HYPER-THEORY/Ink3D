#include <common>

uniform sampler2D color_map;

uniform float alpha;
uniform float alpha_test;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	float t_alpha = alpha * texture(color_map, v_uv).w;
	if (t_alpha < alpha_test) discard;
}
