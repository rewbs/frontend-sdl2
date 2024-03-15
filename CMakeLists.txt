cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_POSITION_INDEPENDENT_CODE YES)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(projectM-SDL
        LANGUAGES C CXX
        VERSION 2.0.0
        )

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(SDL2_LINKAGE "shared" CACHE STRING "Set to either shared or static to specify how libSDL2 should be linked. Defaults to shared.")

set(DEFAULT_PRESETS_PATH "\${application.dir}/presets" CACHE STRING "Default presets path in the configuration file.")
set(DEFAULT_TEXTURES_PATH "\${application.dir}/textures" CACHE STRING "Default textures path in the configuration file.")

if(NOT SDL2_LINKAGE STREQUAL "shared" AND NOT SDL2_LINKAGE STREQUAL "static")
    message(FATAL_ERROR "Invalid libSDL2 linkage provided in SDL2_LINKAGE: \"${SDL2_LINKAGE}\".\n"
            "Please specify either \"shared\" or \"static\"."
            )
endif()

find_package(projectM4 REQUIRED COMPONENTS Playlist)
find_package(SDL2 REQUIRED)


set(CMAKE_PREFIX_PATH "/usr/local")
set(CMAKE_INCLUDE_PATH "/usr/local/include")
set(CMAKE_LIBRARY_PATH "/usr/local/lib")

find_package(Poco REQUIRED COMPONENTS JSON XML Util Foundation PATHS /usr/local )

include_directories(${Poco_INCLUDE_DIRS})
link_directories(${Poco_LIBRARY_DIRS})

include(SDL2Target)
include(dependencies_check.cmake)

add_subdirectory(src)

if(ENABLE_TESTING)
    add_subdirectory(test)
endif()

include(install.cmake)
include(packaging.cmake)

message(STATUS "SDL version: ${SDL2_VERSION}")
message(STATUS "Poco version: ${Poco_VERSION}")
message(STATUS "projectM version: ${projectM4_VERSION}")
