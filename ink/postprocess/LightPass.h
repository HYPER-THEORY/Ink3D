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

#include "../scene/Scene.h"

#include "RenderPass.h"

namespace ink {

class LightPass : public RenderPass {
public:
	/**
	 * Creates a new LightPass object.
	 */
	LightPass() = default;
	
	/**
	 * Initializes the render pass and prepares the resources for rendering.
	 */
	void init() override;
	
	/**
	 * Compiles the required shaders and renders to the render target.
	 */
	void render() override;
	
	/**
	 * Returns the scene that represents the input of the render pass.
	 */
	const Scene* get_scene() const;
	
	/**
	 * Sets the specified scene as the input of the render pass.
	 *
	 * \param s scene
	 */
	void set_scene(const Scene* s);
	
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
	 * Returns the 2D texture that represents the base color buffer of G-Buffer.
	 */
	const gpu::Texture* get_texture_color() const;
	
	/**
	 * Sets the specified 2D texture as the base color buffer of G-Buffer.
	 *
	 * \param t base color texture
	 */
	void set_texture_color(const gpu::Texture* t);
	
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
	 * Returns the 2D texture that represents the material data buffer of
	 * G-Buffer.
	 */
	const gpu::Texture* get_texture_material() const;
	
	/**
	 * Sets the specified 2D texture as the material data buffer of G-Buffer.
	 *
	 * \param t material data texture
	 */
	void set_texture_material(const gpu::Texture* t);
	
	/**
	 * Returns the 2D texture that represents the indirect light buffer of
	 * G-Buffer.
	 */
	const gpu::Texture* get_texture_light() const;
	
	/**
	 * Sets the specified 2D texture as the indirect light buffer of G-Buffer.
	 *
	 * \param t indirect light texture
	 */
	void set_texture_light(const gpu::Texture* t);
	
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
	const Scene* scene = nullptr;
	
	const Camera* camera = nullptr;
	
	const gpu::Texture* g_color = nullptr;
	const gpu::Texture* g_normal = nullptr;
	const gpu::Texture* g_material = nullptr;
	const gpu::Texture* g_light = nullptr;
	const gpu::Texture* z_buffer = nullptr;
};

}
