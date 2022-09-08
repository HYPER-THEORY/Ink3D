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
#include "../resource/image.h"
#include "../resource/mesh.h"
#include "../graphics/gpu.h"
#include "geom.h"

class multibuffer {
public:
	/**
	 * Create a new multicanvas.
	 */
	multibuffer();
	
	/**
	 * Init multibuffer.
	 */
	static void init();
	
	/**
	 * Set custom shader to replace default shader.
	 *
	 * \param s custom shader
	 */
	void set_shader(const gpu::shader& s);
	
	/**
	 * Set texture with index.
	 *
	 * \param t texture
	 * \param i index of texture
	 * \param n name of texture
	 */
	void set_texture(const gpu::texture& t, unsigned int i, const std::string& n = "");
	
	/**
	 * Render multibuffer.
	 */
	void render() const;

private:
	const gpu::shader* custom_shader = nullptr;
	const gpu::texture* textures[32];
	std::string texture_names[32];
	
	static std::unique_ptr<gpu::shader> default_shader;
};
