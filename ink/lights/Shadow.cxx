/**
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

#include "Shadow.h"

namespace Ink {

void Shadow::activate() {
	if (assigner.empty()) assigner.emplace_back(0);
	unique_id = assigner.back();
	assigner.pop_back();
	if (assigner.empty()) assigner.emplace_back(unique_id + 1);
}

void Shadow::deactivate() {
	assigner.emplace_back(unique_id);
	unique_id = -1;
}

int Shadow::get_unique_id() const {
	return unique_id;
}

const Gpu::FrameBuffer* Shadow::get_target() const {
	shadow_target->set_depth_attachment(*shadow_map, 0, unique_id);
	return shadow_target.get();
}

void Shadow::init(int w, int h, int n) {
	resolution = Vec2(w, h);
	shadow_map = std::make_unique<Gpu::Texture>();
	shadow_map->init_2d_array(w, h, n, TEXTURE_D24_UNORM);
	shadow_map->set_wrap_all(TEXTURE_CLAMP_TO_BORDER);
	shadow_map->set_filters(TEXTURE_LINEAR, TEXTURE_NEAREST);
	shadow_map->set_border_color({1, 1, 1, 1});
	shadow_target = std::make_unique<Gpu::FrameBuffer>();
	shadow_target->disable_draw();
}

int Shadow::activate_texture(int l) {
	return shadow_map->activate(l);
}

Vec2 Shadow::get_resolution() {
	return resolution;
}

Vec2 Shadow::resolution;

std::vector<int> Shadow::assigner;

std::unique_ptr<Gpu::Texture> Shadow::shadow_map;

std::unique_ptr<Gpu::FrameBuffer> Shadow::shadow_target;

}
