#ifndef COLORGRADING_GLSL
#define COLORGRADING_GLSL

const mat3 xyz_2_ap1_mat = mat3(
	 1.6410233797, -0.3248032942, -0.2364246952,
	-0.6636628587,  1.6153315917,  0.0167563477,
	 0.0117218943, -0.0082844420,  0.9883948585
);

const mat3 ap1_2_xyz_mat = mat3(
	 0.6624541811,  0.1340042065,  0.1561876870,
	 0.2722287168,  0.6740817658,  0.0536895174,
	-0.0055746495,  0.0040607335,  1.0103391003
);

const mat3 d65_2_d60_mat = mat3(
	 1.01303     ,  0.00610531  , -0.014971    ,
	 0.00769823  ,  0.998165    , -0.00503203  ,
	-0.00284131  ,  0.00468516  ,  0.924507
);

const mat3 d60_2_d65_mat = mat3(
	 0.987224    , -0.00611327  ,  0.0159533   ,
	-0.00759836  ,  1.00186     ,  0.00533002  ,
	 0.00307257  , -0.00509595  ,  1.08168
);

const mat3 srgb_2_xyz_mat = mat3(
	 0.4124564   ,  0.3575761   ,  0.1804375   ,
	 0.2126729   ,  0.7151522   ,  0.0721750   ,
	 0.0193339   ,  0.1191920   ,  0.9503041
);

const mat3 xyz_2_srgb_mat = mat3(
	 3.2409699419, -1.5373831776, -0.4986107603,
	-0.9692436363,  1.8759675015,  0.0415550574,
	 0.0556300797, -0.2039769589,  1.0569715142
);

const mat3 srgb_2_ap1_mat = xyz_2_ap1_mat * d65_2_d60_mat * srgb_2_xyz_mat;
const mat3 ap1_2_srgb_mat = xyz_2_srgb_mat * d60_2_d65_mat * ap1_2_xyz_mat;

vec3 color_grading(vec3 color, vec3 saturation, vec3 contrast, vec3 gamma, vec3 gain, vec3 offset) {
	vec3 working_color = srgb_2_ap1_mat * color;
	float luma = dot(working_color, vec3(0.2722287168, 0.6740817658, 0.0536895174));
	working_color = max(vec3(0.), mix(vec3(luma), working_color, saturation));
	working_color = pow(working_color * 5.55555555, contrast) * 0.18;
	working_color = pow(working_color, 1. / gamma);
	working_color = working_color * gain + offset;
	return ap1_2_srgb_mat * working_color;
}

#endif
