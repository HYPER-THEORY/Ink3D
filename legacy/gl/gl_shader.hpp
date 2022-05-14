/*
 * Copyright (C) 2021-2022 Hypertheory
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include "gl_stdinc.hpp"

namespace gl::gpu {

inline constexpr const char* cubemap_vertex_shader = R"(
	#version 410 core

	uniform mat4 view_proj;

	in vec3 vertex;

	out vec3 tex_coord;

	void main() {
		tex_coord = vertex;
		gl_Position = (view_proj * vec4(vertex, 1)).xyww;
	}
)";

inline constexpr const char* cubemap_fragment_shader = R"(
	#version 410 core

	uniform samplerCube box;

	in vec3 tex_coord;

	out vec4 out_color;

	void main() {
		out_color = texture(box, tex_coord);
	}
)";

inline constexpr const char* canvas_vertex_shader = R"(
	#version 410 core

	in vec2 vertex;
	
	out vec2 tex_coord;

	void main() {
		tex_coord = max(vertex, 0);
		gl_Position = vec4(vertex, 0, 1);
	}
)";

inline constexpr const char* canvas_fragment_shader = R"(
	#version 410 core

	uniform sampler2D canvas;

	in vec2 tex_coord;

	out vec4 out_color;

	void main() {
		out_color = texture(canvas, tex_coord);
	}
)";

inline constexpr const char* framebuffer_vertex_shader = R"(
	#version 410 core

	in vec2 vertex;
	
	out vec2 tex_coord;

	void main() {
		tex_coord = max(vertex, 0);
		gl_Position = vec4(vertex, 0, 1);
	}
)";

inline constexpr const char* framebuffer_fragment_shader = R"(
	#version 410 core

	uniform sampler2D framebuffer;

	in vec2 tex_coord;

	out vec4 out_color;

	void main() {
		out_color = texture(framebuffer, tex_coord);
	}
)";

}

#endif
