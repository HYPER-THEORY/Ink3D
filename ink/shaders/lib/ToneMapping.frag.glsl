#include <common>
#include <tonemapping>

#ifdef TONE_MAPPING_LINEAR
#define TONE_MAPPING linear_tone_mapping
#endif

#ifdef TONE_MAPPING_REINHARD
#define TONE_MAPPING reinhard_tone_mapping
#endif

#ifdef TONE_MAPPING_OPTIMIZED
#define TONE_MAPPING optimized_tone_mapping
#endif

#ifdef TONE_MAPPING_ACES_FILMIC
#define TONE_MAPPING aces_filmic_tone_mapping
#endif

uniform sampler2D map;

uniform float exposure;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(map, v_uv, 0);
	out_color.xyz = TONE_MAPPING(out_color.xyz, exposure);
}
