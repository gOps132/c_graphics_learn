cmake_minimum_required(VERSION 3.14)
project(c_graphics_learn
    DESCRIPTION "Learning the fundamentals of computer graphics"
)

set(CMAKE_CXX_STANDARD 17)


set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )

add_subdirectory(lib/glm)
add_subdirectory(lib/SDL2)
add_subdirectory(lib/ZyklonProfiler)

set(C_GRAPHICS_LEARN_SOURCES 
    src/main.cpp
    src/sphere.cpp
    src/hittable_list.cpp
)

add_executable(c_graphics_learn ${C_GRAPHICS_LEARN_SOURCES})

set_target_properties(c_graphics_learn PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}-${CMAKE_BUILD_TYPE}"
)

target_include_directories(c_graphics_learn 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(c_graphics_learn
    PRIVATE     glm
    PUBLIC      SDL2main 
	PUBLIC      SDL2-static
    PUBLIC      zyklon_profiler
)