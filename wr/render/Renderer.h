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
#include <unordered_map>
#include "../resource/Image.h"
#include "../resource/Mesh.h"
#include "../resource/Material.h"
#include "../resource/SimpleMat.h"
#include "../graphics/Instance.h"
#include "../graphics/Camera.h"
#include "../graphics/Gpu.h"
#include "Scene.h"

namespace WR {

class Renderer {
public:
	/**
	 * Create a new renderer.
	 */
	Renderer() = default;
	
	/**
	 * Preload scene before render it.
	 *
	 * \param s scene
	 */
	void preload(const Scene& s);
	
	/**
	 * Clear render buffer.
	 */
	void clear();
	
	/**
	 * Render scene.
	 *
	 * \param s scene
	 * \param c camera information
	 */
	void render(const Scene& s, const Camera& c) const;
	
	/**
	 * Set parameters of preloaded textures.
	 *
	 * \param w wrapping type
	 * \param mag magnifying filter type
	 * \param min minifying filter type
	 * \param mip mipmap type
	 */
	void texture_parameter(int w = TEXTURE_REPEAT, int mag = TEXTURE_LINEAR,
						   int min = TEXTURE_NEAREST, int mip = TEXTURE_NO_MIPMAP);
	
private:
	std::unordered_map<const Mesh*, std::unique_ptr<Gpu::VertexObject[]> > mesh_buffer;
	std::unordered_map<const Image*, std::unique_ptr<Gpu::Texture> > image_buffer;
	
	void use_material(const Material& m, const Gpu::VertexObject& o,
                      const std::unordered_map<std::string, Mat<4, 4> >& k) const;
	
	static void material_depth_test(const Material& m);
	
	static void material_stencil_test(const Material& m);
	
	static void material_wireframe(const Material& m);
	
	static void material_side(const Material& m);
	
	static void material_shading(const Material& m);
	
	static void simple_shading(const SimpleMat& m);
};

}
