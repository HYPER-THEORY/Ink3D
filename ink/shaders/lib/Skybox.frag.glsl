#include <common>
#include <cubemap>
#include <tonemapping>

#ifdef USE_CUBEMAP
uniform samplerCube map;
#endif

#ifdef USE_EQUIRECT
uniform sampler2D map;
#endif

uniform float exposure;
uniform float intensity;

in vec3 v_dir;

layout(location = 0) out vec4 out_color;

void main() {
	#ifdef USE_CUBEMAP
		out_color = textureLod(map, v_dir, 0);
	#endif
	#ifdef USE_EQUIRECT
		vec2 uv = cube_to_equirect(normalize(v_dir));
		out_color = textureLod(map, uv, 0);
	#endif
	out_color.xyz *= intensity;
	out_color.xyz = TONE_MAP(out_color.xyz, exposure);
}
