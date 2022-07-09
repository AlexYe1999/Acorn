set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(GLOB imgui_sources CONFIGURE_DEPENDS  "${IMGUI_SOURCE_DIR}/*.cpp")
file(GLOB imgui_impl CONFIGURE_DEPENDS  
    "${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.cpp" 
    "${IMGUI_SOURCE_DIR}/backends/imgui_impl_glfw.h"
    "${IMGUI_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp" 
    "${IMGUI_SOURCE_DIR}/backends/imgui_impl_vulkan.h"
)
add_library(imgui STATIC ${imgui_sources} ${imgui_impl})
target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${IMGUI_SOURCE_DIR}>)
#target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${vulkan_include}>)
#target_link_libraries(imgui PUBLIC glfw ${vulkan_lib})