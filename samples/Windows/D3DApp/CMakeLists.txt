project(D3DApp)

include_directories(
    ../../AppFramework
    ${ENGINE_DIR}/core/graphics/D3DGraphics
)

set(ALL_FILE
    stdafx.hpp
    D3DApp.hpp
    D3DApp.cpp
    D3DTest.cpp
)

add_executable(Test WIN32 ${ALL_FILE})
target_link_libraries(Test AppFramework)
set_target_properties(Test AppFramework PROPERTIES FOLDER "samples/D3DApp")
