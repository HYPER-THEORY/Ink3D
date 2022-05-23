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

#include "../math/maths.h"
#include "../objects/image.h"
#include "../objects/mesh.h"
#include "../graphics/gpu.h"
#include "../meshes/boxmesh.h"

class cubemap {
public:
	/**
	 * Init cubemap.
	 */
	static void init();
	
	/**
	 * Create a new cubemap.
	 */
	cubemap() = default;
	
	/**
	 * Preload the images of cubemap.
	 *
	 * \param is left, right, upper, lower, front, back images
	 */
	void preload(const image* is);
	
	/**
	 * Returns the current target framebuffer if there are, returns nullptr
	 * otherwise.
	 */
	const gpu::framebuffer* get_target() const;
	
	/**
	 * Sets the target framebuffer. When nullptr is given, the default
	 * framebuffer is set as the target framebuffer.
	 *
	 * \param t target framebuffer
	 */
	void set_target(const gpu::framebuffer* t);
	
	/**
	 * Returns the current custom shader if there are, returns nullptr
	 * otherwise.
	 */
	const gpu::shader* get_shader() const;
	
	/**
	 * Sets the custom shader. When nullptr is given, use the default shader
	 * instead.
	 *
	 * \param s custom shader
	 */
	void set_shader(const gpu::shader* s);
	
	/**
	 * Render the cubemap using a camera.
	 *
	 * \param c camera
	 */
	void render(const camera& c) const;

private:
	const gpu::framebuffer* target = nullptr;
	const gpu::shader* custom_shader = nullptr;
	
	std::unique_ptr<gpu::texture> cube_map;
	
	static std::unique_ptr<gpu::vertexobject> cube;
	static std::unique_ptr<gpu::shader> default_shader;
};
