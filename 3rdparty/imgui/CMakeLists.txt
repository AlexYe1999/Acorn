add_subdirectory(backends)

set(IMGUI_FILES
    imgui.h
    imconfig.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_truetype.h
    imstb_textedit.h
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)

add_library(IMGUI ${IMGUI_FILES})

set_target_properties(IMGUI PROPERTIES FOLDER "3rdparty/imgui")
