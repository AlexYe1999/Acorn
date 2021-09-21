include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)

set(ALL_FILE
    AssetLoader.hpp
    AssetLoader.cpp
)

add_library(AssetLoader ${ALL_FILE})
set_target_properties(AssetLoader PROPERTIES FOLDER "engine/core") 