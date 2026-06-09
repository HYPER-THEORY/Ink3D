cmake_minimum_required(VERSION 3.20)
project(Ink3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE INK_HEADERS  CONFIGURE_DEPENDS "ink/*.h")
file(GLOB_RECURSE INK_SOURCES  CONFIGURE_DEPENDS "ink/*.cpp")

file(GLOB_RECURSE ADDON_HEADERS CONFIGURE_DEPENDS "addons/*.h")
file(GLOB_RECURSE ADDON_SOURCES CONFIGURE_DEPENDS "addons/*.cpp")

set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/libs/opengl/glad.c")

set(MAIN_SOURCE "${CMAKE_SOURCE_DIR}/GetStart.cpp")

add_executable(Ink3D_Example
    ${MAIN_SOURCE}
    ${INK_HEADERS}
    ${INK_SOURCES}
    ${ADDON_HEADERS}
    ${ADDON_SOURCES}
    ${GLAD_SOURCE}
)

target_include_directories(Ink3D_Example PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/ink
    ${CMAKE_SOURCE_DIR}/addons
    ${CMAKE_SOURCE_DIR}/libs
    ${CMAKE_SOURCE_DIR}/libs/sdl
)

target_link_directories(Ink3D_Example PRIVATE
    "${CMAKE_SOURCE_DIR}/libs/sdl/lib/x64"
)

target_link_libraries(Ink3D_Example PRIVATE
    SDL2main
    SDL2
    opengl32
)

add_custom_command(TARGET Ink3D_Example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/libs/sdl/lib/x64/SDL2.dll"
        "$<TARGET_FILE_DIR:Ink3D_Example>/SDL2.dll"
)

set_target_properties(Ink3D_Example PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
