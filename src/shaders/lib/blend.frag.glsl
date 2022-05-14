#include <common>

uniform sampler2D a_map;
uniform sampler2D b_map;
uniform sampler2D c_map;
uniform sampler2D d_map;

uniform int use_a_map;
uniform int use_b_map;
uniform int use_c_map;
uniform int use_d_map;

uniform int a_is_factor;
uniform int b_is_factor;
uniform int c_is_factor;
uniform int d_is_factor;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(1.);
	if (use_a_map == TRUE) {
		out_color *= a_is_factor == TRUE ?
			textureLod(a_map, tex_coord, 0).xxxx :
			textureLod(a_map, tex_coord, 0);
	}
	if (use_b_map == TRUE) {
		out_color *= b_is_factor == TRUE ?
			textureLod(b_map, tex_coord, 0).xxxx :
			textureLod(b_map, tex_coord, 0);
	}
	if (use_c_map == TRUE) {
		out_color *= c_is_factor == TRUE ?
			textureLod(c_map, tex_coord, 0).xxxx :
			textureLod(c_map, tex_coord, 0);
	}
	if (use_d_map == TRUE) {
		out_color *= d_is_factor == TRUE ?
			textureLod(d_map, tex_coord, 0).xxxx :
			textureLod(d_map, tex_coord, 0);
	}
	out_color.w = 1;
}
