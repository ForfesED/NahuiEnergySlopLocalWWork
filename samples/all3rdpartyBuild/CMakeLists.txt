set(TargetName all3rdparty)

nau_collect_files(Sources
  DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  MASK "*.cpp" "*.h"
)

add_executable(${TargetName} ${Sources})

target_link_libraries(${TargetName} PRIVATE
  asio
  EABase
  EASTL
#  lua
#  DirectX-Headers
#  DirectX-Guids
#  tinyutf8
#  fmt
#  webrtc-lite
#  steamwebrtc
#  GameNetworkingSockets
#  GameNetworkingSockets_s
#  utf8cpp
#  gainput
#  vectormath
#  Jolt
#  jsoncpp
#  ska_hash_map
#  xxHash
#  wyhash
#  brotli
#  lzma-9.20
#  zlib-ng
#  zstd-1.4.5
#  md5
#  stb
#  tinyimageformat
#  fast_float
#  ispc_texcomp
#  tinydds
#  binPack2D
#  RenderDoc
#  adlSdk
#  dxc
#  imgui
#  uriparser
#  TracyClient
#  ozz_base
#  ozz_animation
#  png
)

nau_add_compile_options(${TargetName})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${Sources})

set_target_properties (${TargetName} PROPERTIES
    FOLDER "${NauEngineFolder}/samples"
)
