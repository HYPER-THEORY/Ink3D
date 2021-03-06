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
#include "../graphics/gpu.h"

class renderpass {
public:
	/**
	 * Init renderpass.
	 */
	static void init();
	
	/**
	 * Returns the current viewport.
	 */
	static gpu::rect get_viewport();
	
	/**
	 * Sets the viewport.
	 *
	 * \param v viewport
	 */
	static void set_viewport(const gpu::rect& v);
	
	/**
	 * Determines whether to enable scissor test.
	 */
	static bool get_scissor_test();
	
	/**
	 * Determines whether to enable scissor test.
	 *
	 * \param t enable scissor test
	 */
	static void set_scissor_test(bool t);
	
	/**
	 * Returns the current scissor region.
	 */
	static gpu::rect get_scissor();
	
	/**
	 * Sets the scissor region.
	 *
	 * \param s scissor region
	 */
	static void set_scissor(const gpu::rect& s);
	
	/**
	 * Render full screen to the specified framebuffer.
	 *
	 * \param t target framebuffer
	 * \param s shader
	 */
	static void render(const gpu::shader* s, const gpu::framebuffer* t = nullptr);

private:
	static bool scissor_test;
	static gpu::rect scissor;
	static gpu::rect viewport;
	static std::unique_ptr<gpu::vertexobject> quad;
};
