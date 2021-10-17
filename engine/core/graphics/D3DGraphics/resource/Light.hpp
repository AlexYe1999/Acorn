#pragma once
#include"Vector.hpp"
namespace Acorn{

    struct Light{
        Vector3f Strength = {0.5f, 0.5f, 0.5f};
	    float  FalloffStart = 1.0f;
	    Vector3f Direction = {0.0f, -1.0f, 0.0f};
	    float  FalloffEnd = 10.0f;
	    Vector3f Position = {0.0f, 0.0f, 0.0f};
	    float  SpotPower = 64.0f;
    };

}