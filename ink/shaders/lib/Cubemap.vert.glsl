#include <common>
#include <cubemap>

uniform int face;

in vec3 vertex;
in vec2 uv;

out vec3 v_dir;

void main() {
	v_dir = face_to_cube(uv * 2. - 1., face);
	gl_Position = vec4(vertex, 1.);
}
