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

#pragma once

#include <memory>

#include "../camera/Camera.h"
#include "../graphics/Gpu.h"

namespace Ink {

enum ShadowType {
	SHADOW_HARD,
	SHADOW_PCF,
	SHADOW_PCSS,
};

class Shadow {
public:
	int type = SHADOW_PCF;    /**< the type of shadow */
	float bias = 0;           /**< shadow bias, should be close to 0 */
	float normal_bias = 0;    /**< shadow bias along the object normal, range is 0 to 1 */
	float radius = 3;         /**< the radius of blurred edge, only used in PCF or PCSS */
	Camera camera;            /**< light's view camera of shadow */
	
	/**
	 * Create a new Shadow.
	 */
	Shadow() = default;
	
	/**
	 * Activate shadow and assign an unique id for it.
	 */
	void activate();
	
	/**
	 * Deactivate shadow and collect the unique id.
	 */
	void deactivate();
	
	/**
	 * Returns the unique id of shadow.
	 */
	int get_unique_id() const;
	
	/**
	 * Returns the render target of shadow map.
	 */
	const Gpu::FrameBuffer* get_target() const;
	
	/**
	 * Sets the resolution of shadow map and the number of max shadows.
	 * Initialize shadows.
	 *
	 * \param w the width of shadow map
	 * \param h the height of shadow map
	 * \param n the number of max shadows
	 */
	static void init(int w = 1024, int h = 1024, int n = 16);
	
	/**
	 * Sets the texture of shadow maps active.
	 *
	 * \param l the location of texture
	 */
	static int activate_texture(int l);
	
	/**
	 * Returns the resolution of shadow map.
	 */
	static Vec2 get_resolution();
	
private:
	int unique_id = -1;
	
	static Vec2 resolution;
	
	static std::vector<int> assigner;
	
	static std::unique_ptr<Gpu::Texture> shadow_map;
	
	static std::unique_ptr<Gpu::FrameBuffer> shadow_target;
	
	Shadow(const Shadow&) = delete;
};

}
