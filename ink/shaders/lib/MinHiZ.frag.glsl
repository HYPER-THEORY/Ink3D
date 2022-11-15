#include <common>

uniform sampler2D map;

uniform vec2 screen_size;
uniform int lod;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	vec2 coord = v_uv * screen_size * 2.;
	out_color = min4(texelFetch(map, ivec2(coord + vec2( 0.5,  0.5)), lod),
					 texelFetch(map, ivec2(coord + vec2( 0.5, -0.5)), lod),
					 texelFetch(map, ivec2(coord + vec2(-0.5,  0.5)), lod),
					 texelFetch(map, ivec2(coord + vec2(-0.5, -0.5)), lod));
}
