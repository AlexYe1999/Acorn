project(AppFramework)

set(ALL_FILE
    AppFramework.hpp
    AppFramework.cpp
)

add_library(AppFramework ${ALL_FILE})
add_executable(Test WIN32 test.cpp)
target_link_libraries(Test AppFramework)
set_target_properties(Test AppFramework PROPERTIES FOLDER "samples/framework")
