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
#include "../core/math.h"
#include "image.h"
#include "mesh.h"
#include "material.h"
#include "simplemat.h"

class loader {
public:
	/**
	 * Init loader.
	 */
	static void init();
	
	/**
	 * Load image data from file.
	 *
	 * \param f image file url
	 * \param i target image
	 * \param s whether to swap red and blue color
	 */
	static int load_image(const std::string& f, image& i, bool s = true);
	
	/**
	 * Load model data from obj file.
	 *
	 * \param f obj file url
	 * \param m target meshs
	 */
	static int load_obj(const std::string& f, mesh* m);
	
	/**
	 * Load material data from mtl file.
	 *
	 * \param f mtl file url
	 * \param m target materials
	 */
	static int load_mtl(const std::string& f, simplemat* m);
};
