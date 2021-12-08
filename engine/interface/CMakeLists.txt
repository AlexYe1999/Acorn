
set(ALL_FILE
    None.cpp
    ISceneNode.hpp
    IKeyModule.hpp
    IApplication.hpp
    IRuntimeModule.hpp
)

add_library(Interface ${ALL_FILE})
set_target_properties(Interface PROPERTIES FOLDER "engine/interface") 