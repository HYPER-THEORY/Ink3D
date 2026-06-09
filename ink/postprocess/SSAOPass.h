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

#include "RenderPass.h"

#include "../camera/Camera.h"

namespace ink {

class SSAOPass : public RenderPass {
public:
	int width = 0;            /**< the width of the screen */
	int height = 0;           /**< the height of the screen */
	int samples = 32;         /**< sample number, must be 16, 32 or 64 */
	float radius = 0;         /**< radius to search for occluders */
	float max_radius = 0;     /**< the maximum radius from occluders to the pixel */
	float max_z = 100;        /**< the maximum depth to render ambient occlusion */
	float intensity = 1;      /**< the intensity of ambient occlusion, range is 0 to 1 */
	
	/**
	 * Creates a new SSAOPass (Screen Space Ambient Occlusion) object.
	 */
	SSAOPass() = default;
	
	/**
	 * Creates a new SSAOPass (Screen Space Ambient Occlusion) object and
	 * initializes it with the specified parameters.
	 *
	 * \param w the width of the screen
	 * \param h the height of the screen
	 * \param r radius to search for occluders
	 * \param m the maximum radius from occluders to the pixel
	 * \param i the intensity of ambient occlusion, range is 0 to 1
	 */
	SSAOPass(int w, int h, float r, float m, float i = 1);
	
	/**
	 * Initializes the render pass and prepares the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() override;
	
	/**
	 * Returns the camera that represents the input of the render pass.
	 */
	const Camera* get_camera() const;
	
	/**
	 * Sets the specified camera as the input of the render pass.
	 *
	 * \param c camera
	 */
	void set_camera(const Camera* c);
	
	/**
	 * Returns the 2D texture that represents the input of the render pass.
	 */
	const gpu::Texture* get_texture() const;
	
	/**
	 * Sets the specified 2D texture as the input of the render pass.
	 *
	 * \param t input texture
	 */
	void set_texture(const gpu::Texture* t);
	
	/**
	 * Returns the 2D texture that represents the world normal buffer of
	 * G-Buffer.
	 */
	const gpu::Texture* get_texture_normal() const;
	
	/**
	 * Sets the specified 2D texture as the world normal buffer of G-Buffer.
	 *
	 * \param t world normal texture
	 */
	void set_texture_normal(const gpu::Texture* t);
	
	/**
	 * Returns the 2D texture that represents the depth map.
	 */
	const gpu::Texture* get_texture_depth() const;
	
	/**
	 * Sets the specified 2D texture as the depth map (should be set to linear
	 * filtering).
	 *
	 * \param t depth texture
	 */
	void set_texture_depth(const gpu::Texture* t);
	
private:
	const Camera* camera = nullptr;
	
	const gpu::Texture* map = nullptr;
	const gpu::Texture* g_normal = nullptr;
	const gpu::Texture* z_buffer = nullptr;
	
	std::unique_ptr<gpu::Texture> blur_map_1;
	std::unique_ptr<gpu::Texture> blur_map_2;
	
	std::unique_ptr<gpu::RenderTarget> blur_target_1;
	std::unique_ptr<gpu::RenderTarget> blur_target_2;
};

}
