cmake_minimum_required (VERSION 3.0)
project (GuiCode)

set (CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_subdirectory(libs)

set(SRCGUI "${GuiCode_SOURCE_DIR}/")

file(GLOB_RECURSE SOURCE
  "${SRCGUI}source/*.cpp"
  "${SRCGUI}include/*.hpp"
  "${SRCGUI}resources/*.h"
  "${SRCGUI}resources/*.rc"
  "${SRCGUI}shaders/*.shader"
) 

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_library(GLAD STATIC
  libs/glad/glad.cpp
)

include_directories(
  libs/
  include/
  resources/
)

add_library(GuiCode STATIC
  ${SOURCE}
)

source_group(TREE ${SRCGUI} FILES ${SOURCE})

target_precompile_headers(GuiCode PUBLIC
  "${SRCGUI}include/GuiCode/pch.hpp"
)

target_link_libraries(GuiCode
  ${OPENGL_LIBRARY}
  ${GLFW_LIBRARIES}
  glfw
  GLAD
  freetype
  comctl32
  propsys
  dwmapi
  gdiplus
  user32
)

add_definitions(-DABSOLUTE_SHADER_PATH=\"${SRCGUI}shaders/\")
add_definitions(-DABSOLUTE_ASSETS_PATH=\"${SRCGUI}assets/\")

