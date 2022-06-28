include_directories(${ENGINE_DIR}/interface)

set(ALL_FILE
    Timer.hpp
    Timer.cpp
)

add_library(Timer ${ALL_FILE})
set_target_properties(Timer PROPERTIES FOLDER "engine/core") 