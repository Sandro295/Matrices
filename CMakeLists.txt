cmake_minimum_required(VERSION 3.12)
project(Matrix_Multiplication)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN " -fsanitize=address,undefined -fno-sanitize-recover=all -Wall -Werror -Wsign-compare  -g"
CACHE STRING "Compiler flags for adress sanitazer build"
FORCE)

find_package(MKL CONFIG REQUIRED)

set(MM_SOURCES_DIR src)
set(MM_SOURCES ${MM_SOURCES_DIR}/main.cpp
               ${MM_SOURCES_DIR}/matrix_util.cpp
               ${MM_SOURCES_DIR}/mkl_util.cpp)

add_executable(matrix ${MM_SOURCES})
# set(CMAKE_CXX_FLAGS -DMKL_ILP64  -m64  -I"${MKLROOT}/include")

target_compile_options(matrix PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
target_include_directories(matrix PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(matrix PUBLIC $<LINK_ONLY:MKL::MKL>)
