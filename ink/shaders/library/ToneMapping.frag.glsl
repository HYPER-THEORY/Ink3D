#include <common>
#include <tonemapping>

uniform sampler2D map;

uniform float exposure;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(map, v_uv, 0);
	out_color.xyz = TONE_MAP(out_color.xyz, exposure);
}
