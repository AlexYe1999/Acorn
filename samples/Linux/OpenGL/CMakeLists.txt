project(Samples_Linux_Opengl)

include_directories(
    ${ENGINE_DIR}/interface
)

set(ALL_FILES
    GLApp.hpp
    GLApp.cpp
    main.cpp
)


add_executable(GLApp ${ALL_FILES})
target_link_libraries(GLApp AppFramework)