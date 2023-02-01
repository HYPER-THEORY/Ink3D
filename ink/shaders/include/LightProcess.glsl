#ifndef LIGHTPROCESS_GLSL
#define LIGHTPROCESS_GLSL

#include <lights>
#include <fogs>
#include <tonemapping>

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
uniform LinearFog fog;
#endif

#ifdef USE_EXP2_FOG
uniform Exp2Fog fog;
#endif

uniform float exposure;

vec3 light_process(Material mat, Geometry geom, vec3 light, float occlusion) {
	vec3 direct_light = vec3(0.);
	vec3 indirect_light = light;
	
	/* apply point / spot / directional / hemisphere lights */
	#if NUM_POINT_LIGHT > 0
		for (int i = 0; i < NUM_POINT_LIGHT; ++i) {
			direct_light += apply_light(point_lights[i], mat, geom);
		}
	#endif
	#if NUM_SPOT_LIGHT > 0
		for (int i = 0; i < NUM_SPOT_LIGHT; ++i) {
			direct_light += apply_light(spot_lights[i], mat, geom);
		}
	#endif
	#if NUM_DIRECTIONAL_LIGHT > 0
		for (int i = 0; i < NUM_DIRECTIONAL_LIGHT; ++i) {
			direct_light += apply_light(directional_lights[i], mat, geom);
		}
	#endif
	#if NUM_HEMISPHERE_LIGHT > 0
		for (int i = 0; i < NUM_HEMISPHERE_LIGHT; ++i) {
			indirect_light += apply_light(hemisphere_lights[i], mat, geom);
		}
	#endif
	
	/* calculate total light */
	vec3 total_light = direct_light + indirect_light * occlusion;
	
	/* apply fog and tone mapping on total light */
	#if defined(USE_LINEAR_FOG) || defined(USE_EXP2_FOG)
		apply_fog(fog, distance(camera_pos, geom.position), total_light);
	#endif
	return TONE_MAP(total_light, exposure);
}

#endif
