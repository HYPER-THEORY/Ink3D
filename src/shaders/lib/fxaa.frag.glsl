#include <common>

#define FXAA_PC				1
#define FXAA_GLSL_130		1
#define FXAA_GREEN_AS_LUMA	1

#include <Fxaa3_11>

uniform sampler2D source_map;

uniform vec2 screen_size;

in vec2 tex_coord;

layout(location = 0) out vec4 out_color;

void main() {
	vec2 texel_size = vec2(1.) / screen_size;
	out_color = FxaaPixelShader(
		tex_coord,					/* pos */
		vec4(0.),					/* unused */
		source_map,					/* tex */
		source_map,					/* unused */
		source_map,					/* unused */
		texel_size,					/* fxaaQualityRcpFrame */
		vec4(0.),					/* unused */
		vec4(0.),					/* unused */
		vec4(0.),					/* unused */
		0.75,						/* fxaaQualitySubpix */
		0.166,						/* fxaaQualityEdgeThreshold */
		0.0833,						/* fxaaQualityEdgeThresholdMin */
		0.,							/* unused */
		0.,							/* unused */
		0.,							/* unused */
		vec4(0.)					/* unused */
	);
}
