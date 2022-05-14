#include <common>

uniform mat4 inv_model;
uniform mat4 model_view_proj;

in vec3 vertex;
in vec3 normal;

out vec3 world_normal;

void main() {
	world_normal = inv_transform_dir(normal, inv_model);
	gl_Position = model_view_proj * vec4(vertex, 1.);
}
