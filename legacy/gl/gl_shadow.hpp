/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GL_SHADOW
#define GL_SHADOW

#include "gl_stdinc.hpp"
#include "gl_type.hpp"
#include "gl_gpu.hpp"

namespace gl::gpu {

class shadow {
public:
	int width;
	int height;
	
	shadow() = default;
	
	/**
	 * \param w depth map width
	 * \param h depth map height
	 */
	shadow(int w, int h) : width(w), height(h) {}
	
	void init() {
		shadow_map.width = width;
		shadow_map.height = height;
		shadow_map.depth_test = true;
		shadow_map.stencil_test = false;
		shadow_map.buffer_readonly = false;
		shadow_map.preload(0, 1);
	}
	
	/**
	 * \param c camera info
	 * \param f framebuffer
	 */
	void shadow_mapping(const camera& c, shader& s, const scene& scene_info) {
		shadow_map.bind();
		framebuffer::clear();
		draw_scene(c, s, scene_info);
		framebuffer::unbind();
	}
	
private:
	framebuffer shadow_map;
};

}

#endif
