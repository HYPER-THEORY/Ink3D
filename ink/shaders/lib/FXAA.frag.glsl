#include <common>

#define FXAA_PC				1
#define FXAA_GLSL_130		1
#define FXAA_GREEN_AS_LUMA	1
#include <fxaa3_11>

uniform sampler2D map;

uniform vec2 screen_size;

in vec2 v_uv;

layout(location = 0) out vec4 out_color;

void main() {
	vec2 texel_size = vec2(1.) / screen_size;
	out_color = FxaaPixelShader(
		v_uv,                                /* pos */
		vec4(0.),                            /* unused */
		map,                                 /* tex */
		map,                                 /* unused */
		map,                                 /* unused */
		texel_size,                          /* fxaaQualityRcpFrame */
		vec4(0.),                            /* unused */
		vec4(0.),                            /* unused */
		vec4(0.),                            /* unused */
		0.75,                                /* fxaaQualitySubpix */
		0.166,                               /* fxaaQualityEdgeThreshold */
		0.0833,                              /* fxaaQualityEdgeThresholdMin */
		0.,                                  /* unused */
		0.,                                  /* unused */
		0.,                                  /* unused */
		vec4(0.)                             /* unused */
	);
}
