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

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>

#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

#include "../core/error.h"
#include "../math/maths.h"
#include "../objects/image.h"
#include "../objects/mesh.h"
#include "../objects/material.h"

class loader {
public:
	/**
	 * Init loader.
	 */
	static void init();
	
	/**
	 * Load an image file into image.
	 *
	 * \param f the URL to the file
	 * \param i target image
	 * \param s whether to swap red and blue color
	 */
	static int load_image(const std::string& f, image& i, bool s = true);
	
	/**
	 * Load an OBJ file into meshes.
	 *
	 * \param f the URL to the file
	 * \param m target meshes
	 */
	static int load_obj(const std::string& f, mesh* m);
	
	/**
	 * Load a MTL file into materials.
	 *
	 * \param f the URL to the file
	 * \param m target materials
	 */
	static int load_mtl(const std::string& f, material* m);
};
