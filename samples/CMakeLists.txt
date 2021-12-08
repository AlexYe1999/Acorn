
if(PLATFORM_WINDOWS)
    add_subdirectory(Windows)
elseif(PLATFORM_LINUX)
    add_subdirectory(Linux)
endif()