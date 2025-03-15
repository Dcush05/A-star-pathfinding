cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set this to true to ship the game!
# This will change the RESOURCES_PATH to be the local path.
# DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build" OFF)

# DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/arch:AVX2) # Ensure SIMD optimizations
	add_compile_options(/arch:DDEBUG)
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) # Link time optimization



project(Pathfinding C)


# MY_SOURCES is defined to be a list of all the source files for my game
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable("${CMAKE_PROJECT_NAME}" ${MY_SOURCES})
#add_library("${CMAKE_PROJECT_NAME}" SHARED ${MY_SOURCES}) make engine hot reloadable after refactor
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE BUILD_DLL)


# Specify the include directories
target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_directories("${CMAKE_PROJECT_NAME}" PRIVATE
   "${CMAKE_CURRENT_SOURCE_DIR}/libs"
   )

   #set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs")
   #find_package(SDL3 CONFIG REQUIRED)
# Link SDL2 and SDL2_image libraries (for static linking, add SDL2::SDL2-static and SDL2_image::SDL2_image-static)
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
    "${CMAKE_SOURCE_DIR}/libs/SDL3.lib"
)

