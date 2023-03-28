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

#include "Material.h"

namespace Ink {

Material::Material(const std::string& n) : name(n) {
	custom_maps[0]  = nullptr;
	custom_maps[1]  = nullptr;
	custom_maps[2]  = nullptr;
	custom_maps[3]  = nullptr;
	custom_maps[4]  = nullptr;
	custom_maps[5]  = nullptr;
	custom_maps[6]  = nullptr;
	custom_maps[7]  = nullptr;
	custom_maps[8]  = nullptr;
	custom_maps[9]  = nullptr;
	custom_maps[10] = nullptr;
	custom_maps[11] = nullptr;
	custom_maps[12] = nullptr;
	custom_maps[13] = nullptr;
	custom_maps[14] = nullptr;
	custom_maps[15] = nullptr;
	side            = FRONT_SIDE;
	shadow_side     = BACK_SIDE;
	depth_func      = FUNC_LEQUAL;
	stencil_func    = FUNC_ALWAYS;
	stencil_fail    = STENCIL_KEEP;
	stencil_zfail   = STENCIL_KEEP;
	stencil_zpass   = STENCIL_KEEP;
	blend_op_rgb    = BLEND_ADD;
	blend_op_alpha  = BLEND_ADD;
	blend_src_rgb   = FACTOR_SRC_ALPHA;
	blend_src_alpha = FACTOR_SRC_ALPHA;
	blend_dst_rgb   = FACTOR_ONE_MINUS_SRC_ALPHA;
	blend_dst_alpha = FACTOR_ONE_MINUS_SRC_ALPHA;
}

}
