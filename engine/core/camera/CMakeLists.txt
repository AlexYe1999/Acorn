include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)

set(ALL_FILE
    Camera.hpp
    Camera.cpp
)

add_library(Camera ${ALL_FILE})
set_target_properties(Camera PROPERTIES FOLDER "engine/core") 