#ifndef TRANSFORM_GLSL
#define TRANSFORM_GLSL

/* Converts from view Z to depth in perspective camera. */
float z_to_depth_persp(float z, float near, float far) {
	return (2. * near * far / z + near + far) / (far - near) * 0.5 + 0.5;
}

/* Converts from depth to view Z in perspective camera. */
float depth_to_z_persp(float depth, float near, float far) {
	return -2. * near * far / (near + far + (depth * 2. - 1.) * (near - far));
}

/* Converts from view Z to depth in orthographic camera. */
float z_to_depth_ortho(float z, float near, float far) {
	return (2. * z + near + far) / (near - far) * 0.5 + 0.5;
}

/* Converts from depth to view Z in orthographic camera. */
float depth_to_z_ortho(float depth, float near, float far) {
	return -0.5 * (near + far - (depth * 2. - 1.) * (near - far));
}

#endif
