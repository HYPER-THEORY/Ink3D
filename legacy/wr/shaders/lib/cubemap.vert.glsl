#include <common>

uniform mat4 view_proj;

in vec3 vertex;

out vec3 tex_coord;

void main() {
	tex_coord = vertex;
	gl_Position = (view_proj * vec4(vertex, 1.)).xyww;
}
