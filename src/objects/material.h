/*
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
#include <unordered_map>

#include "../math/maths.h"

#include "constants.h"
#include "uniforms.h"
#include "image.h"

class material {
public:
	std::string name;						/* material name */
	
	bool visible = true;					/* whether the material is visible */
	int side = FRONT_SIDE;					/* which side of faces will be rendered */
	
	bool depth_test = true;					/* whether to enable depth test */
	int depth_func = FUNC_LEQUAL;			/* which depth comparison function to use */
	
	bool stencil_test = true;				/* whether to enable stencil test */
	int stencil_writemask = 0xFF;			/* the mask when writing to stencil buffer */
	int stencil_func = FUNC_ALWAYS;			/* which stencil comparison function to use */
	int stencil_ref = 0;					/* the reference value in stencil comparison */
	int stencil_mask = 0xFF;				/* the mask in stencil comparison */
	int stencil_fail = STENCIL_KEEP;		/* the operation when the stencil test fails */
	int stencil_zfail = STENCIL_KEEP;		/* the operation when the stencil test passes but the depth test fails */
	int stencil_zpass = STENCIL_KEEP;		/* the operation when both the stencil test and the depth test pass */
	
	bool wireframe = false;					/* whether to render mesh as wireframe */
	
	float alpha_test = 0;					/* the alpha value in alpha test */
	
	vec3 color = {1};						/* base color factor */
	int color_map = 0;						/* base color map */
	
	/**
	 * Create a new material.
	 */
	material() = default;
	
	/**
	 * Create a new material.
	 *
	 * \param n name
	 */
	material(const std::string& n);
	
	/**
	 * Add color map.
	 *
	 * \param c color map
	 */
	void add_color_map(const image* c);
	
	/**
	 * Returns the uniform variables of material.
	 */
	uniforms get_uniforms() const;
	
	/**
	 * Returns the shader linked with material.
	 */
	const void* get_shader() const;
	
	/**
	 * Sets shader to link with material.
	 *
	 * \param s shader
	 */
	void set_shader(const void* s);
	
	/**
	 * Determines the size of images linked with material.
	 */
	size_t image_size() const;
	
	/**
	 * Clear images.
	 */
	void clear_image();
	
	/**
	 * Add image to link with material.
	 *
	 * \param i image
	 */
	void add_image(const image* i);
	
	/**
	 * Add images to link with material.
	 *
	 * \param i image list
	 */
	void add_images(const std::initializer_list<const image*>& i);
	
	/**
	 * Returns the images linked with material.
	 */
	const image** get_images();
	
	/**
	 * Returns the images linked with material.
	 */
	const image* const* get_images() const;
	
protected:
	const void* linked_shader;
	std::vector<const image*> linked_images;
};
