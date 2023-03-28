#include <common>
#include <packing>

uniform sampler2D g_color;       /* G-Buffer base color */
uniform sampler2D g_normal;      /* G-Buffer world normal */
uniform sampler2D g_material;    /* G-Buffer material data */
uniform sampler2D g_light;       /* G-Buffer indirect light */
uniform sampler2D z_map;         /* Z-Buffer */

uniform vec3 camera_pos;
uniform mat4 inv_view_proj;

#include <lightprocess>

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	/* sample base color and ambient from G-Buffer color */
	vec4 diffuse_occlusion = textureLod(g_color, v_uv, 0);
	vec3 diffuse = diffuse_occlusion.xyz;
	float occlusion = diffuse_occlusion.w;
	
	/* sample depth from Z-Buffer */
	float depth = textureLod(z_map, v_uv, 0).x;
	
	if (depth == 1.) {
		out_color = vec4(TONE_MAP(diffuse, exposure), 0);
		return; /* ignore the pixels on skybox */
	}
	
	/* sample world normal from G-Buffer normal */
	vec3 normal = textureLod(g_normal, v_uv, 0).xyz;
	
	/* normalize normal to avoid artifacts */
	normal = normalize(unpack_normal(normal));
	
	/* sample specular and roughness from G-Buffer material */
	vec4 specular_roughness = textureLod(g_material, v_uv, 0);
	vec3 specular = specular_roughness.xyz;
	float roughness = specular_roughness.w;
	
	/* sample indirect light color from G-Buffer light */
	vec3 indirect_light = textureLod(g_light, v_uv, 0).xyz;
	
	/* create new material object */
	Material material;
	material.color = diffuse;
	material.f0 = specular;
	material.roughness = roughness;
	
	/* transform from screen space to world space */
	vec4 ndc = vec4(v_uv.xy, depth, 1.) * 2. - 1.;
	vec4 world_pos = inv_view_proj * ndc;
	world_pos /= world_pos.w;
	
	/* view from position to camera position */
	vec3 view_dir = normalize(camera_pos - world_pos.xyz);
	
	/* create new geometry object */
	Geometry geometry;
	geometry.position = world_pos.xyz;
	geometry.view_dir = view_dir;
	geometry.normal = normal;
	
	/* calculate color with light pipeline */
	out_color = vec4(light_process(material, geometry, indirect_light, occlusion), 1.);
}
