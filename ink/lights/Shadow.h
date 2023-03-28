/**
 * Copyright (C) 2021-2023 Hypertheory
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
	ShadowType type = SHADOW_PCF;    /**< the type of shadow */
	int map_id = 0;                  /**< the ID of shadow map */
	float bias = 0;                  /**< shadow bias, should be close to 0 */
	float normal_bias = 0;           /**< shadow bias along the object normal, range is 0 to 1 */
	float radius = 3;                /**< the radius of blurred edge, only used in PCF or PCSS */
	Camera camera;                   /**< light's view camera of shadow */
	
	/**
	 * Sets the resolution of shadow map and the max number of shadows.
	 * Initializes shadows.
	 *
	 * \param w the width of shadow map
	 * \param h the height of shadow map
	 * \param n the max number of shadows
	 */
	static void init(int w = 1024, int h = 1024, int n = 16);
	
	/**
	 * Creates a new Shadow object.
	 */
	explicit Shadow() = default;
	
	/**
	 * The copy constructor is deleted.
	 */
	Shadow(const Shadow&) = delete;
	
	/**
	 * Returns the sample numbers when using PCF / PCSS shadow.
	 */
	static int get_samples();
	
	/**
	 * Sets the sample numbers when using PCF / PCSS shadow. Must be 16, 32 or
	 * 64. Default is 32.
	 *
	 * \param s sample numbers
	 */
	static void set_samples(int s);
	
	/**
	 * Returns the resolution of shadow texture (shadow map).
	 */
	static Vec2 get_resolution();
	
	/**
	 * Activates the shadow texture (shadow map) at the specified location.
	 *
	 * \param l the location of texture
	 */
	static int activate_texture(int l);
	
	/**
	 * Returns the render target of the shadow texture (shadow map).
	 */
	const Gpu::RenderTarget* get_target() const;
	
private:
	static int samples;
	
	static Vec2 resolution;
	
	static std::unique_ptr<Gpu::Texture> shadow_map;
	
	static std::unique_ptr<Gpu::RenderTarget> shadow_target;
};

}
