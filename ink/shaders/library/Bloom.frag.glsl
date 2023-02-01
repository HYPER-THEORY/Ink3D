#include <common>

uniform sampler2D map;
uniform sampler2D bloom_map;

uniform vec3 tint;
uniform float intensity;
uniform float radius;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	/* calculate bloom color */
	vec3 bloom_color = vec3(0.);
	bloom_color += textureLod(bloom_map, v_uv, 0).xyz * mix(1.0, 0.2, radius);
	bloom_color += textureLod(bloom_map, v_uv, 1).xyz * mix(0.8, 0.4, radius);
	bloom_color += textureLod(bloom_map, v_uv, 2).xyz * 0.6;
	bloom_color += textureLod(bloom_map, v_uv, 3).xyz * mix(0.4, 0.8, radius);
	bloom_color += textureLod(bloom_map, v_uv, 4).xyz * mix(0.2, 1.0, radius);
	
	/* add bloom color to output */
	out_color = textureLod(map, v_uv, 0);
	out_color.xyz += bloom_color * intensity * tint;
}
