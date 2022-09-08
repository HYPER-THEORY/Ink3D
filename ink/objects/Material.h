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

#include "Constants.h"
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
	
	bool wireframe = false;                     /**< whether to render mesh as wireframe */
	
	bool depth_test = true;                     /**< whether to enable depth test */
	
	int depth_func = FUNC_LEQUAL;               /**< which depth comparison function to use */
	
	bool stencil_test = false;                  /**< whether to enable stencil test */
	
	int stencil_writemask = 0xFF;               /**< the mask when writing to stencil buffer */
	
	int stencil_func = FUNC_ALWAYS;             /**< which stencil comparison function to use */
	
	int stencil_ref = 0;                        /**< the reference value in stencil comparison */
	
	int stencil_mask = 0xFF;                    /**< the mask in stencil comparison */
	
	int stencil_fail = STENCIL_KEEP;            /**< the operation when the stencil test fails */
	
	int stencil_zfail = STENCIL_KEEP;           /**< the operation when the stencil test passes but depth test fails */
	
	int stencil_zpass = STENCIL_KEEP;           /**< the operation when both the stencil test and depth test pass */
	
	bool blending = false;                      /**< whether to enable blending */
	
	int blend_op = BLEND_ADD;                   /**< the RGB blend operation when applying blending */
	
	int blend_op_alpha = BLEND_ADD;             /**< the alpha blend operation when applying blending */
	
	int blend_src = FACTOR_ONE;                 /**< the RGB source blend factor when applying blending */
	
	int blend_src_alpha = FACTOR_ONE;           /**< the alpha source blend factor when applying blending */
	
	int blend_dst = FACTOR_ZERO;                /**< the RGB source destination factor when applying blending */
	
	int blend_dst_alpha = FACTOR_ZERO;          /**< the alpha source destination factor when applying blending */
	
	float alpha_test = 0;                       /**< the alpha value in alpha test */
	
	bool tangent_space = true;                  /**< whether the normal map defines in tangent space */
	
	float normal_scale = 1;                     /**< the scale of the normal map */
	
	float displacement_scale = 1;               /**< the scale of the displacement map */
	
	Vec3 color = {1, 1, 1};                     /**< the base color of material */
	
	float alpha = 1;                            /**< the opacity of material */
	
	Vec3 emissive = {0, 0, 0};                  /**< the emissive color of material */
	 
	float emissive_intensity = 1;               /**< the emissive intensity of material */
	
	float specular = 1;                         /**< how specular the material appears */
	
	float metalness = 0;                        /**< how metallic the material appears */
	
	float roughness = 1;                        /**< how rough the material appears */
	
	const Image* normal_map = nullptr;          /**< the map determines the normals of mesh */
	
	const Image* displacement_map = nullptr;    /**< the map determines the offsets of mesh's vertices */
	
	const Image* color_map = nullptr;           /**< the map affects the base color of material */
	
	const Image* emissive_map = nullptr;        /**< the map affects the emissive color of material */
	
	const Image* roughness_map = nullptr;       /**< the map affects how rough the material appears */
	
	const Image* metalness_map = nullptr;       /**< the map affects how metallic the material appears */
	
	const Image* specular_map = nullptr;        /**< the map affects how specular the material appears */
	
	/**
	 * Create a new Material.
	 *
	 * \param n name
	 */
	Material(const std::string& n = "");
	
	/**
	 * Returns the uniform variables of material.
	 */
	Uniforms get_uniforms() const;
	
	/**
	 * Returns the #define directives of material.
	 */
	Defines get_defines() const;
	
	/**
	 * Determines the number of images linked with material.
	 */
	size_t get_image_count() const;
	
	/**
	 * Link an image to material.
	 *
	 * \param i image
	 */
	void add_image(const Image* i);
	
	/**
	 * Link images to material.
	 *
	 * \param l image list
	 */
	void add_images(const std::initializer_list<const Image*>& l);
	
	/**
	 * Clear all the images.
	 */
	void clear_image();
	
	/**
	 * Returns the image at the index.
	 *
	 * \param i the index of image
	 */
	const Image* get_image(int i) const;
	
	/**
	 * Returns the shader linked with material.
	 */
	const void* get_shader() const;
	
	/**
	 * Returns the shader linked with material.
	 */
	template <typename Type>
	const Type* get_shader() const;
	
	/**
	 * Link a shader to material.
	 *
	 * \param s shader
	 */
	void set_shader(const void* s);
	
	/**
	 * Returns the light probe linked with material.
	 */
	const void* get_light_probe() const;
	
	/**
	 * Returns the light probe linked with material.
	 */
	template <typename Type>
	const Type* get_light_probe() const;
	
	/**
	 * Link a light probe to material.
	 *
	 * \param p light probe
	 */
	void set_light_probe(const void* p);
	
	/**
	 * Returns the environment probe linked with material.
	 */
	const void* get_env_probe() const;
	
	/**
	 * Returns the environment probe linked with material.
	 */
	template <typename Type>
	const Type* get_env_probe() const;
	
	/**
	 * Link a environment probe to material.
	 *
	 * \param p light probe
	 */
	void set_env_probe(const void* p);
	
private:
	const void* shader = nullptr;
	const void* light_probe = nullptr;
	const void* env_probe = nullptr;
	
	std::vector<const Image*> images;
};

/* template implementations */

template <typename Type>
const Type* Material::get_shader() const {
	return static_cast<const Type*>(shader);
}

template <typename Type>
const Type* Material::get_light_probe() const {
	return static_cast<const Type*>(light_probe);
}

template <typename Type>
const Type* Material::get_env_probe() const {
	return static_cast<const Type*>(env_probe);
}

}
