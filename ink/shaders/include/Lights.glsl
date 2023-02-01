#ifndef LIGHTS_GLSL
#define LIGHTS_GLSL

#include <common>
#include <brdf>
#include <shadow>

struct Material {
	vec3 color;
	vec3 f0;
	float roughness;
};

struct Geometry {
	vec3 position;
	vec3 view_dir;
	vec3 normal;
};

struct PointLight {
	bool visible;
	vec3 position;
	vec3 color;
	float distance;
	float decay;
};

struct SpotLight {
	bool visible;
	vec3 position;
	vec3 direction;
	vec3 color;
	float distance;
	float decay;
	float angle;
	float penumbra;
	bool cast_shadow;
	Shadow shadow;
};

struct DirectionalLight {
	bool visible;
	vec3 direction;
	vec3 color;
	bool cast_shadow;
	Shadow shadow;
};

struct HemisphereLight {
	bool visible;
	vec3 direction;
	vec3 sky_color;
	vec3 ground_color;
};

/* Calculates the attenuation with light distance. */
float attenuate(float light_distance, float max_distance, float decay) {
	if (max_distance > 0. && decay > 0.) {
		return pow(saturate(1. - light_distance / max_distance), decay);
	}
	return 1.;
}

/* Calculates the attenuation with spot light angle. */
float spot_attenuate(float angle_cos, float max_angle_cos, float penumbra_cos) {
	return smoothstep(max_angle_cos, penumbra_cos, angle_cos);
}

/* Calculates direct light with Lambert BRDF & GGX BRDF. */
vec3 lighting_direct(Material mat, vec3 light_dir, vec3 view_dir, vec3 normal) {
	float nol = saturate(dot(normal, light_dir));
	vec3 specular = brdf_ggx(light_dir, view_dir, normal, mat.f0, mat.roughness);
	vec3 diffuse = brdf_lambert(mat.color);
	return nol * (specular + diffuse);
}

/* Applies the specified point light to material. */
vec3 apply_light(PointLight light, Material mat, Geometry geom) {
	/* check whether the light is visible */
	if (!light.visible) return vec3(0.);
	
	/* calculate the vector form world to light */
	vec3 light_dir = light.position - geom.position;
	float light_distance = length(light_dir);
	light_dir = normalize(light_dir);
	
	/* calculate distance attenuation */
	float attenuation = attenuate(light_distance, light.distance, light.decay);
	if (attenuation <= 0.) return vec3(0.);
	
	/* calculate the color of light */
	vec3 light_color = light.color * attenuation;
	
	/* calculate with lighting model */
	light_color *= lighting_direct(mat, light_dir, geom.view_dir, geom.normal);
	
	/* return the color of light */
	return light_color;
}

/* Applies the specified spot light to material. */
vec3 apply_light(SpotLight light, Material mat, Geometry geom) {
	/* check whether the light is visible */
	if (!light.visible) return vec3(0.);
	
	/* calculate the vector form world to light */
	vec3 light_dir = light.position - geom.position;
	float light_distance = length(light_dir);
	light_dir = normalize(light_dir);
	
	/* calculate spot & distance attenuation */
	float attenuation = 1.;
	float angle_cos = dot(light_dir, light.direction);
	attenuation *= spot_attenuate(angle_cos, light.angle, light.penumbra);
	attenuation *= attenuate(light_distance, light.distance, light.decay);
	if (attenuation <= 0.) return vec3(0.);
	
	/* calculate the color of light */
	vec3 light_color = light.color * attenuation;
	
	/* apply spot light shadow */
	light_color *= light.cast_shadow ?
		get_shadow(light.shadow, geom.position, geom.normal) : 1.;
	
	/* calculate with lighting model */
	light_color *= lighting_direct(mat, light_dir, geom.view_dir, geom.normal);
	
	/* return the color of light */
	return light_color;
}

/* Applies the specified directional light to material. */
vec3 apply_light(DirectionalLight light, Material mat, Geometry geom) {
	/* check whether the light is visible */
	if (!light.visible) return vec3(0.);
	
	/* calculate the color of light */
	vec3 light_color = light.color;
	
	/* apply directional light shadow */
	light_color *= light.cast_shadow ?
		get_shadow(light.shadow, geom.position, geom.normal) : 1.;
	
	/* calculate with lighting model */
	light_color *= lighting_direct(mat, light.direction, geom.view_dir, geom.normal);
	
	/* return the color of light */
	return light_color;
}

/* Applies the specified hemisphere light to material. */
vec3 apply_light(HemisphereLight light, Material mat, Geometry geom) {
	/* check whether the light is visible */
	if (!light.visible) return vec3(0.);
	
	/* calculate the color of light */
	float weight = dot(geom.normal, light.direction) * 0.5 + 0.5;
	vec3 light_color = mix(light.ground_color, light.sky_color, weight);
	
	/* calculate with lighting model */
	light_color *= brdf_lambert(mat.color);
	
	/* return the color of light */
	return light_color;
}

#endif
