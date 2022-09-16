cmake_minimum_required (VERSION 3.4)
project( Ink3D C)

# ###
# Build for mulle-objc (https://mulle-objc.github.io/):
#
#    mulle-sde init -m foundation/objc-porter executable
#    mulle-sde craft
#
# [mulle-clang >= 8.0.0.0 and mulle-objc >= 0.15 required]
#

if( NOT DEPENDENCY_DIR)
   set( DEPENDENCY_DIR "$ENV{DEPENDENCY_DIR}")
endif()
if( NOT DEPENDENCY_DIR)
   set( DEPENDENCY_DIR "${PROJECT_SOURCE_DIR}/dependency")
endif()
if( EXISTS "${DEPENDENCY_DIR}")
   if( NOT ADDICTION_DIR)
      get_filename_component( ADDICTION_DIR "${DEPENDENCY_DIR}" DIRECTORY)
      set( ADDICTION_DIR "${ADDICTION_DIR}/addiction")
   endif()
   set( CMAKE_FIND_FRAMEWORK "LAST")
   include_directories( BEFORE SYSTEM
      ${DEPENDENCY_DIR}/include
      ${ADDICTION_DIR}/include
   )
   set( CMAKE_FRAMEWORK_PATH
      ${DEPENDENCY_DIR}/Frameworks
      ${ADDICTION_DIR}/Frameworks
      ${CMAKE_FRAMEWORK_PATH}
   )
   set( CMAKE_LIBRARY_PATH
      ${DEPENDENCY_DIR}/lib
      ${ADDICTION_DIR}/lib
      ${CMAKE_LIBRARY_PATH}
   )
   set( SKIP_FIND_FOUNDATION_LIBRARY ON)
   execute_process( COMMAND mulle-sde stash-dir
                    OUTPUT_VARIABLE MULLE_SOURCETREE_STASH_DIR)
   string( STRIP "${MULLE_SOURCETREE_STASH_DIR}" MULLE_SOURCETREE_STASH_DIR)
   execute_process( COMMAND mulle-sde linkorder --stash-dir "${MULLE_SOURCETREE_STASH_DIR}" --startup --simplify --output-format cmake
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                    OUTPUT_VARIABLE MULLE_SDE_LINKER_FLAGS
                    RESULT_VARIABLE RVAL)
   if( NOT ${RVAL} EQUAL 0)
      message( FATAL_ERROR "Failed to procure linkorder from mulle-sde. Old version ?")
   endif()
   message( STATUS "MULLE_SDE_LINKER_FLAGS=\"${MULLE_SDE_LINKER_FLAGS}\"")
else()
   message( STATUS "Not a mulle-sde build (\"${DEPENDENCY_DIR}\" does not exist)")
endif()


#
# Platform specific definitions
#

if( APPLE)
   # # CMAKE_OSX_SYSROOT must be set for CMAKE_OSX_DEPLOYMENT_TARGET (cmake bug)
   # if( NOT CMAKE_OSX_SYSROOT)
   #    set( CMAKE_OSX_SYSROOT "/" CACHE STRING "SDK for OSX" FORCE)   # means current OS X
   # endif()
   #
   # # baseline set to 10.6 for rpath
   # if( NOT CMAKE_OSX_DEPLOYMENT_TARGET)
   #   set(CMAKE_OSX_DEPLOYMENT_TARGET "10.6" CACHE STRING "Deployment target for OSX" FORCE)
   # endif()

   set( CMAKE_POSITION_INDEPENDENT_CODE FALSE)
   set( CMAKE_INSTALL_RPATH "@rpath/../lib")

   set( BEGIN_ALL_LOAD "-all_load")
   set( END_ALL_LOAD)
else()
   set( CMAKE_POSITION_INDEPENDENT_CODE TRUE)

   if( WIN32)
   # windows
   else()
   # linux / gcc
      set( BEGIN_ALL_LOAD "-Wl,--whole-archive")
      set( END_ALL_LOAD "-Wl,--no-whole-archive")
      set( CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")
   endif()
endif()


##
## Ink3D Files
##

# uncomment this for mulle-objc to search libraries first
# set( CMAKE_FIND_FRAMEWORK "LAST")

set( SOURCES
ink/audio/Audio.cxx
ink/camera/Camera.cxx
ink/camera/OrthoCamera.cxx
ink/camera/PerspCamera.cxx
ink/core/Date.cxx
ink/core/Error.cxx
ink/core/File.cxx
ink/core/Format.cxx
ink/graphics/Gpu.cxx
ink/graphics/Software.cxx
ink/lights/AmbientLight.cxx
ink/lights/DirectionalLight.cxx
ink/lights/EnvProbe.cxx
ink/lights/ExpFog.cxx
ink/lights/HemisphereLight.cxx
ink/lights/IBLPrefilter.cxx
ink/lights/Light.cxx
ink/lights/LightProbe.cxx
ink/lights/LinearFog.cxx
ink/lights/PointLight.cxx
ink/lights/Shadow.cxx
ink/lights/SpotLight.cxx
ink/loader/Loader.cxx
ink/math/Matrix.cxx
ink/math/Random.cxx
ink/math/Ray.cxx
ink/math/Vector2.cxx
ink/math/Vector3.cxx
ink/math/Vector4.cxx
ink/meshes/BoxMesh.cxx
ink/meshes/CylinderMesh.cxx
ink/meshes/PlaneMesh.cxx
ink/meshes/SphereMesh.cxx
ink/objects/Defines.cxx
ink/objects/Image.cxx
ink/objects/Instance.cxx
ink/objects/Material.cxx
ink/objects/Mesh.cxx
ink/objects/Uniforms.cxx
ink/physics/Physics.cxx
ink/postprocess/BloomPass.cxx
ink/postprocess/ColorGradingPass.cxx
ink/postprocess/FXAAPass.cxx
ink/postprocess/SSAOPass.cxx
ink/postprocess/ToneMappingPass.cxx
ink/renderer/BlendPass.cxx
ink/renderer/BlurPass.cxx
ink/renderer/CopyPass.cxx
ink/renderer/LightPass.cxx
ink/renderer/RenderPass.cxx
ink/renderer/Renderer.cxx
ink/scene/Scene.cxx
ink/utils/ColorTransform.cxx
ink/utils/ConvexHull.cxx
ink/utils/Viewer.cxx
ink/window/Window.cxx
libs/glad/glad.c
main.cpp
)

set( STATIC_DEPENDENCIES
)

if( NOT SKIP_FIND_FOUNDATION_LIBRARY)
   find_library( FOUNDATION_LIBRARY Foundation)
   message( STATUS "FOUNDATION_LIBRARY is ${FOUNDATION_LIBRARY}")
endif()
if( NOT SKIP_FIND_SDL2_LIBRARY)
   find_library( SDL2_LIBRARY SDL2)
   message( STATUS "SDL2_LIBRARY is ${SDL2_LIBRARY}")
endif()

set( DEPENDENCIES
${FOUNDATION_LIBRARY}
${SDL2_LIBRARY}
)


##
## Ink3D
##

add_executable( Ink3D
${SOURCES}
${PUBLIC_HEADERS}
${PROJECT_HEADERS}
${PRIVATE_HEADERS}
${RESOURCES}
)

target_link_libraries( Ink3D
${BEGIN_ALL_LOAD}
${STATIC_DEPENDENCIES}
${END_ALL_LOAD}
${DEPENDENCIES}
${MULLE_SDE_LINKER_FLAGS}
)
