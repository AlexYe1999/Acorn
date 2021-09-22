set(ALL_FILE
    Vertex.hpp
    Vertex.cpp
    Vector.hpp
    Vector.cpp
    Point.hpp
    Point.cpp
    Color.hpp
    Color.cpp
)

add_library(DataType ${ALL_FILE})
set_target_properties(DataType PROPERTIES FOLDER "engine/data_type")