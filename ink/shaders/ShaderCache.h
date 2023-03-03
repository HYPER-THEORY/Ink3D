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

#include "../graphics/Gpu.h"

#include <memory>

namespace Ink {

class ShaderCache {
public:
	/**
	 * Loads the specified string of vertex shader content to the shader.
	 *
	 * \param n shader name
	 * \param s vertex shader
	 */
	static void load_vert(const std::string& n, const char* s);
	
	/**
	 * Loads the specified string of vertex shader content to the shader.
	 *
	 * \param n shader name
	 * \param s vertex shader
	 */
	static void load_vert(const std::string& n, const std::string& s);
	
	/**
	 * Loads the specified string of geometry shader content to the shader.
	 *
	 * \param n shader name
	 * \param s geometry shader
	 */
	static void load_geom(const std::string& n, const char* s);
	
	/**
	 * Loads the specified string of geometry shader content to the shader.
	 *
	 * \param n shader name
	 * \param s geometry shader
	 */
	static void load_geom(const std::string& n, const std::string& s);
	
	/**
	 * Loads the specified string of fragment shader content to the shader.
	 *
	 * \param n shader name
	 * \param s fragment shader
	 */
	static void load_frag(const std::string& n, const char* s);
	
	/**
	 * Loads the specified string of fragment shader content to the shader.
	 *
	 * \param n shader name
	 * \param s fragment shader
	 */
	static void load_frag(const std::string& n, const std::string& s);
	
	/**
	 * Loads the vertex shader from the specified GLSL file to the shader.
	 *
	 * \param n shader name
	 * \param p the path to vertex shader file
	 */
	static void load_vert_file(const std::string& n, const std::string& p);
	
	/**
	 * Loads the geometry shader from the specified GLSL file to the shader.
	 *
	 * \param n shader name
	 * \param p the path to geometry shader file
	 */
	static void load_geom_file(const std::string& n, const std::string& p);
	
	/**
	 * Loads the fragment shader from the specified GLSL file to the shader.
	 *
	 * \param n shader name
	 * \param p the path to fragment shader file
	 */
	static void load_frag_file(const std::string& n, const std::string& p);
	
	/**
	 * Returns true if the vertex shader is loaded to the specified shader.
	 *
	 * \param n shader name
	 */
	static bool has_vert(const std::string& n);
	
	/**
	 * Returns true if the geometry shader is loaded to the specified shader.
	 *
	 * \param n shader name
	 */
	static bool has_geom(const std::string& n);
	
	/**
	 * Returns true if the fragment shader is loaded to the specified shader.
	 *
	 * \param n shader name
	 */
	static bool has_frag(const std::string& n);
	
	/**
	 * Returns the shader with the specified name from shader cache.
	 *
	 * \param n shader name
	 */
	static const Gpu::Shader* fetch(const std::string& n);
	
	/**
	 * Returns the shader with the specified name and defines from shader cache.
	 *
	 * \param n shader name
	 * \param d defines
	 */
	static const Gpu::Shader* fetch(const std::string& n, const Defines& d);
	
	/**
	 * Clears the shader cache with the specified name.
	 *
	 * \param n shader name
	 */
	static void clear_cache(const std::string& n);
	
	/**
	 * Clears the shader cache with the specified name and defines.
	 *
	 * \param n shader name
	 * \param d defines
	 */
	static void clear_cache(const std::string& n, const Defines& d);
	
	/**
	 * Clears all the shader values from the shader cache.
	 */
	static void clear_caches();
	
	/**
	 * Returns the path to find the included shaders.
	 */
	static std::string get_include_path();
	
	/**
	 * Sets the path to find the included shaders. Default is "ink/shaders/
	 * include/".
	 *
	 * \param p include path
	 */
	static void set_include_path(const std::string& p);
	
private:
	static void resolve_includes(std::string& s);
	
	static std::string include_path;
	
	static std::unordered_map<std::string, std::string> vert_shaders;
	static std::unordered_map<std::string, std::string> geom_shaders;
	static std::unordered_map<std::string, std::string> frag_shaders;
	
	static std::unordered_map<std::string, std::unique_ptr<Gpu::Shader> > cache;
};

}
