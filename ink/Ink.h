/**
 * Copyright (C) 2021-2023 HYPERTHEORY
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

/* core part */
#include "core/Error.h"
#include "core/File.h"

/* math part */
#include "math/Vector.h"
#include "math/Color.h"
#include "math/Constants.h"
#include "math/Matrix.h"
#include "math/Euler.h"
#include "math/Random.h"
#include "math/Ray.h"

/* objects part */
#include "objects/Defines.h"
#include "objects/Enums.h"
#include "objects/Image.h"
#include "objects/Mesh.h"
#include "objects/Instance.h"
#include "objects/Uniforms.h"
#include "objects/Material.h"

/* loader part */
#include "loader/Loader.h"

/* camera part */
#include "camera/Camera.h"
#include "camera/OrthoCamera.h"
#include "camera/PerspCamera.h"

/* meshes part */
#include "meshes/BoxMesh.h"
#include "meshes/CylinderMesh.h"
#include "meshes/PlaneMesh.h"
#include "meshes/SphereMesh.h"

/* graphics part */
#include "graphics/Gpu.h"

/* shaders part */
#include "shaders/ShaderCache.h"
#include "shaders/ShaderLib.h"

/* lights part */
#include "lights/Light.h"
#include "lights/Shadow.h"
#include "lights/DirectionalLight.h"
#include "lights/Exp2Fog.h"
#include "lights/HemisphereLight.h"
#include "lights/LinearFog.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"

/* probes part */
#include "probes/IBLFilter.h"
#include "probes/ReflectionProbe.h"

/* scene part */
#include "scene/Scene.h"

/* renderer part */
#include "renderer/Renderer.h"

/* postprocess part */
#include "postprocess/RenderPass.h"
#include "postprocess/BlendPass.h"
#include "postprocess/BloomPass.h"
#include "postprocess/BlurPass.h"
#include "postprocess/ColorGradePass.h"
#include "postprocess/CopyPass.h"
#include "postprocess/FXAAPass.h"
#include "postprocess/GrainPass.h"
#include "postprocess/LightPass.h"
#include "postprocess/SSAOPass.h"
#include "postprocess/SSRPass.h"
#include "postprocess/ToneMapPass.h"
