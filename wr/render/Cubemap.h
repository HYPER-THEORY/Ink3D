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
#include "../resource/Image.h"
#include "../resource/Mesh.h"
#include "../graphics/Gpu.h"
#include "Geom.h"

namespace WR {

class Cubemap {
public:
	/**
	 * Create a new cubemap.
	 */
	Cubemap() = default;
	
	/**
	 * Init cubemap.
	 */
	static void init();
	
	/**
	 * Preload cubemap before rendering.
	 *
	 * \param is left, right, upper, lower, front, back images of cube
	 */
	void preload(const Image* is);
	
	/**
	 * Set custom shader to replace default shader.
	 *
	 * \param s custom shader
	 */
	void set_shader(const Gpu::Shader& s);
	
	/**
	 * Render cubemap.
	 *
	 * \param c camera infomation
	 */
	void render(const Camera& c) const;

private:
	const Gpu::Shader* custom_shader = nullptr;
	std::unique_ptr<Gpu::Texture> texture_cube;
	
	static std::unique_ptr<Gpu::Shader> default_shader;
};

}
