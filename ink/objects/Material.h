/**
 * Copyright (C) 2021-2022 Hypertheory
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

#include <string>
#include <vector>

#include "../math/Maths.h"

#include "Enums.h"
#include "Defines.h"
#include "Uniforms.h"
#include "Image.h"

namespace Ink {

class Material {
public:
	std::string name;                           /**< material name */
	
	bool visible = true;                        /**< whether the material will be rendered */
	
	int side = FRONT_SIDE;                      /**< which side of faces will be rendered */
	
	int shadow_side = BACK_SIDE;                /**< which side of faces will cast shadows */
	
	bool transparent = false;                   /**< whether the material is transparent */
	
	bool wireframe = false;                     /**< whether to render mesh as wireframe */
	
	bool depth_test = true;                     /**< whether to enable depth test in rendering */
	
	int depth_func = FUNC_LEQUAL;               /**< which depth comparison function to use */
	
	bool stencil_test = false;                  /**< whether to enable stencil test in rendering */
	
	int stencil_writemask = 0xFF;               /**< the mask when writing to stencil buffer */
	
	int stencil_func = FUNC_ALWAYS;             /**< which stencil comparison function to use */
	
	int stencil_ref = 0;                        /**< the reference value to be used in stencil comparison */
	
	int stencil_mask = 0xFF;                    /**< the mask to be used in stencil comparison */
	
	int stencil_fail = STENCIL_KEEP;            /**< the operation when the stencil test fails */
	
	int stencil_zfail = STENCIL_KEEP;           /**< the operation when the stencil test passes but depth test fails */
	
	int stencil_zpass = STENCIL_KEEP;           /**< the operation when both the stencil test and depth test pass */
	
	bool blending = false;                      /**< whether to enable blending in rendering */
	
	int blend_op = BLEND_ADD;                   /**< which blend operation to use in RGB blending */
	
	int blend_op_alpha = BLEND_ADD;             /**< which blend operation to use in alpha blending */
	
	int blend_src = FACTOR_ONE;                 /**< the RGB source blend factor in blending */
	
	int blend_src_alpha = FACTOR_ONE;           /**< the alpha source blend factor in blending */
	
	int blend_dst = FACTOR_ZERO;                /**< the RGB destination blend factor in blending */
	
	int blend_dst_alpha = FACTOR_ZERO;          /**< the alpha destination blend factor in blending */
	
	float alpha_test = 0;                       /**< the threshold, pixels with lower alpha will be discarded */
	
	bool color_with_alpha = true;               /**< whether to use alpha channel from color map */
	
	bool tangent_space = true;                  /**< whether the normal map defines in tangent space */
	
	float normal_scale = 1;                     /**< how much the normal map affects the material */
	
	float displacement_scale = 1;               /**< how much the displacement map affects the mesh */
	
	Vec3 color = {1, 1, 1};                     /**< the base color of material, default is white */
	
	float alpha = 1;                            /**< the opacity of material, range is 0 to 1 */
	
	Vec3 emissive = {0, 0, 0};                  /**< the emissive color of material, default is black */
	
	float emissive_intensity = 1;               /**< the emissive intensity of material, range is 0 to 1 */
	
	float ao_intensity = 1;                     /**< the occlusion intensity of material, range is 0 to 1 */
	
	float specular = 0.5;                       /**< how specular the material appears, range is 0 to 1 */
	
	float metalness = 0;                        /**< how metallic the material appears, range is 0 to 1 */
	
	float roughness = 1;                        /**< how rough the material appears, range is 0 to 1 */
	
	const Image* normal_map = nullptr;          /**< the map determines the normals of mesh */
	
	const Image* displacement_map = nullptr;    /**< the map determines the offsets of mesh's vertices */
	
	const Image* color_map = nullptr;           /**< the map affects the base color of material */
	
	const Image* alpha_map = nullptr;           /**< the map affects the alpha of material */
	
	const Image* emissive_map = nullptr;        /**< the map affects the emissive color of material */
	
	const Image* ao_map = nullptr;              /**< the map affects the ambient occlusion of material */
	
	const Image* roughness_map = nullptr;       /**< the map affects how rough the material appears */
	
	const Image* metalness_map = nullptr;       /**< the map affects how metallic the material appears */
	
	const Image* specular_map = nullptr;        /**< the map affects how specular the material appears */
	
	const void* shader = nullptr;               /**< custom shader determines how lights affect material */
	
	const void* light_probe = nullptr;          /**< light probe affects the diffuse light of material */
	
	const void* reflection_probe = nullptr;     /**< reflection probe affects the specular light of material */
	
	/**
	 * Creates a new Material and initializes it with a name.
	 *
	 * \param n material name
	 */
	explicit Material(const std::string& n = "");
	
	/**
	 * Adds the specified image as custom image to the material.
	 *
	 * \param i image
	 */
	void add_image(const Image* i);
	
	/**
	 * Adds the specified image list as custom images to the material.
	 *
	 * \param l image list
	 */
	void add_images(const std::initializer_list<const Image*>& l);
	
	/**
	 * Clears all the custom images from the material.
	 */
	void clear_image();
	
	/**
	 * Returns the number of custom images linked with material.
	 */
	size_t get_image_count() const;
	
	/**
	 * Returns the custom image at the specified index in the material.
	 *
	 * \param i the index of image
	 */
	const Image* get_image(int i) const;
	
private:
	std::vector<const Image*> images;
};

}
