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

#pragma once

#include "Enums.h"
#include "Defines.h"
#include "Uniforms.h"
#include "Image.h"

#include "../math/Vector.h"

#include <string>
#include <unordered_map>

namespace Ink {

class Material {
public:
	std::string name;                     /**< material name */
	
	RenderSide side;                      /**< which side of faces will be rendered */
	
	RenderSide shadow_side;               /**< which side of faces will cast shadows */
	
	bool visible = true;                  /**< whether the material will be rendered */
	
	bool wireframe = false;               /**< whether to render mesh as wireframe */
	
	bool depth_test = true;               /**< whether to enable depth test in rendering */
	
	ComparisonFunc depth_func;            /**< which depth comparison function to use */
	
	bool stencil_test = false;            /**< whether to enable stencil test in rendering */
	
	int stencil_writemask = 0xFF;         /**< the mask when writing to stencil buffer */
	
	int stencil_ref = 0;                  /**< the reference value to be used in stencil comparison */
	
	int stencil_mask = 0xFF;              /**< the mask to be used in stencil comparison */
	
	ComparisonFunc stencil_func;          /**< which stencil comparison function to use */
	
	StencilOperation stencil_fail;        /**< the operation when the stencil test fails */
	
	StencilOperation stencil_zfail;       /**< the operation when the stencil test passes but depth test fails */
	
	StencilOperation stencil_zpass;       /**< the operation when both the stencil test and depth test pass */
	
	bool blending = false;                /**< whether to enable blending in rendering */
	
	BlendOperation blend_op_rgb;          /**< which RGB blend operation to use in blending */
	
	BlendOperation blend_op_alpha;        /**< which alpha blend operation to use in blending */
	
	BlendFactor blend_src_rgb;            /**< the RGB source blend factor in blending */
	
	BlendFactor blend_src_alpha;          /**< the alpha source blend factor in blending */
	
	BlendFactor blend_dst_rgb;            /**< the RGB destination blend factor in blending */
	
	BlendFactor blend_dst_alpha;          /**< the alpha destination blend factor in blending */
	
	float alpha_test = 0;                 /**< threshold of alpha test, pixels with lower alpha will be discarded */
	
	bool map_with_alpha = true;           /**< whether to use alpha channel from color map */
	
	bool vertex_color = false;            /**< whether to use vertex colors from mesh */
	
	bool tangent_space = true;            /**< whether the normal map defines in tangent space */
	
	float normal_scale = 1;               /**< how much the normal map affects the material */
	
	float displacement_scale = 1;         /**< how much the displacement map affects the mesh */
	
	Vec3 color = {1, 1, 1};               /**< the base color of material, default is white */
	
	float alpha = 1;                      /**< the opacity of material, range is 0 to 1 */
	
	float specular = 0.5;                 /**< how specular the material appears, range is 0 to 1 */
	
	float metalness = 0;                  /**< how metallic the material appears, range is 0 to 1 */
	
	float roughness = 1;                  /**< how rough the material appears, range is 0 to 1 */
	
	Vec3 emissive = {0, 0, 0};            /**< the emissive color of material, default is black */
	
	float emissive_intensity = 1;         /**< the emissive intensity of material, range is 0 to 1 */
	
	float ao_intensity = 1;               /**< the occlusion intensity of material, range is 0 to 1 */
	
	Image* normal_map = nullptr;          /**< the map determines the normals of mesh */
	
	Image* displacement_map = nullptr;    /**< the map determines the offsets of mesh's vertices */
	
	Image* color_map = nullptr;           /**< the map affects the base color of material */
	
	Image* alpha_map = nullptr;           /**< the map affects the alpha of material */
	
	Image* roughness_map = nullptr;       /**< the map affects how rough the material appears */
	
	Image* metalness_map = nullptr;       /**< the map affects how metallic the material appears */
	
	Image* specular_map = nullptr;        /**< the map affects how specular the material appears */
	
	Image* emissive_map = nullptr;        /**< the map affects the emissive color of material */
	
	Image* ao_map = nullptr;              /**< the map affects the ambient occlusion of material */
	
	Image* custom_maps[16];               /**< the custom maps of material */
	
	void* shader = nullptr;               /**< custom shader determines how lights affect material */
	
	void* reflection_probe = nullptr;     /**< reflection probe affects the environment light of material */
	
	Uniforms* uniforms = nullptr;         /**< the custom uniforms of material */
	
	/**
	 * Creates a new Material object and initializes it with a name.
	 *
	 * \param n material name
	 */
	explicit Material(const std::string& n = "");
};

}
