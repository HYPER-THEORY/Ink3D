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

#include "multibuffer.h"

multibuffer::multibuffer() {
	std::fill_n(textures, 32, nullptr);
}

void multibuffer::init() {
	default_shader = std::make_unique<gpu::shader>();
	default_shader->read("shaders/lib/multibuffer.vert.glsl",
						 "shaders/lib/multibuffer.frag.glsl");
}

void multibuffer::set_shader(const gpu::shader& s) {
	custom_shader = &s;
}

void multibuffer::set_texture(const gpu::texture& t, unsigned int i, const std::string& n) {
	textures[i] = &t;
	texture_names[i] = n;
}

void multibuffer::render() const {
	auto* available = custom_shader;
	if (!available) available = default_shader.get();
	available->use();
	auto& square = geom::square();
	square.attach(*available);
	for (int i = 0; i < 32; ++i) {
		if (textures[i] == nullptr) continue;
		std::string name = texture_names[i];
		if (name.empty()) name = "buffer" + std::to_string(i);
		available->uniform_int(texture_names[i], textures[i]->use(i));
	}
	square.draw();
}

std::unique_ptr<gpu::shader> multibuffer::default_shader;
