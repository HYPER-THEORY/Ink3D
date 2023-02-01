#include <common>
#include <cubemap>

#define N    20

uniform samplerCube map;

uniform float lod;
uniform int samples;
uniform float weights[N];
uniform bool latitudinal;
uniform float d_theta;
uniform vec3 pole_axis;

in vec3 v_dir;

layout(location = 0) out vec3 out_color;

vec3 get_sample(float theta, vec3 axis) {
	float cos_theta = cos(theta);
	vec3 dir = v_dir * cos_theta + cross(axis, v_dir) * sin(theta)
		+ axis * dot(axis, v_dir) * (1. - cos_theta);
	return textureLod(map, dir, lod).xyz;
}

void main() {
	vec3 axis = latitudinal ? pole_axis : cross(pole_axis, v_dir);
	if (all(equal(axis, vec3(0.)))) {
		axis = vec3(v_dir.z, 0., -v_dir.x);
	}
	axis = normalize(axis);
	vec3 blur_sum = weights[0] * get_sample(0., axis);
	for (int i = 1; i < N; ++i) {
		if (i >= samples) break;
		float theta = d_theta * i;
		blur_sum += weights[i] * get_sample(-theta, axis);
		blur_sum += weights[i] * get_sample(theta, axis);
	}
	out_color = blur_sum;
}
