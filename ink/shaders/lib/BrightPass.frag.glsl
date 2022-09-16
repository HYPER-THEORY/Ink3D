#include <common>

uniform sampler2D map;

uniform float threshold;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(map, v_uv, 0);
	float luminance = relative_luminance(out_color.xyz);
	float alpha = smoothstep(threshold, threshold + 1., luminance);
	out_color = mix(vec4(0.), out_color, alpha);
}
