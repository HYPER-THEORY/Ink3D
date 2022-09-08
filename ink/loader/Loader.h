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

#include "../objects/Image.h"
#include "../objects/Mesh.h"
#include "../objects/Material.h"

namespace Ink {

class Loader {
public:
	/**
	 * Load an image file into image.
	 *
	 * \param f the URL to the file
	 */
	static Image load_image(const std::string& f);
	
	/**
	 * Load an HDR image file into image.
	 *
	 * \param f the URL to the file
	 */
	static Image load_image_hdr(const std::string& f);
	
	/**
	 * Load an OBJ file into mesh array.
	 *
	 * \param f the URL to the file
	 * \param g grouping keyword
	 */
	static std::vector<Mesh> load_obj(const std::string& f, const std::string& g = "o");
	
	/**
	 * Load a MTL file into materials.
	 *
	 * \param f the URL to the file
	 */
	static std::vector<Material> load_mtl(const std::string& f);
};

}
