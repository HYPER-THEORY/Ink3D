#include <common>

uniform vec3 color;

in vec3 world_normal;

layout(location = 0) out vec3 out_color;
layout(location = 1) out vec3 out_normal;

void main() {
	out_color = color;
	out_normal = world_normal;
}
