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
#include "../objects/scene.h"
#include "../camera/camera.h"
#include "../graphics/gpu.h"
#include "../renderer/renderer.h"
#include "../renderer/renderpass.h"

/* shadow type */
enum {
	SHADOW_HARD,
	SHADOW_PCF,
	SHADOW_PCSS,
};

class shadowpass {
public:
	int type = SHADOW_PCF;		/* the type of shadow */
	
	int width = 1024;			/* the width of shadow map */
	int height = 1024;			/* the height of shadow map */
	float darkness = .5;		/* the darkness of shadow, range is [0, 1] */
	float bias = 0;				/* shadow map bias, close to 0 */
	float normal_bias = .5;		/* shadow map bias along the object normal, range is [0, 1] */
	int radius = 3;				/* the radius of blurred edge, in PCF shadow */
	int light_weight = 1;		/* the weight of light, in PCSS shadow */
	camera light_view;			/* the camera of light's view */
	
	/**
	 * Init shadowpass.
	 */
	static void init();
	
	/**
	 * Create a new shadowpass.
	 *
	 * \param w the width of shadow map
	 * \param h the height of shadow map
	 * \param d the darkness of shadow, range is [0, 1]
	 */
	shadowpass(int w = 1024, int h = 1024, float d = .5);
	
	/**
	 * Returns the camera of light's view.
	 */
	camera get_light_view() const;
	
	/**
	 * Sets the camera of light's view.
	 *
	 * \param v light's view
	 */
	void set_light_view(const camera& v);
	
	/**
	 * Preload shadowpass before creating shadow map.
	 */
	void preload();
	
	/**
	 * Create a depth map using the camera of light's view.
	 *
	 * \param r renderer
	 * \param s scene infomation
	 */
	void create(renderer& r, const scene& s) const;
	
	/**
	 * Returns the current target framebuffer if there are, returns nullptr
	 * otherwise. 
	 */
	const gpu::framebuffer* get_target() const;
	
	/**
	 * Sets the target framebuffer. When nullptr is given, the default
	 * framebuffer is set as the target framebuffer.
	 *
	 * \param t target framebuffer
	 */
	void set_target(const gpu::framebuffer* t);
	
	/**
	 * Returns the normal map of scene.
	 */
	const gpu::texture* get_normal() const;
	
	/**
	 * Sets the normal map of scene.
	 *
	 * \param n normal map of scene
	 */
	void set_normal(const gpu::texture* n);
	
	/**
	 * Returns the depth map of scene.
	 */
	const gpu::texture* get_depth() const;
	
	/**
	 * Sets the depth map of scene.
	 *
	 * \param d depth map of scene
	 */
	void set_depth(const gpu::texture* d);
	
	/**
	 * Render a texture with shadow factors.
	 *
	 * \param c camera
	 */
	void render(const camera& c) const;
	
private:
	const gpu::texture* normal_map = nullptr;
	const gpu::texture* depth_map = nullptr;
	const gpu::framebuffer* target = nullptr;
	
	std::unique_ptr<gpu::texture> shadow_map;
	std::unique_ptr<gpu::framebuffer> shadow_buffer;
	
	static std::unique_ptr<gpu::shader> shadow_shader;
};
