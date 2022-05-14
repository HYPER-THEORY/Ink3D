#include <common>

uniform samplerCube cube_map;

in vec3 tex_coord;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(cube_map, tex_coord, 0);
}
