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
#include "../graphics/Gpu.h"
#include "../graphics/Camera.h"
#include "Geom.h"
#include "Scene.h"
#include "Renderer.h"

namespace WR {

enum ShadowType {
	SHADOW_HARD,
	SHADOW_PCF,
	SHADOW_PCSS,
};

class Shadow {
public:
	float darkness = .5;		/* darkness of shadow */
	float bias = .0001;			/* shadow map bias */
	float normal_bias = 0;		/* shadow map bias along the object normal */
	int resolution = 1024;		/* shadow map resolution */
	
	int type = SHADOW_PCF;		/* type of shadow */
	int radius = 3;				/* radius of blurred edge in pcf algorithm */
	int weight = 1;				/* weight of light in pcss algorithm */
	
	Camera light_view;			/* light view of the world */
	
	/**
	 * Create a new shadow.
	 *
	 * \param r shadow map resolution
	 * \param d darkness of shadow
	 * \param b shadow map bias
	 * \param nb shadow map bias along the object normal
	 */
	Shadow(int r = 1024, float d = .5, float b = .0001, float nb = 0);
	
	/**
	 * Init shadow.
	 */
	static void init();
	
	/**
	 * Set light view of the world.
	 *
	 * \param v light view
	 */
	void set_light_view(const Camera& v);
	
	/**
	 * Preload shdow before shadow mapping.
	 */
	void preload();
	
	/**
	 * Create shadow map.
	 *
	 * \param r renderer
	 * \param s scene infomation
	 */
	void create(const Renderer& r, const Scene& s) const;
	
	/**
	 * Render shadow to target texture in screen space.
	 *
	 * \param c camera
	 * \param t target texture
	 * \param n normal texture
	 * \param d depth texture
	 */
	void render(const Camera& c, const Gpu::Texture& t,
                const Gpu::Texture& n, const Gpu::Texture& d);
	
private:
	std::unique_ptr<Gpu::FrameBuffer> shadow_buffer;
	std::unique_ptr<Gpu::Texture> discard_map;
	std::unique_ptr<Gpu::Texture> shadow_map;
	
	static std::unique_ptr<Gpu::Shader> shadow_shader;
};

}
