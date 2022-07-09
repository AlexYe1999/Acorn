include_directories(${ENGINE_DIR}/interface)

set(ALL_FILE
    Allocator.hpp
    Allocator.cpp
    MemoryManager.hpp
    MemoryManager.cpp
)

add_library(Memory ${ALL_FILE})
set_target_properties(Memory PROPERTIES FOLDER "engine/core") 