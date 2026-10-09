cmake_minimum_required(VERSION 3.16)

project(tinygltf LANGUAGES CXX)

set(TargetName tinygltf)

add_library(${TargetName} 
  INTERFACE
)

target_include_directories(${TargetName} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)