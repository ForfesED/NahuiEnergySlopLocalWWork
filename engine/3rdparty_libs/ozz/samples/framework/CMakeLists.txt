# Framework library
add_library(sample_framework STATIC
  application.h
  application.cc
  imgui.h
  image.h
  image.cc
  profile.h
  profile.cc
  renderer.h
  utils.h
  utils.cc
  mesh.h
  mesh.cc
  internal/camera.h
  internal/camera.cc
  internal/icosphere.h
  internal/immediate.h
  internal/immediate.cc
  internal/imgui_impl.h
  internal/imgui_impl.cc
  internal/renderer_impl.h
  internal/renderer_impl.cc
  internal/shader.h
  internal/shader.cc
  internal/shooter.h
  internal/shooter.cc)

# Samples requires OpenGL package.
if(NOT EMSCRIPTEN)
  add_subdirectory(${PROJECT_SOURCE_DIR}/extern/glfw glfw_ozz) # NAU-1655: avoid conflict with other glfw, but we need this one for working ozz samples
  target_link_libraries(sample_framework glfw_ozz)
else()
  target_link_options(sample_framework PUBLIC -sUSE_GLFW=2 -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2)
endif()

target_link_libraries(sample_framework
  ozz_geometry
  ozz_animation_offline
  ozz_options)
  
target_include_directories(sample_framework PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/samples>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/samples>)

if(TARGET BUILD_DATA_SAMPLE)
  add_dependencies(sample_framework BUILD_DATA_SAMPLE)
endif()

set_target_properties(sample_framework PROPERTIES FOLDER "NauEngine/3rdparty/ozz/samples")

add_subdirectory(tools)
