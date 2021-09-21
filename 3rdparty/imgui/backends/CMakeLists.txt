include_directories(${3RDPARTY_DIR}/imgui)

set(IMGUI_WIN32_DX12
    imgui_impl_dx12.h
    imgui_impl_dx12.cpp
    imgui_impl_win32.h
    imgui_impl_win32.cpp
)

add_library(IMGUI_WIN32_DX12 ${IMGUI_WIN32_DX12})
target_link_libraries(IMGUI_WIN32_DX12 IMGUI)

set_target_properties(IMGUI_WIN32_DX12 PROPERTIES FOLDER "3rdparty/imgui")