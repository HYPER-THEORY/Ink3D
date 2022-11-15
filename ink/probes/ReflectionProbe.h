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

#include "IBLFilter.h"

namespace Ink {

class ReflectionProbe {
public:
	int resolution = 256;    /**< the resolution of reflection probe */
	float intensity = 1;     /**< the intensity of reflection probe */
	
	/**
	 * Creates a new ReflectionProbe and initializes it with intensity and
	 * resolution.
	 *
	 * \param i the intensity of reflection probe
	 * \param r the resolution of reflection probe
	 */
	explicit ReflectionProbe(float i = 1, int r = 256);
	
	/**
	 * Loads a set of specified cube images to the reflection probe.
	 *
	 * \param px right (+X) side of cube image
	 * \param nx left  (-X) side of cube image
	 * \param py upper (+Y) side of cube image
	 * \param ny lower (-Y) side of cube image
	 * \param pz front (+Z) side of cube image
	 * \param nz back  (-Z) side of cube image
	 */
	void load_cubemap(const Image& px, const Image& nx, const Image& py,
					  const Image& ny, const Image& pz, const Image& nz);
	
	/**
	 * Loads the specified equirectangular image to the reflection probe.
	 *
	 * \param i equirectangular image
	 */
	void load_equirect(const Image& i);
	
	/**
	 * Loads a specified cube texture or 2D texture to the reflection probe.
	 *
	 * \param t cube texture
	 */
	void load_texture(const Gpu::Texture& t);
	
	/**
	 * Activates the texture at the specified location of reflection probe.
	 *
	 * \param l the location of texture
	 */
	int activate(int l) const;
	
private:
	std::unique_ptr<Gpu::Texture> reflection_map;
};

}
