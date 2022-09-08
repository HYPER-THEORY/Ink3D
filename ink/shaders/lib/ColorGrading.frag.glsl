#include <common>
#include <colorgrading>

uniform sampler2D map;

uniform vec3 saturation;
uniform vec3 contrast;
uniform vec3 gamma;
uniform vec3 gain;
uniform vec3 offset;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(map, v_uv, 0);
	out_color.xyz = color_grading(out_color.xyz, saturation, contrast, gamma, gain, offset);
}
