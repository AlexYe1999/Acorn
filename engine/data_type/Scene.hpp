#pragma once
#include"Vertex.hpp"
#include"Matrix.hpp"

namespace AcornEngine{
    struct Scene{
        VertexTest vertices[8] = {
            {Vector3f(-1.0f, -1.0f, -1.0f), Vector4f(0.0f, 0.0f, 1.0f, 1.0f)},
            {Vector3f(-1.0f, +1.0f, -1.0f), Vector4f(0.0f, 1.0f, 0.0f, 1.0f)},
            {Vector3f(+1.0f, +1.0f, -1.0f), Vector4f(0.0f, 1.0f, 1.0f, 1.0f)},
            {Vector3f(+1.0f, -1.0f, -1.0f), Vector4f(1.0f, 0.0f, 0.0f, 1.0f)},
            {Vector3f(-1.0f, -1.0f, +1.0f), Vector4f(1.0f, 0.0f, 1.0f, 1.0f)},
            {Vector3f(-1.0f, +1.0f, +1.0f), Vector4f(1.0f, 1.0f, 0.0f, 1.0f)},
            {Vector3f(+1.0f, +1.0f, +1.0f), Vector4f(1.0f, 1.0f, 1.0f, 1.0f)},
            {Vector3f(+1.0f, -1.0f, +1.0f), Vector4f(0.0f, 0.0f, 0.0f, 1.0f)}
        };

        uint16_t indices[36] = {
            2, 1, 0,
            2, 0, 3,

            4, 5, 6,
            6, 7, 4,

            7, 6, 2,
            2, 3, 7,

            1, 5, 4,
            0, 1, 4,

            4, 7, 0,
            7, 3, 0,

            1, 2, 6,
            1, 6, 5
        };

    };

}