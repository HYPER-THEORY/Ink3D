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

#include "RenderPass.h"

#include "../camera/Camera.h"

namespace Ink {

class SSAOPass : public RenderPass {
public:
	int width = 0;            /**< the width of screen */
	int height = 0;           /**< the height of screen */
	int samples = 32;         /**< sample number, must be 16, 32 or 64 */
	float radius = 0;         /**< radius to search for occluders */
	float max_radius = 0;     /**< the maximum radius from occluder to pixel */
	float max_z = 100;        /**< the maximum depth to render ambient occlusion */
	float intensity = 1;      /**< the intensity of ambient occlusion, range is 0 to 1 */
	
	/**
	 * Creates a new SSAOPass (Screen Space Ambient Occlusion) object.
	 */
	explicit SSAOPass() = default;
	
	/**
	 * Creates a new SSAOPass (Screen Space Ambient Occlusion) object and
	 * initializes it with the specified parameters.
	 *
	 * \param w the width of screen
	 * \param h the height of screen
	 * \param r radius to search for occluders
	 * \param m the maximum radius from occluder to shading point
	 * \param i the intensity of ambient occlusion, range is 0 to 1
	 */
	explicit SSAOPass(int w, int h, float r, float m, float i = 1);
	
	/**
	 * Initializes the render pass and prepares the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() override;
	
	/**
	 * Returns the camera represents the input of rendering pass.
	 */
	const Camera* get_camera() const;
	
	/**
	 * Sets the specified camera as the input of rendering pass.
	 *
	 * \param c camera
	 */
	void set_camera(const Camera* c);
	
	/**
	 * Returns the 2D texture represents the input of rendering pass.
	 */
	const Gpu::Texture* get_texture() const;
	
	/**
	 * Sets the specified 2D texture as the input of rendering pass.
	 *
	 * \param t input texture
	 */
	void set_texture(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the world normal buffer in G-Buffers.
	 */
	const Gpu::Texture* get_texture_normal() const;
	
	/**
	 * Sets the specified 2D texture as the world normal buffer in G-Buffers.
	 *
	 * \param t world normal texture
	 */
	void set_texture_normal(const Gpu::Texture* t);
	
	/**
	 * Returns the 2D texture represents the depth buffer / Z-Buffer.
	 */
	const Gpu::Texture* get_texture_depth() const;
	
	/**
	 * Sets the specified 2D texture as the depth buffer / Z-Buffer.
	 *
	 * \param t depth texture
	 */
	void set_texture_depth(const Gpu::Texture* t);
	
private:
	const Camera* camera = nullptr;
	
	const Gpu::Texture* map = nullptr;
	const Gpu::Texture* g_normal = nullptr;
	const Gpu::Texture* z_map = nullptr;
	
	std::unique_ptr<Gpu::Texture> blur_map_1;
	std::unique_ptr<Gpu::Texture> blur_map_2;
	
	std::unique_ptr<Gpu::RenderTarget> blur_target_1;
	std::unique_ptr<Gpu::RenderTarget> blur_target_2;
};

}
