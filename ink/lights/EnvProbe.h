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
	int resolution = 256;    /**< the resolution of environment probe */
	float intensity = 1;     /**< the intensity of environment probe */
	
	/**
	 * Create a new EnvProbe with intensity and resolution.
	 *
	 * \param i the intensity of environment probe
	 * \param r the resolution of environment probe
	 */
	EnvProbe(float i = 1, int r = 256);
	
	/**
	 * Load a set of cube images to environment probe.
	 *
	 * \param px right (+X) face of cube image
	 * \param nx left  (-X) face of cube image
	 * \param py upper (+Y) face of cube image
	 * \param ny lower (-Y) face of cube image
	 * \param pz front (+Z) face of cube image
	 * \param nz back  (-Z) face of cube image
	 */
	void load_cubemap(const Image& px, const Image& nx, const Image& py,
					  const Image& ny, const Image& pz, const Image& nz);
	
	/**
	 * Load an equirectangular image to environment probe.
	 *
	 * \param i equirectangular image
	 */
	void load_equirect(const Image& i);
	
	/**
	 * Load a cube texture or 2D texture to environment probe.
	 *
	 * \param t cube texture
	 */
	void load_texture(const Gpu::Texture& t);
	
	/**
	 * Sets the texture of environment probe active.
	 *
	 * \param l the location of texture
	 */
	int activate(int l) const;
	
private:
	std::unique_ptr<Gpu::Texture> env_map;
};

}
