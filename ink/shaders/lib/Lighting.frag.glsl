#include <common>
#include <packing>
#include <lightprocess>

uniform sampler2D buffer_c;    /* G-Buffer base color */
uniform sampler2D buffer_n;    /* G-Buffer world normal */
uniform sampler2D buffer_m;    /* G-Buffer material */
uniform sampler2D buffer_a;    /* G-Buffer additional light */
uniform sampler2D buffer_d;    /* G-Buffer depth */

uniform mat4 inv_view_proj;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	/* sample diffuse color and ambient from buffer C */
	vec4 diffuse_occlusion = textureLod(buffer_c, v_uv, 0);
	vec3 diffuse = diffuse_occlusion.xyz;
	float occlusion = diffuse_occlusion.w;
	
	/* sample depth from buffer D */
	float depth = textureLod(buffer_d, v_uv, 0).x;
	
	/* ignore pixels on skybox */
	out_color = vec4(diffuse, 0.);
	if (depth == 1.) return;
	
	/* sample world normal from buffer N */
	vec3 normal = textureLod(buffer_n, v_uv, 0).xyz;
	
	/* normalize normal to avoid artifacts */
	normal = normalize(unpack_normal(normal));
	
	/* sample specular color and roughness from buffer M */
	vec4 specular_roughness = textureLod(buffer_m, v_uv, 0);
	vec3 specular = specular_roughness.xyz;
	float roughness = specular_roughness.w;
	
	/* sample additional light color from buffer A */
	vec3 additional = textureLod(buffer_a, v_uv, 0).xyz;
	
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
	vec4 light_occlusion = vec4(additional, occlusion);
	out_color = vec4(light_process(material, geometry, light_occlusion), 0);
}
