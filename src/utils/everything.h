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

/* core */
#include "../core/error.h"
#include "../core/file.h"

/* math */
#include "../math/maths.h"

/* objects */
#include "../objects/image.h"
#include "../objects/mesh.h"
#include "../objects/instance.h"
#include "../objects/scene.h"
#include "../objects/material.h"

/* loader */
#include "../loader/loader.h"

/* camera */
#include "../camera/camera.h"
#include "../camera/perspcamera.h"
#include "../camera/orthocamera.h"

/* graphics */
#include "../graphics/software.h"
#include "../graphics/gpu.h"

/* geoms */
#include "../geoms/cuboid.h"
#include "../geoms/rectangle.h"

/* render */
#include "../render/renderer.h"
#include "../render/cubemap.h"
#include "../render/renderpass.h"
#include "../render/copypass.h"
#include "../render/blendpass.h"
#include "../render/shadowpass.h"
#include "../render/ssaopass.h"
#include "../render/fxaapass.h"

/* audio */
#include "../audio/audio.h"

/* physics */
#include "../physics/physics.h"

/* window */
#include "../window/window.h"

/* convex */
#include "../convex/convexhull.h"
