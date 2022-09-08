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

#include <memory>
#include "../core/math.h"
#include "../graphics/camera.h"
#include "../graphics/gpu.h"
#include "geom.h"

#pragma once

class ssao {
public:
	float radius = 0;			/* radius to search for occluders */
	float max_radius = 0;		/* maximum radius from occluder to shading point */
	float darkness = 0;			/* darkness of ambient occlusion */
	float bias = 0.5;			/* bias to avoid unrealistic effects */
	
	/**
	 * Init ssao.
	 */
	static void init();
	
	/**
	 * Create a new ssao.
	 */
	ssao() = default;
	
	/**
	 * Create a new ssao.
	 *
	 * \param r radius to search for occluders
	 * \param m maximum radius from occluder to shading point
	 * \param d darkness of ambient occlusion
	 * \param b bias to avoid unrealistic effects
	 */
	ssao(float r, float m, float d = .5, float b = .5);
	
	/**
	 * Preload ssao map.
	 */
	void preload();
	
	/**
	 * Render screen space ambient occlusion.
	 *
	 * \param c camera
	 * \param t target texture
	 * \param n normal texture
	 * \param d depth texture
	 */
	void render(const camera& c, const gpu::texture& t,
				const gpu::texture& n, const gpu::texture& d);
	
private:
	std::unique_ptr<gpu::framebuffer> ssao_buffer;
	std::unique_ptr<gpu::texture> ssao_map;
	
	static std::unique_ptr<gpu::shader> ssao_shader;
};
