#include <common>

#ifdef USE_A
uniform sampler2D map_a;
#endif

#ifdef USE_B
uniform sampler2D map_b;
#endif

#ifdef USE_C
uniform sampler2D map_c;
#endif

#ifdef USE_D
uniform sampler2D map_d;
#endif

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = vec4(1.);
	#ifdef USE_A
		out_color = OP(out_color, textureLod(map_a, v_uv, 0) A_SWIZZLE);
	#endif
	#ifdef USE_B
		out_color = OP(out_color, textureLod(map_b, v_uv, 0) B_SWIZZLE);
	#endif
	#ifdef USE_C
		out_color = OP(out_color, textureLod(map_c, v_uv, 0) C_SWIZZLE);
	#endif
	#ifdef USE_D
		out_color = OP(out_color, textureLod(map_d, v_uv, 0) D_SWIZZLE);
	#endif
	out_color.w = 1;
}
