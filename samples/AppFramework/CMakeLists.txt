project(AppFramework)

set(ALL_FILE
    AppFramework.hpp
    AppFramework.cpp
    App.hpp
)

add_library(AppFramework ${ALL_FILE})
set_target_properties(AppFramework PROPERTIES FOLDER "samples/framework")
