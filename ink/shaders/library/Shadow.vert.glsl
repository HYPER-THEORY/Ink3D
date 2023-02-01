#include <common>

uniform mat4 model_view_proj;

in vec3 vertex;
in vec2 uv;

out vec2 v_uv;

void main() {
	v_uv = uv;
	gl_Position = model_view_proj * vec4(vertex, 1.);
}
