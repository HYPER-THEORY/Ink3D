/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

namespace ink {

class ReflectionProbe {
public:
	int resolution = 256;    /**< the resolution of the reflection probe */
	float intensity = 1;     /**< the intensity of the reflection probe */
	Vec3 position;           /**< the position of the reflection probe */
	
	/**
	 * Creates a new ReflectionProbe object and initializes it with intensity,
	 * resolution, and position.
	 *
	 * \param i the intensity of the reflection probe
	 * \param r the resolution of the reflection probe
	 * \param p the position of the reflection probe
	 */
	ReflectionProbe(float i = 1, int r = 256, const Vec3& p = {0, 0, 0});
	
	/**
	 * Loads the specified cube image set to the reflection probe.
	 *
	 * \param px right (+X) side of the cube image
	 * \param nx left  (-X) side of the cube image
	 * \param py upper (+Y) side of the cube image
	 * \param ny lower (-Y) side of the cube image
	 * \param pz front (+Z) side of the cube image
	 * \param nz back  (-Z) side of the cube image
	 */
	void load_cubemap(const Image& px, const Image& nx,
					  const Image& py, const Image& ny,
					  const Image& pz, const Image& nz);
	
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
	void load_texture(const gpu::Texture& t);
	
	/**
	 * Activates the texture at the specified location of reflection probe.
	 *
	 * \param l the location of the texture
	 */
	int activate(int l) const;
	
private:
	std::unique_ptr<gpu::Texture> reflection_map;
};

}
