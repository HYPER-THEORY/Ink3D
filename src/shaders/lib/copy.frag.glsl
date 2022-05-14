#include <common>

uniform sampler2D source_map;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

void main() {
	out_color = textureLod(source_map, tex_coord, 0);
}
