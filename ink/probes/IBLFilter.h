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

#include "../graphics/Gpu.h"

#include <memory>

namespace ink {

class IBLFilter {
public:
	/**
	 * Loads the specified cube image set to the prefiltered radiance
	 * environment map.
	 *
	 * \param px right (+X) side of the cube image
	 * \param nx left  (-X) side of the cube image
	 * \param py upper (+Y) side of the cube image
	 * \param ny lower (-Y) side of the cube image
	 * \param pz front (+Z) side of the cube image
	 * \param nz back  (-Z) side of the cube image
	 * \param m prefiltered radiance environment map
	 * \param s size of the radiance environment map
	 */
	static void load_cubemap(const Image& px, const Image& nx,
							 const Image& py, const Image& ny,
							 const Image& pz, const Image& nz,
							 gpu::Texture& m, int s = 256);
	
	/**
	 * Loads the specified equirectangular image to the prefiltered radiance
	 * environment map.
	 *
	 * \param i equirectangular image
	 * \param m prefiltered radiance environment map
	 * \param s size of the radiance environment map
	 */
	static void load_equirect(const Image& i, gpu::Texture& m, int s = 256);
	
	/**
	 * Loads a cube texture or 2D texture to the prefiltered radiance
	 * environment map.
	 *
	 * \param t source texture
	 * \param m prefiltered radiance environment map
	 * \param s size of the radiance environment map
	 */
	static void load_texture(const gpu::Texture& t, gpu::Texture& m, int s = 256);
	
private:
	static std::unique_ptr<gpu::VertexObject> fullscreen_plane;
	
	static std::unique_ptr<gpu::Texture> blur_map;
	
	static std::unique_ptr<gpu::RenderTarget> cubemap_target;
	
	static std::unique_ptr<gpu::RenderTarget> blur_target;
	
	static void init_fullscreen_plane();
	
	static void gaussian_weights(float s, int n, float* w);
};

}
