set(ALL_FILE
    BasicKeyBoard.hpp
    BasicMouse.hpp
    BasicMouse.cpp
)

add_library(Input ${ALL_FILE})

include_directories(${ENGINE_DIR}/interface)
include_directories(${ENGINE_DIR}/data_type)
target_link_libraries(Input DataType)

set_target_properties(Input PROPERTIES FOLDER "engine/core") 
