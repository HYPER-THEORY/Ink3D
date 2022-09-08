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

#include "cubemap.h"

void cubemap::init() {
	default_shader = std::make_unique<gpu::shader>();
	default_shader->read("shaders/lib/cubemap.vert.glsl",
						 "shaders/lib/cubemap.frag.glsl");
}

void cubemap::preload(const image* is) {
	texture_cube = std::make_unique<gpu::texture>();
	texture_cube->preload_cube(is);
	texture_cube->set_parameters(TEXTURE_CLAMP_TO_EDGE);
}

void cubemap::set_shader(const gpu::shader& s) {
	custom_shader = &s;
}

void cubemap::render(const camera& c) const {
	mat<4, 4> viewing = c.viewing;
	viewing[0][3] = 0;
	viewing[1][3] = 0;
	viewing[2][3] = 0;
	auto* available = custom_shader;
	if (!available) available = default_shader.get();
	available->use();
	auto& cube = geom::cube();
	cube.attach(*available);
	available->uniform_mat4("view_proj", c.projection * viewing);
	available->uniform_int("cubemap", texture_cube->use(0));
	gpu::depth_func(FUNC_LEQUAL);
	cube.draw();
	gpu::depth_func(FUNC_LESS);
}

std::unique_ptr<gpu::shader> cubemap::default_shader;
