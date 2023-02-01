#include <common>

in vec3 vertex;
in vec2 uv;

out vec2 v_uv;

void main() {
	v_uv = uv;
	gl_Position = vec4(vertex, 1.);
}
