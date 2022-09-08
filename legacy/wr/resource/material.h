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
#include "../core/math.h"
#include "image.h"

enum material_type {
	MATERIAL_DEFAULT,
	MATERIAL_SIMPLE,
};

enum material_map {
	NO_MAP = -1,
};

enum rendering_side {
	FRONT_SIDE,
	BACK_SIDE,
	DOUBLE_SIDE,
};

class material {
public:
	std::string name;				/* material name */
	
	int side = FRONT_SIDE;			/* which side of faces will be rendered */
	
	float alpha_test = 0;			/* alpha value of alpha test */
	bool depth_test = true;			/* whether to enable depth test */
	bool stencil_test = true;		/* whether to enable stencil test */
	bool wireframe = false;			/* whether to enable wireframe mode */
	
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
	 * Get material type.
	 */
	virtual int type() const;
	
	/**
	 * Get shader which is linked with material.
	 */
	const void* get_shader() const;
	
	/**
	 * Set shader to link with material.
	 *
	 * \param s shader
	 */
	void set_shader(const void* s);
	
	/**
	 * Get images size which is linked with material.
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
	 * Add images linked with material.
	 *
	 * \param i image list
	 */
	void add_images(const std::initializer_list<const image*>& i);
	
	/**
	 * Get images which is linked with material.
	 */
	const image** get_images();
	
	/**
	 * Get images which is linked with material.
	 */
	const image* const* get_images() const;
	
protected:
	const void* linked_shader;
	std::vector<const image*> linked_images;
};
