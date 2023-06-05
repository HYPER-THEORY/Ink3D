cmake_minimum_required(VERSION 3.4)
project(Ink3D)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        ink/renderer/Renderer.cxx
        ink/probes/IBLFilter.cxx
        ink/probes/ReflectionProbe.cxx
        ink/core/File.cxx
        ink/core/Date.cxx
        ink/core/Error.cxx
        ink/camera/PerspCamera.cxx
        ink/camera/OrthoCamera.cxx
        ink/camera/Camera.cxx
        ink/objects/Material.cxx
        ink/objects/Defines.cxx
        ink/objects/Instance.cxx
        ink/objects/Uniforms.cxx
        ink/objects/Image.cxx
        ink/objects/Mesh.cxx
        ink/utils/ImageUtils.cxx
        ink/utils/ConvexHull.cxx
        ink/utils/ColorUtils.cxx
        ink/utils/Viewer.cxx
        ink/math/Color.cxx
        ink/math/Euler.cxx
        ink/math/Matrix.cxx
        ink/math/Ray.cxx
        ink/math/Random.cxx
        ink/math/Vector2.cxx
        ink/math/Vector3.cxx
        ink/math/Vector4.cxx
        ink/meshes/SphereMesh.cxx
        ink/meshes/BoxMesh.cxx
        ink/meshes/CylinderMesh.cxx
        ink/meshes/PlaneMesh.cxx
        ink/audio/Audio.cxx
        ink/loader/Loader.cxx
        ink/window/Window.cxx
        ink/shaders/ShaderLib.cxx
        ink/shaders/ShaderCache.cxx
        ink/postprocess/FXAAPass.cxx
        ink/postprocess/ToneMapPass.cxx
        ink/postprocess/BlurPass.cxx
        ink/postprocess/LightPass.cxx
        ink/postprocess/SSAOPass.cxx
        ink/postprocess/CopyPass.cxx
        ink/postprocess/RenderPass.cxx
        ink/postprocess/BlendPass.cxx
        ink/postprocess/ColorGradePass.cxx
        ink/postprocess/SSRPass.cxx
        ink/postprocess/BloomPass.cxx
        ink/postprocess/GrainPass.cxx
        ink/scene/Scene.cxx
        ink/graphics/Gpu.cxx
        ink/lights/LinearFog.cxx
        ink/lights/Light.cxx
        ink/lights/HemisphereLight.cxx
        ink/lights/SpotLight.cxx
        ink/lights/PointLight.cxx
        ink/lights/Exp2Fog.cxx
        ink/lights/Shadow.cxx
        ink/lights/DirectionalLight.cxx
        libs/opengl/glad.c
)

if(NOT SKIP_FIND_SDL2_LIBRARY)
    find_library(SDL2_LIBRARY SDL2)
    message(STATUS "SDL2_LIBRARY is ${SDL2_LIBRARY}")
endif()

set(
        DEPENDENCIES
        ${SDL2_LIBRARY}
)

add_executable(Ink3D ${SOURCES})

target_link_libraries(Ink3D ${DEPENDENCIES})
