#include <common>

uniform samplerCube cubemap;

in vec3 tex_coord;

out vec4 out_color;

void main() {
	out_color = texture(cubemap, tex_coord);
}
