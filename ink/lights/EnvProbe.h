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

#include "IBLPrefilter.h"

namespace Ink {

class EnvProbe {
public:
	float intensity = 1;    /**< the intensity of environment probe */
	
	/**
	 * Create a new EnvProbe with its intensity.
	 *
	 * \param i the intensity of environment probe
	 */
	EnvProbe(float i = 1);
	
	/**
	 * Load a set of cube images to environment probe.
	 *
	 * \param i +X, -X, +Y, -Y, +Z, -Z images
	 * \param r the resolution of environment probe
	 */
	void load_cubemap(const Image* i, int r = 256);
	
	/**
	 * Load an equirectangular image to environment probe.
	 *
	 * \param i equirectangular image
	 * \param r the resolution of environment probe
	 */
	void load_equirect(const Image& i, int r = 256);
	
	/**
	 * Load a cube texture or 2D texture to environment probe.
	 *
	 * \param t texture
	 * \param r the resolution of environment probe
	 */
	void load_texture(const Gpu::Texture& t, int r = 256);
	
	/**
	 * Sets the texture of environment probe active.
	 *
	 * \param l the location of texture
	 */
	int activate(int l) const;
	
	/**
	 * Returns the resolution of environment probe.
	 */
	int get_resolution() const;
	
private:
	int resolution = 0;
	
	std::unique_ptr<Gpu::Texture> env_map;
};

}
