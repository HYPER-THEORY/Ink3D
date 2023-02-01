/**
 * Copyright (C) 2021-2023 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Gpu.h"

#include <array>
#include <iostream>
#include <memory>

#include "../../libs/opengl/glad.h"

#include "../core/Error.h"
#include "../core/File.h"

namespace Ink::Gpu {

constexpr uint32_t GL_COMPARISON_FUNCTIONS[] = {
	GL_NEVER,                                                 /**< FUNC_NEVER */
	GL_LESS,                                                  /**< FUNC_LESS */
	GL_EQUAL,                                                 /**< FUNC_EQUAL */
	GL_LEQUAL,                                                /**< FUNC_LEQUAL */
	GL_GREATER,                                               /**< FUNC_GREATER */
	GL_NOTEQUAL,                                              /**< FUNC_NOTEQUAL */
	GL_GEQUAL,                                                /**< FUNC_GEQUAL */
	GL_ALWAYS,                                                /**< FUNC_ALWAYS */
};

constexpr uint32_t GL_STENCIL_OPERATIONS[] = {
	GL_ZERO,                                                  /**< STENCIL_ZERO */
	GL_KEEP,                                                  /**< STENCIL_KEEP */
	GL_REPLACE,                                               /**< STENCIL_REPLACE */
	GL_INCR,                                                  /**< STENCIL_INCR */
	GL_DECR,                                                  /**< STENCIL_DECR */
	GL_INCR_WRAP,                                             /**< STENCIL_INCR_WRAP */
	GL_DECR_WRAP,                                             /**< STENCIL_DECR_WRAP */
	GL_INVERT,                                                /**< STENCIL_INVERT */
};

constexpr uint32_t GL_BLEND_OPERATIONS[] = {
	GL_FUNC_ADD,                                              /**< BLEND_ADD */
	GL_FUNC_SUBTRACT,                                         /**< BLEND_SUBTRACT */
	GL_FUNC_REVERSE_SUBTRACT,                                 /**< BLEND_REVERSE_SUBTRACT */
	GL_MIN,                                                   /**< BLEND_MIN */
	GL_MAX,                                                   /**< BLEND_MAX */
};

constexpr uint32_t GL_BLEND_FACTORS[] = {
	GL_ZERO,	                                              /**< FACTOR_ZERO */
	GL_ONE,                                                   /**< FACTOR_ONE */
	GL_SRC_COLOR,                                             /**< FACTOR_SRC_COLOR */
	GL_ONE_MINUS_SRC_COLOR,                                   /**< FACTOR_ONE_MINUS_SRC_COLOR */
	GL_DST_COLOR,                                             /**< FACTOR_DST_COLOR */
	GL_ONE_MINUS_DST_COLOR,                                   /**< FACTOR_ONE_MINUS_DST_COLOR */
	GL_SRC_ALPHA,                                             /**< FACTOR_SRC_ALPHA */
	GL_ONE_MINUS_SRC_ALPHA,                                   /**< FACTOR_ONE_MINUS_SRC_ALPHA */
	GL_DST_ALPHA,                                             /**< FACTOR_DST_ALPHA */
	GL_ONE_MINUS_DST_ALPHA,                                   /**< FACTOR_ONE_MINUS_DST_ALPHA */
};

constexpr uint32_t GL_IMAGE_TYPES[] = {
	GL_UNSIGNED_BYTE,                                         /**< IMAGE_UBYTE */
	GL_BYTE,                                                  /**< IMAGE_BYTE */
	GL_UNSIGNED_SHORT,                                        /**< IMAGE_USHORT */
	GL_SHORT,                                                 /**< IMAGE_SHORT */
	GL_UNSIGNED_INT,                                          /**< IMAGE_UINT */
	GL_INT,                                                   /**< IMAGE_INT */
	GL_HALF_FLOAT,                                            /**< IMAGE_HALF_FLOAT */
	GL_FLOAT,                                                 /**< IMAGE_FLOAT */
	GL_UNSIGNED_INT_24_8,                                     /**< IMAGE_UINT_24_8 */
};

constexpr uint32_t GL_IMAGE_FORMATS[] = {
	GL_RED,                                                   /**< Image::channel is 1 */
	GL_RG,                                                    /**< Image::channel is 2 */
	GL_RGB,                                                   /**< Image::channel is 3 */
	GL_RGBA,                                                  /**< Image::channel is 4 */
};

constexpr uint32_t GL_TEXTURE_TYPES[] = {
	GL_TEXTURE_1D,                                            /**< TEXTURE_1D */
	GL_TEXTURE_2D,                                            /**< TEXTURE_2D */
	GL_TEXTURE_3D,                                            /**< TEXTURE_3D */
	GL_TEXTURE_CUBE_MAP,                                      /**< TEXTURE_CUBE */
	GL_TEXTURE_1D_ARRAY,                                      /**< TEXTURE_1D_ARRAY */
	GL_TEXTURE_2D_ARRAY,                                      /**< TEXTURE_2D_ARRAY */
	GL_TEXTURE_CUBE_MAP_ARRAY,                                /**< TEXTURE_CUBE_ARRAY */
};

constexpr std::pair<int32_t, uint32_t> GL_TEXTURE_FORMATS[] = {
	{GL_R8                , GL_RED            },              /**< TEXTURE_R8_UNORM */
	{GL_R8_SNORM          , GL_RED            },              /**< TEXTURE_R8_SNORM */
	{GL_R16               , GL_RED            },              /**< TEXTURE_R16_UNORM */
	{GL_R16_SNORM         , GL_RED            },              /**< TEXTURE_R16_SNORM */
	{GL_RG8               , GL_RG             },              /**< TEXTURE_R8G8_UNORM */
	{GL_RG8_SNORM         , GL_RG             },              /**< TEXTURE_R8G8_SNORM */
	{GL_RG16              , GL_RG             },              /**< TEXTURE_R16G16_UNORM */
	{GL_RG16_SNORM        , GL_RG             },              /**< TEXTURE_R16G16_SNORM */
	{GL_R3_G3_B2          , GL_RGB            },              /**< TEXTURE_R3G3B2_UNORM */
	{GL_RGB4              , GL_RGB            },              /**< TEXTURE_R4G4B4_UNORM */
	{GL_RGB5              , GL_RGB            },              /**< TEXTURE_R5G5B5_UNORM */
	{GL_RGB8              , GL_RGB            },              /**< TEXTURE_R8G8B8_UNORM */
	{GL_RGB8_SNORM        , GL_RGB            },              /**< TEXTURE_R8G8B8_SNORM */
	{GL_RGB10             , GL_RGB            },              /**< TEXTURE_R10G10B10_UNORM */
	{GL_RGB12             , GL_RGB            },              /**< TEXTURE_R12G12B12_UNORM */
	{GL_RGB16_SNORM       , GL_RGB            },              /**< TEXTURE_R16G16B16_UNORM */
	{GL_RGBA2             , GL_RGB            },              /**< TEXTURE_R2G2B2A2_UNORM */
	{GL_RGBA4             , GL_RGB            },              /**< TEXTURE_R4G4B4A4_UNORM */
	{GL_RGB5_A1           , GL_RGBA           },              /**< TEXTURE_R5G5B5A1_UNORM */
	{GL_RGBA8             , GL_RGBA           },              /**< TEXTURE_R8G8B8A8_UNORM */
	{GL_RGBA8_SNORM       , GL_RGBA           },              /**< TEXTURE_R8G8B8A8_SNORM */
	{GL_RGB10_A2          , GL_RGBA           },              /**< TEXTURE_R10G10B10A2_UNORM */
	{GL_RGB10_A2UI        , GL_RGBA           },              /**< TEXTURE_R10G10B10A2_UINT */
	{GL_RGBA12            , GL_RGBA           },              /**< TEXTURE_R12G12B12A12_UNORM */
	{GL_RGBA16            , GL_RGBA           },              /**< TEXTURE_R16G16B16A16_UNORM */
	{GL_SRGB8             , GL_RGB            },              /**< TEXTURE_R8G8B8_SRGB */
	{GL_SRGB8_ALPHA8      , GL_RGBA           },              /**< TEXTURE_R8G8B8A8_SRGB */
	{GL_R16F              , GL_RED            },              /**< TEXTURE_R16_SFLOAT */
	{GL_RG16F             , GL_RG             },              /**< TEXTURE_R16G16_SFLOAT */
	{GL_RGB16F            , GL_RGB            },              /**< TEXTURE_R16G16B16_SFLOAT */
	{GL_RGBA16F           , GL_RGBA           },              /**< TEXTURE_R16G16B16A16_SFLOAT */
	{GL_R32F              , GL_RED            },              /**< TEXTURE_R32_SFLOAT */
	{GL_RG32F             , GL_RG             },              /**< TEXTURE_R32G32_SFLOAT */
	{GL_RGB32F            , GL_RGB            },              /**< TEXTURE_R32G32B32_SFLOAT */
	{GL_RGBA32F           , GL_RGBA           },              /**< TEXTURE_R32G32B32A32_SFLOAT */
	{GL_R11F_G11F_B10F    , GL_RGB            },              /**< TEXTURE_R11G11B10_SFLOAT */
	{GL_RGB9_E5           , GL_RGB            },              /**< TEXTURE_R9G9B9E5_UNORM */
	{GL_R8I               , GL_RED            },              /**< TEXTURE_R8_SINT */
	{GL_R8UI              , GL_RED            },              /**< TEXTURE_R8_UINT */
	{GL_R16I              , GL_RED            },              /**< TEXTURE_R16_SINT */
	{GL_R16UI             , GL_RED            },              /**< TEXTURE_R16_UINT */
	{GL_R32I              , GL_RED            },              /**< TEXTURE_R32_SINT */
	{GL_R32UI             , GL_RED            },              /**< TEXTURE_R32_UINT */
	{GL_RG8I              , GL_RG             },              /**< TEXTURE_R8G8_SINT */
	{GL_RG8UI             , GL_RG             },              /**< TEXTURE_R8G8_UINT */
	{GL_RG16I             , GL_RG             },              /**< TEXTURE_R16G16_SINT */
	{GL_RG16UI            , GL_RG             },              /**< TEXTURE_R16G16_UINT */
	{GL_RG32I             , GL_RG             },              /**< TEXTURE_R32G32_SINT */
	{GL_RG32UI            , GL_RG             },              /**< TEXTURE_R32G32_UINT */
	{GL_RGB8I             , GL_RGB            },              /**< TEXTURE_R8G8B8_SINT */
	{GL_RGB8UI            , GL_RGB            },              /**< TEXTURE_R8G8B8_UINT */
	{GL_RGB16I            , GL_RGB            },              /**< TEXTURE_R16G16B16_SINT */
	{GL_RGB16UI           , GL_RGB            },              /**< TEXTURE_R16G16B16_UINT */
	{GL_RGB32I            , GL_RGB            },              /**< TEXTURE_R32G32B32_SINT */
	{GL_RGB32UI           , GL_RGB            },              /**< TEXTURE_R32G32B32_UINT */
	{GL_RGBA8I            , GL_RGBA           },              /**< TEXTURE_R8G8B8A8_SINT */
	{GL_RGBA8UI           , GL_RGBA           },              /**< TEXTURE_R8G8B8A8_UINT */
	{GL_RGBA16I           , GL_RGBA           },              /**< TEXTURE_R16G16B16A16_SINT */
	{GL_RGBA16UI          , GL_RGBA           },              /**< TEXTURE_R16G16B16A16_UINT */
	{GL_RGBA32I           , GL_RGBA           },              /**< TEXTURE_R32G32B32A32_SINT */
	{GL_RGBA32UI          , GL_RGBA           },              /**< TEXTURE_R32G32B32A32_UINT */
	{GL_DEPTH_COMPONENT16 , GL_DEPTH_COMPONENT},              /**< TEXTURE_D16_UNORM */
	{GL_DEPTH_COMPONENT24 , GL_DEPTH_COMPONENT},              /**< TEXTURE_D24_UNORM */
	{GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT},              /**< TEXTURE_D32_SFLOAT */
	{GL_DEPTH24_STENCIL8  , GL_DEPTH_STENCIL  },              /**< TEXTURE_D24_UNORM_S8_UINT */
	{GL_DEPTH32F_STENCIL8 , GL_DEPTH_STENCIL  },              /**< TEXTURE_D32_SFLOAT_S8_UINT */
};

constexpr int32_t GL_TEXTURE_WRAPPINGS[] = {
	GL_REPEAT,                                                /**< TEXTURE_REPEAT */
	GL_MIRRORED_REPEAT,                                       /**< TEXTURE_MIRRORED_REPEAT */
	GL_CLAMP_TO_EDGE,                                         /**< TEXTURE_CLAMP_TO_EDGE */
	GL_CLAMP_TO_BORDER,                                       /**< TEXTURE_CLAMP_TO_BORDER */
};

constexpr int32_t GL_TEXTURE_FILTERS[] = {
	GL_NEAREST,                                               /**< TEXTURE_NEAREST */
	GL_LINEAR,                                                /**< TEXTURE_LINEAR */
	GL_NEAREST_MIPMAP_NEAREST,                                /**< TEXTURE_NEAREST_MIPMAP_NEAREST */
	GL_LINEAR_MIPMAP_NEAREST,                                 /**< TEXTURE_LINEAR_MIPMAP_NEAREST */
	GL_NEAREST_MIPMAP_LINEAR,                                 /**< TEXTURE_NEAREST_MIPMAP_LINEAR */
	GL_LINEAR_MIPMAP_LINEAR,                                  /**< TEXTURE_LINEAR_MIPMAP_LINEAR */
};

constexpr uint32_t GL_COLOR_ATTACHMENTS[] = {
	GL_COLOR_ATTACHMENT0,                                     /**< render number is 1 */
	GL_COLOR_ATTACHMENT1,                                     /**< render number is 2 */
	GL_COLOR_ATTACHMENT2,                                     /**< render number is 3 */
	GL_COLOR_ATTACHMENT3,                                     /**< render number is 4 */
	GL_COLOR_ATTACHMENT4,                                     /**< render number is 5 */
	GL_COLOR_ATTACHMENT5,                                     /**< render number is 6 */
	GL_COLOR_ATTACHMENT6,                                     /**< render number is 7 */
	GL_COLOR_ATTACHMENT7,                                     /**< render number is 8 */
	GL_COLOR_ATTACHMENT8,                                     /**< render number is 9 */
	GL_COLOR_ATTACHMENT9,                                     /**< render number is 10 */
	GL_COLOR_ATTACHMENT10,                                    /**< render number is 11 */
	GL_COLOR_ATTACHMENT11,                                    /**< render number is 12 */
	GL_COLOR_ATTACHMENT12,                                    /**< render number is 13 */
	GL_COLOR_ATTACHMENT13,                                    /**< render number is 14 */
	GL_COLOR_ATTACHMENT14,                                    /**< render number is 15 */
	GL_COLOR_ATTACHMENT15,                                    /**< render number is 16 */
	GL_COLOR_ATTACHMENT16,                                    /**< render number is 17 */
	GL_COLOR_ATTACHMENT17,                                    /**< render number is 18 */
	GL_COLOR_ATTACHMENT18,                                    /**< render number is 19 */
	GL_COLOR_ATTACHMENT19,                                    /**< render number is 20 */
	GL_COLOR_ATTACHMENT20,                                    /**< render number is 21 */
	GL_COLOR_ATTACHMENT21,                                    /**< render number is 22 */
	GL_COLOR_ATTACHMENT22,                                    /**< render number is 23 */
	GL_COLOR_ATTACHMENT23,                                    /**< render number is 24 */
	GL_COLOR_ATTACHMENT24,                                    /**< render number is 25 */
	GL_COLOR_ATTACHMENT25,                                    /**< render number is 26 */
	GL_COLOR_ATTACHMENT26,                                    /**< render number is 27 */
	GL_COLOR_ATTACHMENT27,                                    /**< render number is 28 */
	GL_COLOR_ATTACHMENT28,                                    /**< render number is 29 */
	GL_COLOR_ATTACHMENT29,                                    /**< render number is 30 */
	GL_COLOR_ATTACHMENT30,                                    /**< render number is 31 */
	GL_COLOR_ATTACHMENT31,                                    /**< render number is 32 */
};

constexpr int TEXTURE_DEFAULT_FORMATS[][2] = {
	{TEXTURE_R8_UNORM      , TEXTURE_R16_SFLOAT         },
	{TEXTURE_R8G8_UNORM    , TEXTURE_R16G16_SFLOAT      },
	{TEXTURE_R8G8B8_UNORM  , TEXTURE_R16G16B16_SFLOAT   },
	{TEXTURE_R8G8B8A8_UNORM, TEXTURE_R16G16B16A16_SFLOAT},
};

int get_comparison_functions(uint32_t v) {
	if (v == GL_NEVER   ) return FUNC_NEVER;
	if (v == GL_LESS    ) return FUNC_LESS;
	if (v == GL_EQUAL   ) return FUNC_EQUAL;
	if (v == GL_LEQUAL  ) return FUNC_LEQUAL;
	if (v == GL_GREATER ) return FUNC_GREATER;
	if (v == GL_NOTEQUAL) return FUNC_NOTEQUAL;
	if (v == GL_GEQUAL  ) return FUNC_GEQUAL;
	/*   ... GL_ALWAYS */ return FUNC_ALWAYS;
}

int get_stencil_operations(uint32_t v) {
	if (v == GL_ZERO     ) return STENCIL_ZERO;
	if (v == GL_KEEP     ) return STENCIL_KEEP;
	if (v == GL_REPLACE  ) return STENCIL_REPLACE;
	if (v == GL_INCR     ) return STENCIL_INCR;
	if (v == GL_DECR     ) return STENCIL_DECR;
	if (v == GL_INCR_WRAP) return STENCIL_INCR_WRAP;
	if (v == GL_DECR_WRAP) return STENCIL_DECR_WRAP;
	/*   ... GL_ZERO    */ return STENCIL_INVERT;
}

int get_blend_equations(uint32_t v) {
	if (v == GL_FUNC_ADD             ) return BLEND_ADD;
	if (v == GL_FUNC_SUBTRACT        ) return BLEND_SUBTRACT;
	if (v == GL_FUNC_REVERSE_SUBTRACT) return BLEND_REVERSE_SUBTRACT;
	if (v == GL_MIN                  ) return BLEND_MIN;
	/*   ... GL_MAX                 */ return BLEND_MAX;
}

int get_blend_functions(uint32_t v) {
	if (v == GL_ZERO               ) return FACTOR_ZERO;
	if (v == GL_ONE                ) return FACTOR_ONE;
	if (v == GL_SRC_COLOR          ) return FACTOR_SRC_COLOR;
	if (v == GL_ONE_MINUS_SRC_COLOR) return FACTOR_ONE_MINUS_SRC_COLOR;
	if (v == GL_DST_COLOR          ) return FACTOR_DST_COLOR;
	if (v == GL_ONE_MINUS_DST_COLOR) return FACTOR_ONE_MINUS_DST_COLOR;
	if (v == GL_SRC_ALPHA          ) return FACTOR_SRC_ALPHA;
	if (v == GL_ONE_MINUS_SRC_ALPHA) return FACTOR_ONE_MINUS_SRC_ALPHA;
	if (v == GL_DST_ALPHA          ) return FACTOR_DST_ALPHA;
	/* ... GL_ONE_MINUS_DST_ALPHA */ return FACTOR_ONE_MINUS_DST_ALPHA;
}

Rect::Rect(int w, int h) : width(w), height(h) {}

Rect::Rect(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}

std::string State::get_device_info() {
	std::string info = "Vender: ";
	const uint8_t* vender = glGetString(GL_VENDOR);
	info += reinterpret_cast<const char*>(vender);
	info += "\nRenderer: ";
	const uint8_t* renderer = glGetString(GL_RENDERER);
	info += reinterpret_cast<const char*>(renderer);
	info += "\nVersion: ";
	const uint8_t* version = glGetString(GL_VERSION);
	info += reinterpret_cast<const char*>(version);
	info += "\nGLSL Version: ";
	const uint8_t* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
	info += reinterpret_cast<const char*>(glsl);
	return info + "\n";
}

void State::finish() {
	glFinish();
}

void State::flush() {
	glFlush();
}

std::string State::get_error() {
	std::string info;
	uint32_t error = glGetError();
	if (error == GL_NO_ERROR) return "";
	while (error != GL_NO_ERROR) {
		info += "OpenGL: " + std::to_string(error) + ": ";
		if (error == GL_INVALID_ENUM) {
			info += "An unacceptable value is specified for an enumerated argument.\n";
		} else if (error == GL_INVALID_VALUE) {
			info += "A numeric argument is out of range.\n";
		} else if (error == GL_INVALID_OPERATION) {
			info += "The specified operation is not allowed in the current state.\n";
		} else if (error == GL_OUT_OF_MEMORY) {
			info += "There is not enough memory left to execute the command.\n";
		} else {
			info += "An unknown error has occurred.\n";
		}
		error = glGetError();
	}
	return info;
}

void State::clear(bool c, bool d, bool s) {
	GLbitfield flag = 0;
	flag |= GL_COLOR_BUFFER_BIT * c;
	flag |= GL_DEPTH_BUFFER_BIT * d;
	flag |= GL_STENCIL_BUFFER_BIT * s;
	glClear(flag);
}

Vec4 State::get_clear_color() {
	Vec4 clear_color;
	glGetFloatv(GL_COLOR_CLEAR_VALUE, &clear_color.x);
	return clear_color;
}

void State::set_clear_color(const Vec3& c, float a) {
	glClearColor(c.x, c.y, c.z, a);
}

void State::set_clear_color(const Vec4& c) {
	glClearColor(c.x, c.y, c.z, c.w);
}

Vec4 State::get_color_writemask() {
	int color_writemasks[4];
	glGetIntegerv(GL_COLOR_WRITEMASK, &color_writemasks[0]);
	return Vec4(color_writemasks[0], color_writemasks[1],
				color_writemasks[2], color_writemasks[3]);
}

void State::set_color_writemask(bool r, bool g, bool b, bool a) {
	glColorMask(r, g, b, a);
}

void State::enable_depth_test() {
	glEnable(GL_DEPTH_TEST);
}

void State::disable_depth_test() {
	glDisable(GL_DEPTH_TEST);
}

double State::get_clear_depth() {
	double clear_depth = 0;
	glGetDoublev(GL_DEPTH_CLEAR_VALUE, &clear_depth);
	return clear_depth;
}

void State::set_clear_depth(double d) {
	glClearDepth(d);
}

bool State::get_depth_writemask() {
	int depth_writemask = 0;
	glGetIntegerv(GL_DEPTH_WRITEMASK, &depth_writemask);
	return depth_writemask;
}

void State::set_depth_writemask(bool m) {
	glDepthMask(m);
}

int State::get_depth_func() {
	int depth_func = 0;
	glGetIntegerv(GL_DEPTH_FUNC, &depth_func);
	return get_comparison_functions(depth_func);
}

void State::set_depth_func(int f) {
	glDepthFunc(GL_COMPARISON_FUNCTIONS[f]);
}

void State::enable_stencil_test() {
	glEnable(GL_STENCIL_TEST);
}

void State::disable_stencil_test() {
	glDisable(GL_STENCIL_TEST);
}

int State::get_clear_stencil() {
	int clear_stencil = 0;
	glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &clear_stencil);
	return clear_stencil;
}

void State::set_clear_stencil(int s) {
	glClearStencil(s);
}

unsigned int State::get_stencil_writemask() {
	int stencil_writemask = 0;
	glGetIntegerv(GL_STENCIL_WRITEMASK, &stencil_writemask);
	return stencil_writemask;
}

void State::set_stencil_writemask(unsigned int m) {
	glStencilMask(m);
}

int State::get_stencil_func() {
	int stencil_func = 0;
	glGetIntegerv(GL_STENCIL_FUNC, &stencil_func);
	return get_comparison_functions(stencil_func);
}

int State::get_stencil_ref() {
	int stencil_ref = 0;
	glGetIntegerv(GL_STENCIL_REF, &stencil_ref);
	return stencil_ref;
}

int State::get_stencil_mask() {
	int stencil_mask = 0;
	glGetIntegerv(GL_STENCIL_VALUE_MASK, &stencil_mask);
	return stencil_mask;
}

void State::set_stencil_func(int f, int r, int m) {
	glStencilFunc(GL_COMPARISON_FUNCTIONS[f], r, m);
}

int State::get_stencil_fail() {
	int stencil_fail = 0;
	glGetIntegerv(GL_STENCIL_FAIL, &stencil_fail);
	return get_stencil_operations(stencil_fail);
}

int State::get_stencil_zfail() {
	int stencil_zfail = 0;
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &stencil_zfail);
	return get_stencil_operations(stencil_zfail);
}

int State::get_stencil_zpass() {
	int stencil_zpass = 0;
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &stencil_zpass);
	return get_stencil_operations(stencil_zpass);
}

void State::set_stencil_op(int f, int zf, int zp) {
	glStencilOp(GL_STENCIL_OPERATIONS[f],
				GL_STENCIL_OPERATIONS[zf],
				GL_STENCIL_OPERATIONS[zp]);
}

void State::enable_blending() {
	glEnable(GL_BLEND);
}

void State::disable_blending() {
	glDisable(GL_BLEND);
}

int State::get_blend_op_rgb() {
	int blend_op_rgb = 0;
	glGetIntegerv(GL_BLEND_EQUATION_RGB, &blend_op_rgb);
	return get_blend_equations(blend_op_rgb);
}

int State::get_blend_op_alpha() {
	int blend_op_alpha = 0;
	glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &blend_op_alpha);
	return get_blend_equations(blend_op_alpha);
}

void State::set_blend_op(int o) {
	glBlendEquation(GL_BLEND_OPERATIONS[o]);
}

void State::set_blend_op(int rgb, int a) {
	glBlendEquationSeparate(GL_BLEND_OPERATIONS[rgb],
							GL_BLEND_OPERATIONS[a]);
}

int State::get_blend_src_rgb() {
	int blend_src_rgb = 0;
	glGetIntegerv(GL_BLEND_SRC_RGB, &blend_src_rgb);
	return get_blend_functions(blend_src_rgb);
}

int State::get_blend_src_alpha() {
	int blend_src_alpha = 0;
	glGetIntegerv(GL_BLEND_SRC_ALPHA, &blend_src_alpha);
	return get_blend_functions(blend_src_alpha);
}

int State::get_blend_dst_rgb() {
	int blend_dst_rgb = 0;
	glGetIntegerv(GL_BLEND_DST_RGB, &blend_dst_rgb);
	return get_blend_functions(blend_dst_rgb);
}

int State::get_blend_dst_alpha() {
	int blend_dst_alpha = 0;
	glGetIntegerv(GL_BLEND_DST_ALPHA, &blend_dst_alpha);
	return get_blend_functions(blend_dst_alpha);
}

void State::set_blend_factor(int s, int d) {
	glBlendFunc(GL_BLEND_FACTORS[s], GL_BLEND_FACTORS[d]);
}

void State::set_blend_factor(int srgb, int drgb, int sa, int da) {
	glBlendFuncSeparate(GL_BLEND_FACTORS[srgb], GL_BLEND_FACTORS[drgb],
						GL_BLEND_FACTORS[sa], GL_BLEND_FACTORS[da]);
}

Rect State::get_viewport() {
	Rect viewport_rect;
	glGetIntegerv(GL_VIEWPORT, &viewport_rect.x);
	return viewport_rect;
}

void State::set_viewport(const Rect& v) {
	glViewport(v.x, v.y, v.width, v.height);
}

void State::enable_scissor_test() {
	glEnable(GL_SCISSOR_TEST);
}

void State::disable_scissor_test() {
	glDisable(GL_SCISSOR_TEST);
}

Rect State::get_scissor() {
	Rect scissor_rect;
	glGetIntegerv(GL_SCISSOR_BOX, &scissor_rect.x);
	return scissor_rect;
}

void State::set_scissor(const Rect& s) {
	glScissor(s.x, s.y, s.width, s.height);
}

void State::enable_wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void State::disable_wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void State::enable_culling() {
	glEnable(GL_CULL_FACE);
}

void State::disable_culling() {
	glDisable(GL_CULL_FACE);
}

int State::get_cull_side() {
	int cull_side = 0;
	glGetIntegerv(GL_CULL_FACE, &cull_side);
	if (cull_side == GL_FRONT) return FRONT_SIDE;
	if (cull_side == GL_BACK ) return BACK_SIDE;
	return DOUBLE_SIDE;
}

void State::set_cull_side(int s) {
	if (s == FRONT_SIDE) {
		glCullFace(GL_FRONT);
	} else if (s == BACK_SIDE) {
		glCullFace(GL_BACK);
	} else if (s == DOUBLE_SIDE) {
		glCullFace(GL_FRONT_AND_BACK);
	}
}

void State::enable_polygon_offset() {
	glEnable(GL_POLYGON_OFFSET_FILL);
}

void State::disable_polygon_offset() {
	glDisable(GL_POLYGON_OFFSET_FILL);
}

float State::get_polygon_offset_factor() {
	float polygon_offset_factor = 0;
	glGetFloatv(GL_POLYGON_OFFSET_FACTOR, &polygon_offset_factor);
	return polygon_offset_factor;
}

float State::get_polygon_offset_units() {
	float polygon_offset_units = 0;
	glGetFloatv(GL_POLYGON_OFFSET_UNITS, &polygon_offset_units);
	return polygon_offset_units;
}

void State::set_polygon_offset(float f, float u) {
	glPolygonOffset(f, u);
}

void State::enable_dithering() {
	glEnable(GL_DITHER);
}

void State::disable_dithering() {
	glDisable(GL_DITHER);
}

void State::enable_multisample() {
	glEnable(GL_MULTISAMPLE);
}

void State::disable_multisample() {
	glDisable(GL_MULTISAMPLE);
}

void State::enable_alpha_to_coverage() {
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void State::disable_alpha_to_coverage() {
	glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void State::enable_texture_cube_seamless() {
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void State::disable_texture_cube_seamless() {
	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void MaterialState::set_depth(const Material& m) {
	if (!m.depth_test) return State::disable_depth_test();
	State::enable_depth_test();
	State::set_depth_func(m.depth_func);
}

void MaterialState::set_stencil(const Material& m) {
	if (!m.stencil_test) return State::disable_stencil_test();
	State::enable_stencil_test();
	State::set_stencil_writemask(m.stencil_writemask);
	State::set_stencil_func(m.stencil_func, m.stencil_ref, m.stencil_mask);
	State::set_stencil_op(m.stencil_fail, m.stencil_zfail, m.stencil_zpass);
}

void MaterialState::set_blending(const Material& m) {
	if (!m.blending) return State::disable_blending();
	State::enable_blending();
	State::set_blend_op(m.blend_op, m.blend_op_alpha);
	State::set_blend_factor(m.blend_src, m.blend_dst, m.blend_src_alpha, m.blend_dst_alpha);
}

void MaterialState::set_wireframe(const Material& m) {
	if (!m.wireframe) return State::disable_wireframe();
	State::enable_wireframe();
}

void MaterialState::set_side(const Material& m) {
	if (m.side == FRONT_SIDE) {
		State::enable_culling();
		State::set_cull_side(BACK_SIDE);
	} else if (m.side == BACK_SIDE) {
		State::enable_culling();
		State::set_cull_side(FRONT_SIDE);
	} else if (m.side == DOUBLE_SIDE) {
		State::disable_culling();
	}
}

void MaterialState::set_shadow_side(const Material& m) {
	if (m.shadow_side == FRONT_SIDE) {
		State::enable_culling();
		State::set_cull_side(BACK_SIDE);
	} else if (m.shadow_side == BACK_SIDE) {
		State::enable_culling();
		State::set_cull_side(FRONT_SIDE);
	} else if (m.shadow_side == DOUBLE_SIDE) {
		State::disable_culling();
	}
}

Shader::Shader() {
	program = glCreateProgram();
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::load_vert(const char* s) {
	vert_shader = s;
}

void Shader::load_vert(const std::string& s) {
	vert_shader = s;
}

void Shader::load_geom(const char* s) {
	geom_shader = s;
}

void Shader::load_geom(const std::string& s) {
	geom_shader = s;
}

void Shader::load_frag(const char* s) {
	frag_shader = s;
}

void Shader::load_frag(const std::string& s) {
	frag_shader = s;
}

void Shader::load_vert_file(const std::string& p) {
	vert_shader = File::read(p);
}

void Shader::load_geom_file(const std::string& p) {
	geom_shader = File::read(p);
}

void Shader::load_frag_file(const std::string& p) {
	frag_shader = File::read(p);
}

void Shader::compile() const {
	compile_shaders();
}

void Shader::use_program() const {
	glUseProgram(program);
}

void Shader::set_defines(const Defines& d) {
	defines = d.get();
}

void Shader::set_uniform_i(const std::string& n, int v) const {
	glUniform1i(glGetUniformLocation(program, n.c_str()), v);
}

void Shader::set_uniform_u(const std::string& n, unsigned int v) const {
	glUniform1ui(glGetUniformLocation(program, n.c_str()), v);
}

void Shader::set_uniform_f(const std::string& n, float v) const {
	glUniform1f(glGetUniformLocation(program, n.c_str()), v);
}

void Shader::set_uniform_v2(const std::string& n, const Vec2& v) const {
	glUniform2fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void Shader::set_uniform_v3(const std::string& n, const Vec3& v) const {
	glUniform3fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void Shader::set_uniform_v4(const std::string& n, const Vec4& v) const {
	glUniform4fv(glGetUniformLocation(program, n.c_str()), 1, &v.x);
}

void Shader::set_uniform_m2(const std::string& n, const Mat2& v) const {
	glUniformMatrix2fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void Shader::set_uniform_m3(const std::string& n, const Mat3& v) const {
	glUniformMatrix3fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void Shader::set_uniform_m4(const std::string& n, const Mat4& v) const {
	glUniformMatrix4fv(glGetUniformLocation(program, n.c_str()), 1, GL_TRUE, v[0]);
}

void Shader::set_uniforms(const Uniforms& u) const {
	auto* data_f = u.get_data();
	auto* data_i = reinterpret_cast<const int*>(data_f);
	auto* data_u = reinterpret_cast<const unsigned int*>(data_f);
	size_t uniform_count = u.count();
	for (int i = 0; i < uniform_count; ++i) {
		std::string name = u.get_name(i);
		int32_t gl_location = glGetUniformLocation(program, name.c_str());
		int type = u.get_type(i);
		int location = u.get_location(i);
		if (type == 0 /* int */ ) {
			glUniform1i(gl_location, data_i[location]);
		} else if (type == 1 /* unsigned int */ ) {
			glUniform1ui(gl_location, data_u[location]);
		} else if (type == 2 /* float */ ) {
			glUniform1f(gl_location, data_f[location]);
		} else if (type == 3 /* Vec2 */ ) {
			glUniform2fv(gl_location, 1, data_f + location);
		} else if (type == 4 /* Vec3 */ ) {
			glUniform3fv(gl_location, 1, data_f + location);
		} else if (type == 5 /* Vec4 */ ) {
			glUniform4fv(gl_location, 1, data_f + location);
		} else if (type == 6 /* Mat2 */ ) {
			glUniformMatrix2fv(gl_location, 1, GL_TRUE, data_f + location);
		} else if (type == 7 /* Mat3 */ ) {
			glUniformMatrix3fv(gl_location, 1, GL_TRUE, data_f + location);
		} else if (type == 8 /* Mat4 */ ) {
			glUniformMatrix4fv(gl_location, 1, GL_TRUE, data_f + location);
		} else {
			Error::set("Shader: Unknown uniform variable type");
		}
	}
}

void Shader::set_glsl_version(const std::string& v) {
	glsl_version = v;
}

uint32_t Shader::compile_shader(const std::string& s, int32_t t) const {
	std::string shader_string = s;
	resolve_defines(shader_string);
	resolve_version(shader_string);
	uint32_t shader_id = glCreateShader(t);
	const char* shader_str = shader_string.c_str();
	glShaderSource(shader_id, 1, &shader_str, nullptr);
	glCompileShader(shader_id);
	std::string info = get_compile_info(shader_id, t);
	if (!info.empty()) {
		Error::set(get_error_info(info, shader_string));
	}
	glAttachShader(program, shader_id);
	return shader_id;
}

void Shader::compile_shaders() const {
	/* compile vertex shader */
	bool use_vert_shader = !vert_shader.empty();
	uint32_t vert_id = 0;
	if (use_vert_shader) {
		vert_id = compile_shader(vert_shader, GL_VERTEX_SHADER);
	} else {
		return Error::set("Shader: Vertex shader is missing");
	}
	
	/* compile geometry shader */
	bool use_geom_shader = !geom_shader.empty();
	uint32_t geom_id = 0;
	if (use_geom_shader) {
		geom_id = compile_shader(geom_shader, GL_GEOMETRY_SHADER);
	}
	
	/* compile fragment shader */
	bool use_frag_shader = !frag_shader.empty();
	uint32_t frag_id = 0;
	if (use_frag_shader) {
		frag_id = compile_shader(frag_shader, GL_FRAGMENT_SHADER);
	} else {
		return Error::set("Shader: Fragment shader is missing");
	}
	
	/* link shaders to program */
	glLinkProgram(program);
	std::string info = get_link_info();
	if (!info.empty()) std::cerr << info;
	
	/* delete vertex shader */
	glDeleteShader(vert_id);
	
	/* delete geometry shader */
	if (use_frag_shader) glDeleteShader(geom_id);
	
	/* delete fragment shader */
	glDeleteShader(frag_id);
}

std::string Shader::get_link_info() const {
	int32_t success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_TRUE) return "";
	char info[1024];
	glGetProgramInfoLog(program, 1024, nullptr, info);
	return std::string("Shader: Link error\n") + info;
}

void Shader::resolve_defines(std::string& s) const {
	s = defines + s;
}

void Shader::resolve_version(std::string& s) {
	s = "#version " + glsl_version + "\n" + s;
}

std::string Shader::get_compile_info(uint32_t s, uint32_t t) {
	int32_t success;
	glGetShaderiv(s, GL_COMPILE_STATUS, &success);
	if (success == GL_TRUE) return "";
	char info[1024];
	glGetShaderInfoLog(s, 1024, nullptr, info);
	if (t == GL_VERTEX_SHADER) {
		return std::string("Shader: Vertex shader compile error\n") + info;
	} else if (t == GL_GEOMETRY_SHADER) {
		return std::string("Shader: Geometry shader compile error\n") + info;
	} else if (t == GL_FRAGMENT_SHADER) {
		return std::string("Shader: Fragment shader compile error\n") + info;
	}
	return "Shader: No such shader";
}

std::string Shader::get_error_info(const std::string& c, const std::string& s) {
	size_t line_begin = 0;
	size_t line_end = -1;
	
	/* initialize error information and error number */
	std::string info;
	int error_number = 0;
	
	while (true) {
		/* search the string of each line */
		line_begin = line_end + 1;
		line_end = c.find('\n', line_begin);
		if (line_end == -1) break;
		size_t line_length = line_end - line_begin;
		std::string line = c.substr(line_begin, line_length);
		info += line + '\n';
		if (line.substr(0, 9) != "ERROR: 0:") continue;
		
		/* get the line number from error information */
		size_t number_begin = line.find(':', 7) + 1;
		size_t number_end = c.find(':', number_begin);
		size_t number_length = number_end - number_begin;
		int line_number = std::stoi(line.substr(number_begin, number_length));
		
		/* search the code where the error occurred */
		size_t error_begin = 0;
		size_t error_end = -1;
		for (int i = 0; i < line_number; ++i) {
			error_begin = error_end + 1;
			error_end = s.find('\n', error_begin);
			if (error_end == -1) break;
		}
		size_t error_length = error_end - error_begin;
		std::string error_line = s.substr(error_begin, error_length);
		
		/* add code line to error information */
		info += error_line + "\n\n";
		++error_number;
	}
	
	/* count how many errors have occurred */
	info += std::to_string(error_number);
	info += error_number == 1 ? " error generated.\n" : " errors generated.\n";
	
	return info; /* return error infomation */
}

std::string Shader::glsl_version = "410";

VertexObject::VertexObject() {
	glGenVertexArrays(1, &id);
	glGenBuffers(1, &buffer_id);
}

VertexObject::~VertexObject() {
	glDeleteVertexArrays(1, &id);
	glDeleteBuffers(1, &buffer_id);
}

void VertexObject::load(const Mesh& m, const MeshGroup& g) {
	auto& vertex = m.vertex;
	auto& normal = m.normal;
	auto& uv = m.uv;
	auto& tangent = m.tangent;
	auto& color = m.color;
	
	/* check whether to use attributes */
	bool has_normal = !normal.empty();
	bool has_uv = !uv.empty();
	bool has_tangent = !tangent.empty();
	bool has_color = !color.empty();
	
	/* calculate length and stride */
	length = g.length;
	int group_end = g.position + length;
	int stride = 3;
	if (has_normal) stride += 3;
	if (has_uv) stride += 2;
	if (has_tangent) stride += 4;
	if (has_color) stride += 3;
	
	/* pack attributes' data into one vector */
	std::vector<float> data(length * stride);
	/* has_vertex */ {
		names = {"vertex"};
		sizes = {3};
		locations = {0};
		auto* data_ptr = data.data() + locations.back();
		for (int i = g.position; i < group_end; ++i) {
			data_ptr[0] = vertex[i].x;
			data_ptr[1] = vertex[i].y;
			data_ptr[2] = vertex[i].z;
			data_ptr += stride;
		}
		locations.emplace_back(locations.back() + 3);
	}
	if (has_normal) {
		names.emplace_back("normal");
		sizes.emplace_back(3);
		auto* data_ptr = data.data() + locations.back();
		for (int i = g.position; i < group_end; ++i) {
			data_ptr[0] = normal[i].x;
			data_ptr[1] = normal[i].y;
			data_ptr[2] = normal[i].z;
			data_ptr += stride;
		}
		locations.emplace_back(locations.back() + 3);
	}
	if (has_uv) {
		names.emplace_back("uv");
		sizes.emplace_back(2);
		auto* data_ptr = data.data() + locations.back();
		for (int i = g.position; i < group_end; ++i) {
			data_ptr[0] = uv[i].x;
			data_ptr[1] = uv[i].y;
			data_ptr += stride;
		}
		locations.emplace_back(locations.back() + 2);
	}
	if (has_tangent) {
		names.emplace_back("tangent");
		sizes.emplace_back(4);
		auto* data_ptr = data.data() + locations.back();
		for (int i = g.position; i < group_end; ++i) {
			data_ptr[0] = tangent[i].x;
			data_ptr[1] = tangent[i].y;
			data_ptr[2] = tangent[i].z;
			data_ptr[3] = tangent[i].w;
			data_ptr += stride;
		}
		locations.emplace_back(locations.back() + 4);
	}
	if (has_color) {
		names.emplace_back("color");
		sizes.emplace_back(3);
		auto* data_ptr = data.data() + locations.back();
		for (int i = g.position; i < group_end; ++i) {
			data_ptr[0] = color[i].x;
			data_ptr[1] = color[i].y;
			data_ptr[2] = color[i].z;
			data_ptr += stride;
		}
		locations.emplace_back(locations.back() + 3);
	}
	
	/* upload data to GPU */
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
}

void VertexObject::attach(const Shader& s) const {
	int stride = locations.back();
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	for (int i = 0; i < sizes.size(); ++i) {
		int32_t attrib = glGetAttribLocation(s.program, names[i].c_str());
		if (attrib == -1) continue;
		void* pointer = reinterpret_cast<void*>(sizeof(float) * locations[i]);
		glVertexAttribPointer(attrib, sizes[i], GL_FLOAT, GL_FALSE, sizeof(float) * stride, pointer);
		glEnableVertexAttribArray(attrib);
	}
}

void VertexObject::render() const {
	glBindVertexArray(id);
	glDrawArrays(GL_TRIANGLES, 0, length);
}

Texture::Texture() {
	glGenTextures(1, &id);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void Texture::init_1d(int w, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_1D, id);
	glTexImage1D(GL_TEXTURE_1D, 0, internal, w, 0, external, data, nullptr);
	set_parameters(TEXTURE_1D, f, w, 0, 0);
}

void Texture::init_2d(int w, int h, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal, w, h, 0, external, data, nullptr);
	set_parameters(TEXTURE_2D, f, w, h, 0);
}

void Texture::init_2d(const Image& i, int f) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_IMAGE_FORMATS[i.channel - 1];
	uint32_t data = GL_IMAGE_TYPES[i.bytes == 1 ? IMAGE_UBYTE : IMAGE_FLOAT];
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, internal, i.width, i.height, 0, external, data, i.data.data());
	set_parameters(TEXTURE_2D, f, i.width, i.height, 0);
}

void Texture::init_3d(int w, int h, int d, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_3D, id);
	glTexImage3D(GL_TEXTURE_3D, 0, internal, w, h, d, 0, external, data, nullptr);
	set_parameters(TEXTURE_3D, f, w, h, d);
}

void Texture::init_cube(int w, int h, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	for (int i = 0; i < 6; ++i) {
		uint32_t target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glTexImage2D(target, 0, internal, w, h, 0, external, data, nullptr);
	}
	set_parameters(TEXTURE_CUBE, f, w, h, 0);
}

void Texture::init_cube(const Image& px, const Image& nx, const Image& py,
						const Image& ny, const Image& pz, const Image& nz, int f) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_IMAGE_FORMATS[px.channel - 1];
	uint32_t data = GL_IMAGE_TYPES[px.bytes == 1 ? IMAGE_UBYTE : IMAGE_FLOAT];
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	uint32_t target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	glTexImage2D(target, 0, internal, px.width, px.height, 0, external, data, px.data.data());
	target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	glTexImage2D(target, 0, internal, nx.width, nx.height, 0, external, data, nx.data.data());
	target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	glTexImage2D(target, 0, internal, py.width, py.height, 0, external, data, py.data.data());
	target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	glTexImage2D(target, 0, internal, ny.width, ny.height, 0, external, data, ny.data.data());
	target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	glTexImage2D(target, 0, internal, pz.width, pz.height, 0, external, data, pz.data.data());
	target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	glTexImage2D(target, 0, internal, nz.width, nz.height, 0, external, data, nz.data.data());
	set_parameters(TEXTURE_CUBE, f, px.width, px.height, 0);
}

void Texture::init_1d_array(int w, int l, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_1D_ARRAY, id);
	glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, internal, w, l, 0, external, data, nullptr);
	set_parameters(TEXTURE_1D_ARRAY, f, w, l, 0);
}

void Texture::init_2d_array(int w, int h, int l, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal, w, h, l, 0, external, data, nullptr);
	set_parameters(TEXTURE_2D_ARRAY, f, w, h, l);
}

void Texture::init_cube_array(int w, int h, int l, int f, int t) {
	int32_t internal = GL_TEXTURE_FORMATS[f].first;
	uint32_t external = GL_TEXTURE_FORMATS[f].second;
	uint32_t data = GL_IMAGE_TYPES[t];
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, id);
	for (int i = 0; i < l * 6; ++i) {
		glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, internal, w, h, i, 0, external, data, nullptr);
	}
	set_parameters(TEXTURE_CUBE_ARRAY, f, w, h, l);
}

int Texture::get_type() const {
	return type;
}

int Texture::get_format() const {
	return format;
}

int Texture::get_width() const {
	return width;
}

int Texture::get_height() const {
	return height;
}

int Texture::get_depth() const {
	return depth;
}

int Texture::get_layer() const {
	/* layer of texture 1D (width) */
	if (type == TEXTURE_1D_ARRAY) return height;
	
	/* layer of texture 2D (width x height) */
	if (type == TEXTURE_2D_ARRAY) return depth;
	
	/* layer of texture cube (width x height) */
	if (type == TEXTURE_CUBE_ARRAY) return depth;
	
	/* illegal texture */
	return 0;
}

void Texture::copy_to_image(Image& i) const {
	/* check whether the texture is 2D */
	if (type != TEXTURE_2D) {
		return Error::set("Texture: Cannot get image from non-2D texture");
	}
	
	/* get the external format of texture */
	uint32_t external = GL_TEXTURE_FORMATS[format].second;
	
	/* get image data from GPU */
	uint8_t* image_data = i.data.data();
	uint32_t image_type = i.bytes == 1 ? GL_UNSIGNED_BYTE : GL_FLOAT;
	glGetTexImage(GL_TEXTURE_2D, 0, external, image_type, image_data);
}

void Texture::generate_mipmap() const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glGenerateMipmap(gl_type);
}

void Texture::set_wrap_s(int m) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAPPINGS[m]);
}

void Texture::set_wrap_t(int m) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAPPINGS[m]);
}

void Texture::set_wrap_r(int m) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_WRAP_R, GL_TEXTURE_WRAPPINGS[m]);
}

void Texture::set_wrap_all(int m) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAPPINGS[m]);
	glTexParameteri(gl_type, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAPPINGS[m]);
	glTexParameteri(gl_type, GL_TEXTURE_WRAP_R, GL_TEXTURE_WRAPPINGS[m]);
}

void Texture::set_filters(int mag, int min) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_FILTERS[mag]);
	glTexParameteri(gl_type, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_FILTERS[min]);
}

void Texture::set_border_color(const Vec4& c) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameterfv(gl_type, GL_TEXTURE_BORDER_COLOR, &c.x);
}

void Texture::set_lod_range(int min, int max) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_BASE_LEVEL, min);
	glTexParameteri(gl_type, GL_TEXTURE_MAX_LEVEL, max);
}

void Texture::set_lod_bias(int b) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glBindTexture(gl_type, id);
	glTexParameteri(gl_type, GL_TEXTURE_LOD_BIAS, b);
}

int Texture::activate(int l) const {
	uint32_t gl_type = GL_TEXTURE_TYPES[type];
	glActiveTexture(GL_TEXTURE0 + l);
	glBindTexture(gl_type, id);
	return l;
}

void Texture::set_parameters(int t, int f, int w, int h, int d) {
	type = t;
	format = f;
	width = w;
	height = h;
	depth = d;
}

int Texture::default_format(const Image& i) {
	return TEXTURE_DEFAULT_FORMATS[i.channel - 1][i.bytes == 4];
}

int Texture::default_format(int c, int b) {
	return TEXTURE_DEFAULT_FORMATS[c - 1][b == 4];
}

RenderBuffer::RenderBuffer() {
	glGenRenderbuffers(1, &id);
}

RenderBuffer::~RenderBuffer() {
	glDeleteRenderbuffers(1, &id);
}

void RenderBuffer::init(int w, int h, int f) const {
	int32_t internal = GL_TEXTURE_FORMATS[f].second;
	glBindRenderbuffer(GL_RENDERBUFFER, id);
	glRenderbufferStorage(GL_RENDERBUFFER, internal, w, h);
}

RenderTarget::RenderTarget() {
	glGenFramebuffers(1, &id);
}

RenderTarget::~RenderTarget() {
	glDeleteFramebuffers(1, &id);
}

void RenderTarget::set_texture(const Texture& t, unsigned int i, int l, int p) const {
	set_framebuffer(t, GL_COLOR_ATTACHMENT0 + i, l, p);
}

void RenderTarget::set_depth_texture(const Texture& t, int l, int p) const {
	set_framebuffer(t, GL_DEPTH_ATTACHMENT, l, p);
}

void RenderTarget::set_stencil_texture(const Texture& t, int l, int p) const {
	set_framebuffer(t, GL_STENCIL_ATTACHMENT, l, p);
}

void RenderTarget::set_depth_stencil_texture(const Texture& t, int l, int p) const {
	set_framebuffer(t, GL_DEPTH_STENCIL_ATTACHMENT, l, p);
}

void RenderTarget::set_depth_buffer(const RenderBuffer& r) const {
	set_framebuffer(r, GL_DEPTH_ATTACHMENT);
}

void RenderTarget::set_stencil_buffer(const RenderBuffer& r) const {
	set_framebuffer(r, GL_STENCIL_ATTACHMENT);
}

void RenderTarget::set_depth_stencil_buffer(const RenderBuffer& r) const {
	set_framebuffer(r, GL_DEPTH_STENCIL_ATTACHMENT);
}

void RenderTarget::set_target_number(int n) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glDrawBuffers(n, GL_COLOR_ATTACHMENTS);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::activate() const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Error::set("RenderTarget: Render target is not complete");
	}
}

void RenderTarget::activate(const RenderTarget* f) {
	uint32_t id = f == nullptr ? 0 : f->id;
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Error::set("RenderTarget: Render target is not complete");
	}
}

void RenderTarget::set_framebuffer(const Texture& t, uint32_t a, int l, int p) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	if (t.type == TEXTURE_2D) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, a, GL_TEXTURE_2D, t.id, l);
	} else if (t.type == TEXTURE_CUBE) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, a, GL_TEXTURE_CUBE_MAP_POSITIVE_X + p, t.id, l);
	} else if (t.type == TEXTURE_3D) {
		glFramebufferTexture3D(GL_FRAMEBUFFER, a, GL_TEXTURE_3D, t.id, l, p);
	} else if (t.type == TEXTURE_2D_ARRAY) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, a, t.id, l, p);
	} else if (t.type == TEXTURE_CUBE_ARRAY) {
		glFramebufferTextureLayer(GL_FRAMEBUFFER, a, t.id, l, p);
	} else {
		Error::set("RenderTarget: Texture type is not supported");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTarget::set_framebuffer(const RenderBuffer& r, uint32_t a) const {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, a, GL_RENDERBUFFER, r.id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
