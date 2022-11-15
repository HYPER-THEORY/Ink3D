#ifndef LIGHTPIPELINE_GLSL
#define LIGHTPIPELINE_GLSL

#include <lights>
#include <fogs>
#include <tonemapping>

uniform vec3 camera_pos;
uniform float exposure;

#if NUM_POINT_LIGHT > 0
uniform PointLight point_lights[NUM_POINT_LIGHT];
#endif

#if NUM_SPOT_LIGHT > 0
uniform SpotLight spot_lights[NUM_SPOT_LIGHT];
#endif

#if NUM_DIRECTIONAL_LIGHT > 0
uniform DirectionalLight directional_lights[NUM_DIRECTIONAL_LIGHT];
#endif

#if NUM_HEMISPHERE_LIGHT > 0
uniform HemisphereLight hemisphere_lights[NUM_HEMISPHERE_LIGHT];
#endif

#ifdef USE_LINEAR_FOG
uniform LinearFog linear_fog;
#endif

#ifdef USE_EXP2_FOG
uniform Exp2Fog exp2_fog;
#endif

vec3 light_process(Material mat, Geometry geom, vec4 light_occlusion) {
	/* initialize light with additional light */
	vec3 direct = light_occlusion.xyz;
	vec3 indirect = vec3(0.);
	
	#if NUM_POINT_LIGHT > 0
		/* apply point lights to material */
		for (int i = 0; i < NUM_POINT_LIGHT; ++i) {
			direct += apply_point_light(mat, geom, point_lights[i]);
		}
	#endif
	
	#if NUM_SPOT_LIGHT > 0
		/* apply spot lights to material */
		for (int i = 0; i < NUM_SPOT_LIGHT; ++i) {
			direct += apply_spot_light(mat, geom, spot_lights[i]);
		}
	#endif
	
	#if NUM_DIRECTIONAL_LIGHT > 0
		/* apply directional lights to material*/
		for (int i = 0; i < NUM_DIRECTIONAL_LIGHT; ++i) {
			direct += apply_directional_light(mat, geom, directional_lights[i]);
		}
	#endif
	
	#if NUM_HEMISPHERE_LIGHT > 0
		/* apply hemisphere lights to material */
		for (int i = 0; i < NUM_HEMISPHERE_LIGHT; ++i) {
			indirect += apply_hemisphere_light(mat, geom, hemisphere_lights[i]);
		}
	#endif
	
	/* calculate direct and indirect light */
	vec3 light = direct + indirect * light_occlusion.w;
	
	#ifdef USE_LINEAR_FOG
		/* apply linear fog on the light */
		apply_linear_fog(linear_fog, distance(camera_pos, geom.position), light);
	#endif
	
	#ifdef USE_EXP2_FOG
		/* apply exp square fog on the light */
		apply_exp2_fog(exp2_fog, distance(camera_pos, geom.position), light);
	#endif
	
	/* return the light after tone mapping */
	return TONE_MAP(light, exposure);
}

#endif
