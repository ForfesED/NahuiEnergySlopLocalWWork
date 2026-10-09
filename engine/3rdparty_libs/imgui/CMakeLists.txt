SET(SOURCE
  "imgui.cpp"
  "imgui_demo.cpp"
  "imgui_draw.cpp"
  "imgui_tables.cpp"
  "imgui_widgets.cpp"
  "implot.cpp"
  "implot_demo.cpp"
  "implot_items.cpp"
)

SET(HEADERS
  "include/imconfig.h"
  "include/imgui.h"
  "include/imgui.h.imvector.inc"
  "include/imgui_internal.h"
  "include/implot.h"
  "include/implot_internal.h"
  "include/imstb_rectpack.h"
  "include/imstb_textedit.h"
  "include/imstb_truetype.h"
)

if (BUILD_SHARED_LIBS)
  add_library(imgui SHARED ${SOURCE} ${HEADERS})
  
  target_compile_definitions(imgui PRIVATE
    IMGUI_EXPORTS)
else()
  add_library(imgui STATIC ${SOURCE} ${HEADERS})
    target_compile_definitions(imgui PRIVATE
    NAU_STATIC_RUNTIME)
endif()

target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include/3rdPartyLibs/imgui/include>
)

nau_add_compile_options(imgui)

nau_install(imgui 3rdPartyLibs ${HEADERS})