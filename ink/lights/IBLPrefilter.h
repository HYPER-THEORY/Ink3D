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

#include "../graphics/Gpu.h"

namespace Ink {

class IBLPrefilter {
public:
	/**
	 * Initialize IBLPrefilter.
	 */
	static void init();
	
	/**
	 * Load a set of cube images to prefiltered radiance environment map.
	 *
	 * \param px right (+X) face of cube image
	 * \param nx left  (-X) face of cube image
	 * \param py upper (+Y) face of cube image
	 * \param ny lower (-Y) face of cube image
	 * \param pz front (+Z) face of cube image
	 * \param nz back  (-Z) face of cube image
	 * \param m prefiltered radiance environment map
	 * \param s the size of radiance environment map
	 */
	static void load_cubemap(const Image& px, const Image& nx, const Image& py,
							 const Image& ny, const Image& pz, const Image& nz,
							 Gpu::Texture& m, int s = 256);
	
	/**
	 * Load an equirectangular image to prefiltered radiance environment map.
	 *
	 * \param i equirectangular image
	 * \param m prefiltered radiance environment map
	 * \param s the size of radiance environment map
	 */
	static void load_equirect(const Image& i, Gpu::Texture& m, int s = 256);
	
	/**
	 * Load a cube texture or 2D texture to prefiltered radiance environment map.
	 *
	 * \param t source texture
	 * \param m prefiltered radiance environment map
	 * \param s the size of radiance environment map
	 */
	static void load_texture(const Gpu::Texture& t, Gpu::Texture& m, int s = 256);
	
private:
	static std::unique_ptr<Gpu::VertexObject> plane;
	
	static std::unique_ptr<Gpu::Texture> blur_map;
	
	static std::unique_ptr<Gpu::FrameBuffer> cubemap_target;
	static std::unique_ptr<Gpu::FrameBuffer> blur_target;
	
	static std::unique_ptr<Gpu::Shader> cubemap_shader;
	static std::unique_ptr<Gpu::Shader> blur_shader;
	
	static void get_weights(float s, int n, float* w);
};

}
