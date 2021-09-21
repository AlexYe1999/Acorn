
set(ALL_FILE
    None.cpp
    IKeyModule.hpp
    IRuntimeModule.hpp
)

add_library(Interface ${ALL_FILE})
set_target_properties(Interface PROPERTIES FOLDER "engine/interface") 