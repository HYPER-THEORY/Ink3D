#include <common>

uniform sampler2D map_a;
uniform sampler2D map_b;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = BLEND_OP(textureLod(map_a, v_uv, 0) A_SWIZZLE,
						 textureLod(map_b, v_uv, 0) B_SWIZZLE);
}
