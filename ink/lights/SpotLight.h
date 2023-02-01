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

#include "../math/Constants.h"
#include "../camera/PerspCamera.h"

#include "Light.h"
#include "Shadow.h"

namespace Ink {

class SpotLight : public Light {
public:
	Vec3 position;               /**< the position where the light casts from */
	Vec3 direction;              /**< the direction where the light casts through */
	float angle = PI / 3;        /**< the maximum angle of light */
	float decay = 1;             /**< the decay speed of light */
	float distance = 0;          /**< the maximum distance affected by the light */
	float penumbra = 0;          /**< the attenuation of spot light cone */
	bool cast_shadow = false;    /**< whether the light will cast shadow */
	Shadow shadow;               /**< the shadow of light */
	
	/**
	 * Creates a new SpotLight object and initializes it with color and
	 * intensity.
	 *
	 * \param c the color of light
	 * \param i the intensity of light
	 */
	explicit SpotLight(const Vec3& c = {1, 1, 1}, float i = 1);
};

}
