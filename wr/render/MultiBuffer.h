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

class MultiBuffer {
public:
	/**
	 * Create a new multicanvas.
	 */
	MultiBuffer();
	
	/**
	 * Init multibuffer.
	 */
	static void init();
	
	/**
	 * Set custom shader to replace default shader.
	 *
	 * \param s custom shader
	 */
	void set_shader(const Gpu::Shader& s);
	
	/**
	 * Set texture with index.
	 *
	 * \param t texture
	 * \param i index of texture
	 * \param n name of texture
	 */
	void set_texture(const Gpu::Texture& t, unsigned int i, const std::string& n = "");
	
	/**
	 * Render multibuffer.
	 */
	void render() const;

private:
	const Gpu::Shader* custom_shader = nullptr;
	const Gpu::Texture* textures[32];
	std::string texture_names[32];
	
	static std::unique_ptr<Gpu::Shader> default_shader;
};

}
