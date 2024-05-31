cmake_minimum_required(VERSION 3.28)
project(meta-fsm LANGUAGES CXX)

#Set C++ Options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Print processor and compiler
message(STATUS "Detected processor is " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER_ID: " ${CMAKE_CXX_COMPILER_ID})
message(STATUS "C++ Compiler is " ${CMAKE_CXX_COMPILER_ID} " version " ${CMAKE_CXX_COMPILER_VERSION})

include(FetchContent)

FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.0.0"
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG "v1.14.0"
        GIT_SHALLOW ON
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#Enable clang-tidy checks
set(CMAKE_CXX_CLANG_TIDY clang-tidy -header-filter=.*)

include_directories(include)

add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors -Wsign-conversion)

add_subdirectory(samples)

enable_testing()

add_subdirectory(utest)