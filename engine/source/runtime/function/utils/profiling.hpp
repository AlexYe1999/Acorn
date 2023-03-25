#pragma once

#include "tracy/Tracy.hpp"

#if defined(_DEBUG) 

#define PROF_THREAD_NAME(name) tracy::SetThreadName(name);
#define PROF_FRAME() FrameMark;
#define PROF_FUNCTION() ZoneScoped;

#else

#define PROF_THREAD_NAME(name)
#define PROF_FRAME()
#define PROF_FUNCTION()

#endif


