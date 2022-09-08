#ifndef LIGHTS_GLSL
#define LIGHTS_GLSL

struct PointLight {
	bool visible;
	vec3 position;
	vec3 color;
	float distance;
	float decay;
};

struct SpotLight {
	bool visible;
	bool shadow;
	vec3 position;
	vec3 direction;
	vec3 color;
	float distance;
	float decay;
	float angle;
	float penumbra;
};

struct DirectionalLight {
	bool visible;
	bool shadow;
	vec3 direction;
	vec3 color;
};

struct AmbientLight {
	bool visible;
	vec3 color;
};

struct HemisphereLight {
	bool visible;
	vec3 direction;
	vec3 sky_color;
	vec3 ground_color;
};

#endif
