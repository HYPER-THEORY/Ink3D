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
#include "../core/Error.h"
#include "../core/Math.h"
#include "../core/File.h"
#include "../core/String.h"

/* resource */
#include "../resource/Image.h"
#include "../resource/Mesh.h"
#include "../resource/Material.h"
#include "../resource/SimpleMat.h"
#include "../resource/Loader.h"

/* graphics */
#include "../graphics/Instance.h"
#include "../graphics/Camera.h"
#include "../graphics/Gpu.h"
#include "../graphics/Software.h"

/* renderer */
#include "../render/Geom.h"
#include "../render/Scene.h"
#include "../render/Renderer.h"
#include "../render/Cubemap.h"
#include "../render/MultiBuffer.h"
#include "../render/Shadow.h"
#include "../render/SSAO.h"

/* audio */
#include "../audio/Audio.h"

/* window */
#include "../window/Window.h"

/* physics */
#include "../physics/Physics.h"

namespace WR {

/**
 * Init every libraries.
 */
void init_everything();

}
