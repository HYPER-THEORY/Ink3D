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

#include <memory>
#include <unordered_map>
#include "../resource/image.h"
#include "../resource/mesh.h"
#include "../resource/material.h"
#include "../resource/simplemat.h"
#include "../graphics/instance.h"
#include "../graphics/camera.h"
#include "../graphics/gpu.h"
#include "scene.h"

class renderer {
public:
	/**
	 * Create a new renderer.
	 */
	renderer() = default;
	
	/**
	 * Preload scene before render it.
	 *
	 * \param s scene
	 */
	void preload(const scene& s);
	
	/**
	 * Clear render buffer.
	 */
	void clear();
	
	/**
	 * Render scene.
	 *
	 * \param s scene
	 * \param c camera information
	 */
	void render(const scene& s, const camera& c) const;
	
	/**
	 * Set parameters of preloaded textures.
	 *
	 * \param w wrapping type
	 * \param mag magnifying filter type
	 * \param min minifying filter type
	 * \param mip mipmap type
	 */
	void texture_parameter(int w = TEXTURE_REPEAT, int mag = TEXTURE_LINEAR,
						   int min = TEXTURE_NEAREST, int mip = TEXTURE_NO_MIPMAP);
	
private:
	std::unordered_map<const mesh*, std::unique_ptr<gpu::vertexobject[]> > mesh_buffer;
	std::unordered_map<const image*, std::unique_ptr<gpu::texture> > image_buffer;
	
	void use_material(const material& m, const gpu::vertexobject& o,
					  const std::unordered_map<std::string, mat<4, 4> >& k) const;
	
	static void material_depth_test(const material& m);
	
	static void material_stencil_test(const material& m);
	
	static void material_wireframe(const material& m);
	
	static void material_side(const material& m);
	
	static void material_shading(const material& m);
	
	static void simple_shading(const simplemat& m);
};
