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

#include "../core/error.h"
#include "../math/maths.h"
#include "../objects/constants.h"
#include "../objects/uniforms.h"
#include "../objects/image.h"
#include "../objects/mesh.h"
#include "../objects/instance.h"
#include "../objects/scene.h"
#include "../objects/material.h"
#include "../camera/camera.h"
#include "../graphics/gpu.h"

class renderer {
public:
	/**
	 * Create a new renderer.
	 */
	renderer() = default;
	
	/**
	 * Clear the color, depth or stencil drawing buffers. Initialize the color
	 * buffer to the current clear color value.
	 *
	 * \param c whether to clear the color buffer
	 * \param d whether to clear the depth buffer
	 * \param s whether to clear the stencil buffer
	 */
	void clear(bool c = true, bool d = true, bool s = true) const;
	
	/**
	 * Returns the clear color and opacity.
	 */
	vec4 get_clear_color() const;
	
	/**
	 * Sets the clear color and opacity.
	 *
	 * \param c clear color
	 */
	void set_clear_color(const vec4& c);
	
	/**
	 * Returns the viewport.
	 */
	gpu::rect get_viewport() const;
	
	/**
	 * Sets the viewport.
	 *
	 * \param v viewport
	 */
	void set_viewport(const gpu::rect& v);
	
	/**
	 * Determines whether to enable scissor test.
	 */
	bool get_scissor_test() const;
	
	/**
	 * Determines whether to enable scissor test.
	 *
	 * \param t enable scissor test
	 */
	void set_scissor_test(bool t);
	
	/**
	 * Returns the scissor region.
	 */
	gpu::rect get_scissor() const;
	
	/**
	 * Sets the scissor region.
	 *
	 * \param s scissor region
	 */
	void set_scissor(const gpu::rect& s);
	
	/**
	 * Returns the current target framebuffer if there are, returns nullptr
	 * otherwise.
	 */
	const gpu::framebuffer* get_target() const;
	
	/**
	 * Set the target framebuffer. When nullptr is given, the default
	 * framebuffer is set as the target framebuffer.
	 *
	 * \param t target framebuffer
	 */
	void set_target(const gpu::framebuffer* t);
	
	/**
	 * Preload meshes and images in a scene.
	 *
	 * \param s scene
	 * \param wrap wrapping type of textures
	 * \param mag magnifying filter type of textures
	 * \param min minifying filter type of textures
	 * \param mip mipmap type of textures
	 */
	void preload(const scene& s, int wrap = TEXTURE_REPEAT, int mag = TEXTURE_LINEAR,
				 int min = TEXTURE_LINEAR_MIPMAP_LINEAR, bool mip = true);
	
	/**
	 * Render a scene using a camera.
	 *
	 * \param s scene
	 * \param c camera
	 */
	void render(const scene& s, const camera& c) const;
	
private:
	bool scissor_test = false;
	vec4 clear_color = {0, 0, 0, 1};
	gpu::rect scissor = {0, 0, 0, 0};
	gpu::rect viewport = {0, 0, 0, 0};

	const gpu::framebuffer* target = nullptr;
	
	std::unordered_map<const mesh*, std::unique_ptr<gpu::vertexobject[]> > mesh_buffer;
	std::unordered_map<const image*, std::unique_ptr<gpu::texture> > image_buffer;
};
