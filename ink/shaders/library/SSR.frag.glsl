#include <common>
#include <packing>
#include <transform>

uniform sampler2D map;
uniform sampler2D g_normal;
uniform sampler2D g_material;
uniform sampler2D z_map;

uniform int max_steps;
uniform float intensity;
uniform float thickness;
uniform float max_roughness;
uniform float near;
uniform float far;
uniform vec2 screen_size;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 inv_proj;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return any(bvec4(coord.x < 0., coord.x > 1., coord.y < 0., coord.y > 1.));
}

void main() {
	/* sample color from texture */
	vec3 color = textureLod(map, v_uv, 0).xyz;
	
	/* sample depth from Z-Buffer */
	float depth = textureLod(z_map, v_uv, 0).x;
	
	/* ignore the pixels on skybox */
	out_color = vec4(color, 1.);
	if (depth == 1.) return;
	
	/* sample roughness from G-Buffer material */
	float roughness = textureLod(g_material, v_uv, 0).w;
	if (roughness >= max_roughness) return;
	
	/* sample world normal from G-Buffer normal */
	vec3 normal = textureLod(g_normal, v_uv, 0).xyz;
	normal = normalize(unpack_normal(normal));
	normal = mat3(view) * normal;
	
	/* transform from screen space to view space */
	vec4 ndc = vec4(vec3(v_uv.xy, depth) * 2. - 1., 1.);
	vec4 view_pos = inv_proj * ndc;
	view_pos /= view_pos.w;
	vec3 view_dir = normalize(view_pos.xyz);
	
	/* calculate reflect direction & position */
	vec3 reflect_dir = normalize(reflect(view_dir, normal));
	vec3 reflect_pos = view_pos.xyz + reflect_dir;
	
	/* calculate UV and depth of reflected point */
	vec4 reflect_ndc = proj * vec4(reflect_pos, 1.);
	reflect_ndc /= reflect_ndc.w;
	vec2 reflect_uv = reflect_ndc.xy * 0.5 + 0.5;
	float reflect_depth = reflect_ndc.z * 0.5 + 0.5;
	
	/* if ray is traced towards camera or out of the far plane */
	if (any(bvec2(reflect_depth < depth, reflect_depth > 1.))) return;
	
	/* calculate the direction of UV and Z */
	vec2 direction_uv = reflect_uv - v_uv;
	float direction_depth = reflect_depth - depth;
	
	/* calculate the minimum ray length to step over one texel */
	vec2 texel_size = 1. / screen_size;
	float ray_step = min(texel_size.x, texel_size.y) / length(direction_uv);
	
	/* initialize ray length to prevent intersecting at the beginning */
	float ray_length = 2.5 * ray_step;
	
	/* initialize total steps */
	int total_steps = 0;
	
	/* ray matching until total steps exceeds the limit */
	while (total_steps < max_steps) {
		
		/* get the UV of the current ray */
		vec2 ray_uv = v_uv + direction_uv * ray_length;
		if (out_of_screen(ray_uv)) break;
		
		/* get the Z of the current ray and cell */
		float ray_depth = depth + direction_depth * ray_length;
		float cell_depth = textureLod(z_map, ray_uv, 0).x;
		
		/* if ray has intersected with cell */
		if (ray_depth > cell_depth) {
			
			/* linearize the depth of the current ray and cell */
			if (ray_depth > 1.) return;
			ray_depth = depth + direction_depth * ray_length;
			float ray_z = depth_to_z_persp(ray_depth, near, far);
			float cell_z = depth_to_z_persp(cell_depth, near, far);
			
			/* ignore the pixels if the depth of ray is too large */
			if (-ray_z > -cell_z + thickness) return;
			
			/* calculate intensity attenuation */
			float attenuation = intensity;
			
			/* calculate distance attenuation */
			float max_f = float(max_steps);
			attenuation *= 1. - smoothstep(max_f * 0.5, max_f, float(total_steps));
			
			/* calculate screen edge attenuation */
			vec2 coords = smoothstep(0.2, 0.6, abs(vec2(0.5) - ray_uv.xy));
			float screen_edge_fade = saturate(1. - (coords.x + coords.y));
			attenuation *= screen_edge_fade;
			
			/* calculate reflected color with Fresnel */
			float cos_theta = max(dot(normal, view_dir), 0.);
			vec3 f0 = textureLod(g_material, v_uv, 0).xyz;
			vec3 fresnel = f0 + (1. - f0) * pow(1. - cos_theta, 5.);
			vec3 reflect_color = textureLod(map, ray_uv, 0).xyz * fresnel;
			
			/* calculate final color with reflected color */
			out_color = vec4(mix(color, reflect_color, attenuation), 1.);
			return;
		}
		
		/* step to the next cell */
		ray_length += ray_step;
		
		/* accumulate to total steps */
		++total_steps;
	}
}
