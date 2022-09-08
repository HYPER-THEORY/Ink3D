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
#include "../core/math.h"
#include "../core/file.h"
#include "../core/string.h"

/* resource */
#include "../resource/image.h"
#include "../resource/mesh.h"
#include "../resource/material.h"
#include "../resource/simplemat.h"
#include "../resource/loader.h"

/* graphics */
#include "../graphics/instance.h"
#include "../graphics/camera.h"
#include "../graphics/gpu.h"
#include "../graphics/software.h"

/* renderer */
#include "../render/geom.h"
#include "../render/scene.h"
#include "../render/renderer.h"
#include "../render/cubemap.h"
#include "../render/multibuffer.h"
#include "../render/shadow.h"
#include "../render/ssao.h"

/* audio */
#include "../audio/audio.h"

/* window */
#include "../window/window.h"

/* physics */
#include "../physics/physics.h"

/**
 * Init every libraries.
 */
void init_everything();
