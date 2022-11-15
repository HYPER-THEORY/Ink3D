#ifndef COMMON_GLSL
#define COMMON_GLSL

#define PI                  3.141592653589793
#define TWO_PI              6.283185307179586
#define HALF_PI             1.5707963267948966
#define INV_PI              0.3183098861837907
#define EPS                 0.000001

#define saturate(a)         clamp(a, 0., 1.)

#define min3(a, b, c)       min(a, min(b, c))
#define max3(a, b, c)       max(a, max(b, c))
#define min4(a, b, c, d)    min(a, min(b, min(c, d)))
#define max4(a, b, c, d)    max(a, max(b, max(c, d)))

/* Returns the squared value of x */
float square(float x)       { return x * x; }
vec2  square(vec2  x)       { return x * x; }
vec3  square(vec3  x)       { return x * x; }
vec4  square(vec4  x)       { return x * x; }

/* Returns the cubed value of x */
float cube(float x)         { return x * x * x; }
vec2  cube(vec2  x)         { return x * x * x; }
vec3  cube(vec3  x)         { return x * x * x; }
vec4  cube(vec4  x)         { return x * x * x; }

/* Returns a pseudo-random value in the range from 0 to 1. */
highp float rand(vec2 s) {
	const highp float a = 12.9898;
	const highp float b = 78.233;
	const highp float c = 43758.5453;
	highp float dt = dot(s, vec2(a, b));
	highp float sn = mod(dt, PI);
	return fract(sin(sn) * c);
}

/* Returns a pseudo-random value in the range from 0 to 1. */
highp float rand(vec3 s) {
	const highp float a = 12.9898;
	const highp float b = 78.233;
	const highp float c = 45.164;
	const highp float d = 43758.5453;
	highp float dt = dot(s, vec3(a, b, c));
	highp float sn = mod(dt, PI);
	return fract(sin(sn) * d);
}

/* Converts color to relative luminance. */
float relative_luminance(vec3 color) {
	const vec3 weights = vec3(0.2126, 0.7152, 0.0722);
	return dot(weights, color);
}

#endif
