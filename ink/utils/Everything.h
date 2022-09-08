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

/* core */
#include "../core/Error.h"
#include "../core/File.h"
#include "../core/Format.h"
#include "../core/Date.h"

/* math */
#include "../math/Constants.h"
#include "../math/Random.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"
#include "../math/Matrix.h"
#include "../math/Ray.h"

/* objects */
#include "../objects/Constants.h"
#include "../objects/Uniforms.h"
#include "../objects/Defines.h"
#include "../objects/Mesh.h"
#include "../objects/Image.h"
#include "../objects/Material.h"
#include "../objects/Instance.h"

/* loader */
#include "../loader/Loader.h"

/* camera */
#include "../camera/Camera.h"
#include "../camera/PerspCamera.h"
#include "../camera/OrthoCamera.h"

/* meshes */
#include "../meshes/BoxMesh.h"
#include "../meshes/PlaneMesh.h"
#include "../meshes/CylinderMesh.h"
#include "../meshes/SphereMesh.h"

/* graphics */
#include "../graphics/Software.h"
#include "../graphics/Gpu.h"

/* lights */
#include "../lights/Light.h"
#include "../lights/Shadow.h"
#include "../lights/PointLight.h"
#include "../lights/SpotLight.h"
#include "../lights/DirectionalLight.h"
#include "../lights/AmbientLight.h"
#include "../lights/HemisphereLight.h"
#include "../lights/IBLPrefilter.h"
#include "../lights/LightProbe.h"
#include "../lights/EnvProbe.h"

/* scene */
#include "../scene/Scene.h"

/* renderer */
#include "../renderer/RenderPass.h"
#include "../renderer/Renderer.h"
#include "../renderer/CopyPass.h"
#include "../renderer/BlendPass.h"
#include "../renderer/BlurPass.h"
#include "../renderer/LightPass.h"

/* postprocess */
#include "../postprocess/ToneMappingPass.h"
#include "../postprocess/ColorGradingPass.h"
#include "../postprocess/SSAOPass.h"
#include "../postprocess/FXAAPass.h"

/* audio */
#include "../audio/Audio.h"

/* physics */
#include "../physics/Physics.h"

/* window */
#include "../window/Window.h"
