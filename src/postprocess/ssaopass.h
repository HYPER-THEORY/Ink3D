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

#include "../math/maths.h"
#include "../camera/camera.h"
#include "../graphics/gpu.h"
#include "../renderer/renderpass.h"

#pragma once

/* ssao quality */
enum {
	SSAO_LOW,
	SSAO_HIGH,
};

class ssaopass {
public:
	int quality = SSAO_HIGH;		/* the quality of ssao */
	
	int width = 0;					/* the width of screen */
	int height = 0;					/* the height of screen */
	float radius = 0;				/* radius to search for occluders */
	float max_radius = 0;			/* the maximum radius from occluder to shading point */
	float darkness = 0;				/* the darkness of ambient occlusion, range is [0, 1] */
	float bias = .2;				/* bias to avoid unrealistic effects */
	
	/**
	 * Init ssaopass.
	 */
	static void init();
	
	/**
	 * Create a new ssaopass.
	 *
	 * \param w the width of screen
	 * \param h the height of screen
	 * \param r radius to search for occluders
	 * \param m the maximum radius from occluder to shading point
	 * \param d the darkness of ambient occlusion, range is [0, 1]
	 */
	ssaopass(int w = 0, int h = 0, float r = 0, float m = 0, float d = .5);
	
	/**
	 * Preload ssaopass.
	 */
	void preload();
	
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
	 * Render a texture with ambient occlusion factors.
	 *
	 * \param c camera
	 */
	void render(const camera& c) const;
	
private:
	const gpu::texture* normal_map = nullptr;
	const gpu::texture* depth_map = nullptr;
	const gpu::framebuffer* target = nullptr;
	
	std::unique_ptr<gpu::texture> ssao_map;
	std::unique_ptr<gpu::framebuffer> ssao_buffer;
	
	static std::unique_ptr<gpu::shader> ssao_shader;
	static std::unique_ptr<gpu::shader> blur_shader;
};
