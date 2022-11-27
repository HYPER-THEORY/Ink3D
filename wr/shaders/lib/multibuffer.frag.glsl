#include <common>

uniform sampler2D buffer0;

in vec2 tex_coord;

out vec4 out_color;

void main() {
	out_color = texture(buffer0, tex_coord);
}
