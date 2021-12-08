include_directories(
    ${ENGINE_DIR}/interface
)

set(FrameworkFiles 
    AppFramework.hpp
    AppFramework.cpp
)

add_library(AppFramework SHARED ${FrameworkFiles})
target_link_libraries(AppFramework xcb)

add_subdirectory(OpenGL)