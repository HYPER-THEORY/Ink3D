#include <common>

in vec3 vertex;
in vec2 uv;

out vec2 tex_coord;

void main() {
	tex_coord = uv;
	gl_Position = vec4(vertex, 1.);
}
