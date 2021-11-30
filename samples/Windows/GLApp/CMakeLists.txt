project(GLApp)

include_directories(
    ../../AppFramework
    ${ENGINE_DIR}/core/log
)

set(ALL_FILE
    stdafx.hpp
    GLApp.hpp
    GLApp.cpp
    GL_Sample.cpp
)

add_executable(GLApp WIN32 ${ALL_FILE})
target_link_libraries(GLApp Log AppFramework)
set_target_properties(GLApp PROPERTIES FOLDER "samples/windows")
