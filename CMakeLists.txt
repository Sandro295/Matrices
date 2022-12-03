cmake_minimum_required(VERSION 3.12)
project(Matrix_Multiplication)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN " -fsanitize=address,undefined -fno-sanitize-recover=all -Wall -Werror -Wsign-compare  -g"
        CACHE STRING "Compiler flags build for YDS"
        FORCE)

set(CMAKE_CXX_FLAGS_ULTIMATE "-g -Wall -Wextra -pedantic -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector"
        CACHE STRING "Compiler flags in ultimate debug build"
        FORCE)

add_executable(matrix main.cpp matrix_util.cpp)
