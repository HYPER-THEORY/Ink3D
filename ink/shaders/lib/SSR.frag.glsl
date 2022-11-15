#include <common>
#include <packing>
#include <transform>

uniform sampler2D map;
uniform sampler2D z_map;
uniform sampler2D buffer_n;
uniform sampler2D buffer_m;

uniform int max_steps;
uniform int max_level;
uniform float intensity;
uniform float thickness;
uniform float max_distance;
uniform float max_roughness;
uniform vec3 camera_pos;
uniform float camera_near;
uniform float camera_far;
uniform vec2 z_map_size;
uniform mat4 view_proj;
uniform mat4 inv_view_proj;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

bool out_of_screen(vec2 coord) {
	return any(bvec4(coord.x < 0., coord.x > 1., coord.y < 0., coord.y > 1.));
}

void main() {
	/* calculate UV after correction */
	vec2 z_texel_size = 1. / z_map_size;
	vec2 uv = (floor(v_uv * z_map_size) + 0.5) * z_texel_size;
	
	/* sample color from texture */
	vec4 color = textureLod(map, uv, 0);
	out_color = color;
	
	/* sample depth from texture */
	float depth = texelFetch(z_map, ivec2(uv * z_map_size), 0).x;
	
	/* ignore the pixels on skybox */
	if (depth == 1.) return;
	
	/* sample roughness from texture */
	float roughness = textureLod(buffer_m, uv, 0).w;
	if (roughness >= max_roughness) return;
	
	/* sample world normal from texture */
	vec3 normal = textureLod(buffer_n, uv, 0).xyz;
	
	/* normalize normal to avoid artifacts */
	normal = normalize(unpack_normal(normal));
	
	/* transform from screen space to world space */
	vec4 ndc = vec4(vec3(uv.xy, depth) * 2. - 1., 1.);
	vec4 world_pos = inv_view_proj * ndc;
	world_pos /= world_pos.w;
	
	/* view from position to camera position */
	vec3 view_dir = normalize(camera_pos - world_pos.xyz);
	
	/* calculate reflect direction & position */
	vec3 reflect_dir = normalize(reflect(-view_dir, normal));
	vec3 reflect_pos = world_pos.xyz + reflect_dir;
	
	/* calculate UV and depth of reflected point */
	vec4 reflect_ndc = view_proj * vec4(reflect_pos, 1.);
	reflect_ndc /= reflect_ndc.w;
	vec2 reflect_uv = reflect_ndc.xy * 0.5 + 0.5;
	float reflect_depth = reflect_ndc.z * 0.5 + 0.5;
	
	/* if ray tracing towards camera or tracing out of far plane */
	if (any(bvec2(reflect_depth < depth, reflect_depth > 1.))) return;
	
	/* calculate the direction of UV and depth */
	vec2 direction_uv = reflect_uv - uv;
	float direction_depth = reflect_depth - depth;
	
	/* calculate the minimum ray length to step over one texel */
	float min_texel_size = min(z_texel_size.x, z_texel_size.y);
	float ray_length_per_texel = min_texel_size / length(direction_uv);
	float ray_offset = 0.1 * ray_length_per_texel;
	
	/* initialize ray length to prevent intersecting at the start */
	float ray_length = 2.5 * ray_length_per_texel;
	
	/* initialize level, texel per cell and total steps */
	int level = 0;
	int texel_per_cell = 1;
	int total_steps = 0;
	
	/* ray matching until level descends to -1 or total steps exceeds the limit */
	while (level >= 0 && total_steps < 100) {
		
		/* get the UV of the current ray */
		vec2 ray_uv = uv + direction_uv * ray_length;
		if (out_of_screen(ray_uv)) break;
		
		/* get the minimun depth plane where the ray resides */
		vec2 cell_size = z_texel_size * texel_per_cell;
		float min_z = texelFetch(z_map, ivec2(ray_uv / cell_size), level).x;
		
		/* record the ray length before stepping */
		float ray_length_t = ray_length;
		
		/* accumulate the distance of the boundary of next cell */
		vec2 distance_uv = mod(ray_uv, vec2(cell_size));
		distance_uv.x = direction_uv.x >= 0. ? cell_size.x - distance_uv.x : distance_uv.x;
		distance_uv.y = direction_uv.y >= 0. ? cell_size.y - distance_uv.y : distance_uv.y;
		vec2 cross_length = abs(distance_uv / direction_uv);
		ray_length += min(cross_length.x, cross_length.y);
		
		/* get the depth of the current ray */
		float ray_z = depth + direction_depth * ray_length;
		
		/* if ray depth is below the minimum depth plane */
		if (ray_z > min_z) {
			level -= 1; /* descend a level */
			texel_per_cell /= 2; /* update the texel per cell */
			
			/* update ray length to the ray plane intersection */
			ray_length = max(ray_length_t, ray_length - (ray_z - min_z) / direction_depth);
			
			/* intersection has been found if level descends to -1 */
			if (level < 0) {
				
				/* linearize the depth of ray and the minimum depth plane */
				if (ray_z > 1.) return;
				ray_z = depth + direction_depth * ray_length_t;
				float linear_ray_z = linearize_depth_persp(ray_z, camera_near, camera_far);
				float linear_min_z = linearize_depth_persp(min_z, camera_near, camera_far);
				
				/* ignore the pixels if the depth of ray is too large */
				if (linear_ray_z > linear_min_z + thickness) return;
				
				/* calculate attenuation by intensity and roughness */
				float attenuation = intensity * (1. - roughness / max_roughness);
				
				/* calculate screen edge attenuation */
				vec2 coords = smoothstep(0.2, 0.6, abs(vec2(0.5) - ray_uv.xy));
				float screen_edge_fade = clamp(1. - (coords.x + coords.y), 0., 1.);
				attenuation *= screen_edge_fade;
				
				/* calculate reflected color with Fresnel */
				float fresnel = (-dot(view_dir, reflect_dir) + 1.) * 0.5;
				vec4 reflect_color = textureLod(map, ray_uv, 0) * fresnel;
				
				/* calculate final color with reflected color */
				out_color = mix(color, reflect_color, attenuation);
			}
		} else {
			/* enter the next cell by accumulating a small offset */
			ray_length += ray_offset;
			
			if (level < max_level) {
				level += 1; /* ascend a level */
				texel_per_cell *= 2; /* update the texel per cell */
			}
		}
		
		++total_steps; /* accumulate to total steps */
	}
}
