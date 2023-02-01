#include <common>

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 model_view;
uniform mat4 model_view_proj;
uniform mat3 normal_mat;
uniform vec3 camera_pos;

#ifdef USE_DISPLACEMENT_MAP
uniform sampler2D displacement_map;
uniform float displacement_scale;
#endif

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 v_normal;
out vec2 v_uv;
out vec3 v_world_pos;

#ifdef IN_TANGENT_SPACE
in vec4 tangent;
out vec3 v_tangent;
out vec3 v_bitangent;
#endif

#ifdef USE_VERTEX_COLOR
in vec3 color;
out vec3 v_color;
#endif

void main() {
	vec3 t_vertex = vertex;
	vec3 t_normal = normal;
	vec2 t_uv = uv;
	#ifdef IN_TANGENT_SPACE
		vec3 t_tangent = tangent.xyz;
		vec3 t_bitangent;
	#endif
	#ifdef USE_VERTEX_COLOR
		vec3 t_color = color;
	#endif
	
	#ifdef USE_DISPLACEMENT_MAP
		/* reposition the vertex by displacement map */
		t_vertex += t_normal * texture(displacement_map, t_uv).x * displacement_scale;
	#endif
	
	/* transform normal from object space to world space */
	t_normal = normalize(normal_mat * t_normal);
	
	#ifdef IN_TANGENT_SPACE
		/* transform tangent from object space to world space */
		t_tangent = normalize((model * vec4(t_tangent, 0.)).xyz);
		
		/* calculate bitangent with normal and tangent */
		t_bitangent = normalize(cross(t_normal, t_tangent) * tangent.w);
	#endif
	
	/* pass parameters to fragment shader */
	v_normal = t_normal;
	v_uv = t_uv;
	v_world_pos = (model * vec4(t_vertex, 1.)).xyz;
	#ifdef IN_TANGENT_SPACE
		v_tangent = t_tangent;
		v_bitangent = t_bitangent;
	#endif
	#ifdef USE_VERTEX_COLOR
		v_color = color;
	#endif
	gl_Position = model_view_proj * vec4(t_vertex, 1.);
}
