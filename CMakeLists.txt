cmake_minimum_required(VERSION 3.12)
project(Matrix_Multiplication)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_ASAN " -fsanitize=address,undefined -fno-sanitize-recover=all -Wall -Werror -Wsign-compare  -g"
CACHE STRING "Compiler flags for adress sanitazer build"
FORCE)

find_package(MKL CONFIG REQUIRED)

set(MM_SOURCES_DIR src)
file(GLOB_RECURSE MM_SOURCES CONFIGURE_DEPENDS "${MM_SOURCES_DIR}/*.h" "${MM_SOURCES_DIR}/*.cpp")
# set(MM_SOURCES ${MM_SOURCES_DIR}/main.cpp
#                ${MM_SOURCES_DIR}/matrix_util.cpp
#                ${MM_SOURCES_DIR}/mkl_util.cpp
#                ${MM_SOURCES_DIR}/aocl_util.cpp)

add_executable(matrix ${MM_SOURCES})
# set(CMAKE_CXX_FLAGS -DMKL_ILP64  -m64  -I"${MKLROOT}/include")

# add_library(amd-blis SHARED IMPORTED)
# set(AMD_BLIS_SO_DIR /home/danil/unn/science/amd-blis/lib/ILP64)
# set_property(TARGET amd-blis PROPERTY IMPORTED_LOCATION ${AMD_BLIS_SO_DIR}/libblis.a)

# set(BLA_VENDOR ACML)
# find_package(BLAS PATHS /usr/local CONFIG REQUIRED)

target_include_directories(matrix PUBLIC /usr/local/include/blis)
target_link_libraries(matrix PUBLIC /usr/local/lib/libblis.a)


target_compile_options(matrix PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)
target_include_directories(matrix PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(matrix PUBLIC $<LINK_ONLY:MKL::MKL>)
