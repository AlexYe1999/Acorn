include_directories(
    ${ENGINE_DIR}/interface
    ${3RDPARTY_DIR}/spdlog/include
)

set(ALL_FILE
    Log.hpp
    Log.cpp
)

add_library(Log SHARED ${ALL_FILE})
target_link_libraries(Log spdlog)
set_target_properties(Log PROPERTIES FOLDER "engine/core") 