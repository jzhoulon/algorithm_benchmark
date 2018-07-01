#find boost package
cmake_minimum_required(VERSION 2.8)

# set build flags
SET(CMAKE_CXX_FLAGS "-g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_program_options")


IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
ELSEIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -DDEBUG")
ELSE()
  MESSAGE(FATAL_ERROR "Build type is invalid")
ENDIF()

# find dependence libs
SET(__benchmark_libs iomp5 ${lib_name})
FOREACH(__benchmark_lib ${__test_libs})
  STRING(TOUPPER ${__test_lib} __benchmark_lib_upper)
  FIND_LIBRARY(${__test_lib_upper}_LIBRARY
    NAME ${__test_lib}
    PATHS ${lib_link_path} ${CMAKE_BINARY_DIR}
    DOC "The path to library ${__test_lib}")
  LIST(APPEND benchmark_dependency_libs ${__test_lib})
ENDFOREACH()


#find alg src files
SET(benchmark_src_dir "${CMAKE_SOURCE_DIR}/benchmark")
SET(sort_src_dir "${benchmark_src_dir}/sort")

FILE(GLOB_RECURSE benchmark_src_files ${benchmark_src_dir}/*.cpp)
FILE(GLOB_RECURSE sort_src_files ${sort_src_dir}/*.cpp)

#find benchmark includes
SET(benchmark_include_dirs "${CMAKE_SOURCE_DIR}/include")
LIST(APPEND benchmark_include_dirs ${benchmark_src_dir})


#find sort includes
SET(sort_include_dirs "${CMAKE_SOURCE_DIR}/include")
LIST(APPEND sort_incldue_dirs ${sort_src_dir})
#build benchmark

SET(sort_name sort_bench)
ADD_EXECUTABLE(${sort_name} ${sort_src_files})
TARGET_INCLUDE_DIRECTORIES(${sort_name} PUBLIC ${sort_include_dirs})
TARGET_LINK_LIBRARIES(${sort_name} PUBLIC ${benchmark_dependency_libs})
