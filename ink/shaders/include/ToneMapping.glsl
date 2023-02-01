#ifndef TONEMAPPING_GLSL
#define TONEMAPPING_GLSL

const mat3 ACES_INPUT = mat3(
	vec3(0.59719, 0.07600, 0.02840),
	vec3(0.35458, 0.90834, 0.13383),
	vec3(0.04823, 0.01566, 0.83777)
);

const mat3 ACES_OUTPUT = mat3(
	vec3( 1.60475, -0.10208, -0.00327),
	vec3(-0.53108,  1.10813, -0.07276),
	vec3(-0.07367, -0.00605,  1.07602)
);

/* Linear tonemapping operator. */
vec3 linear_tone_map(vec3 color, float exposure) {
	return color * exposure;
}

/* Reinhard tonemapping operator. */
vec3 reinhard_tone_map(vec3 color, float exposure) {
	color *= exposure;
	return saturate(color / (vec3(1.) + color));
}

/* Optimized operator of the Haarm-Peter Duiker’s curve. */
vec3 optimized_tone_map(vec3 color, float exposure) {
	color *= exposure;
	color = max(vec3(0.), color - 0.004);
	color = (color * (6.2 * color + 0.5)) / (color * (6.2 * color + 1.7) + 0.06);
	return pow(color, vec3(2.2));
}

/* RTT and ODT fit operator. */
vec3 rtt_and_odt_fit(vec3 v) {
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return a / b;
}

/* ACES filmic tonemapping operator. */
vec3 aces_filmic_tone_map(vec3 color, float exposure) {
	color = ACES_INPUT * (color * exposure);
	color = ACES_OUTPUT * rtt_and_odt_fit(color);
	return saturate(color);
}

#endif
