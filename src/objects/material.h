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

#include "image.h"

/* rendering side */
enum {
	FRONT_SIDE,
	BACK_SIDE,
	DOUBLE_SIDE,
};

class material {
public:
	std::string name;				/* material name */
	
	int side = FRONT_SIDE;			/* which side will be rendered */
	
	float alpha_test = 0;			/* alpha value in alpha test */
	bool depth_test = true;			/* whether to enable depth test */
	bool stencil_test = true;		/* whether to enable stencil test */
	bool wireframe = false;			/* whether to enable wireframe mode */
	
	vec3 color = {1};				/* color of material */
	int color_map = 0;				/* color map of material */
	
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
	 * Returns shader linked with material.
	 */
	const void* get_shader() const;
	
	/**
	 * Sets shader to link with material.
	 *
	 * \param s shader
	 */
	void set_shader(const void* s);
	
	/**
	 * Determine images size linked with material.
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
	 * Returns images linked with material.
	 */
	const image** get_images();
	
	/**
	 * Returns images linked with material.
	 */
	const image* const* get_images() const;
	
	/**
	 * Sets color map.
	 *
	 * \param c color map
	 */
	void set_color_map(const image* c);
	
protected:
	const void* linked_shader;
	std::vector<const image*> linked_images;
};
