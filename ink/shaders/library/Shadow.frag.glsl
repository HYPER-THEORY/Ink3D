#include <common>

uniform float alpha;
uniform float alpha_test;

#ifdef USE_COLOR_MAP
uniform sampler2D color_map;
#endif

#ifdef USE_ALPHA_MAP
uniform sampler2D alpha_map;
#endif

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	float t_alpha = alpha;
	#ifdef USE_COLOR_MAP
		t_alpha *= texture(color_map, v_uv).w;
	#endif
	#ifdef USE_ALPHA_MAP
		t_alpha *= texture(alpha_map, v_uv).x;
	#endif
	if (t_alpha < alpha_test) discard;
}
