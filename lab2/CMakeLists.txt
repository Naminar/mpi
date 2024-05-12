cmake_minimum_required(VERSION 3.15)

project(exp_computing
        LANGUAGES C)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

# set(CMAKE_CXX_STANDARD          20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS        OFF)

set(SRC_DIR ${PROJECT_SOURCE_DIR})
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR})

add_compile_options(-lm)

add_executable(parallel
               ${SRC_DIR}/par_solver.c)

target_link_libraries(parallel PRIVATE m)

target_include_directories(parallel
                           PRIVATE ${INCLUDE_DIR}/inc)
target_sources(parallel PRIVATE
                ${SRC_DIR}/src/solver.c)