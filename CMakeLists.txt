#expose compiler info
cmake_minimum_required(VERSION 2.8)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#general var
SET(lib_name "alg")

#set build type
IF (NOT CMAKE_BUILD_TYPE)
  MESSAGE(STATUS "Build type is Release by default")
  SET(CMAKE_BUILD_TYPE "Release" CACHE STRING "Set the build type from: Debug Relase." FORCE)
ENDIF()

#set build flags
IF(${CMAKE_BUILD_TYPE} MATCHES  "Release")
  MESSAGE(STATUS "Build typpe is Release")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")
ELSEIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  MESSAGE(STATUS "Build type is Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -DDEBUG")
ELSE()
  MESSAGE(FATAL_ERROR "Build type is invalid")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

SET(__alg_libs iomp5)

FOREACH(__alg_lib ${__alg_libs})
  STRING(TOUPPER ${__alg_lib} __alg_lib_upper)
  FIND_LIBRARY(${__alg_lib_upper}_LIBRARY
    NAME ${__alg_lib}
    PATHS ${lib_link_path}
    DOC "The path to library ${__alg_lib}")
  LIST(APPEND alg_dependency_libs ${__alg_lib})
ENDFOREACH()

# find src file
SET(src_dir "${CMAKE_SOURCE_DIR}/src")
FILE(GLOB_RECURSE alg_src_files ${src_dir}/*.cpp)

# find include dirs
SET(alg_include_dirs "${CMAKE_SOURCE_DIR}/include")
LIST(APPEND alg_include_dirs ${src_dir})

#buld lib
#ADD_LIBRARY(${lib_name} SHARED ${alg_src_files})
#TARGET_INCLUDE_DIRECTORIES(${lib_name} PUBLIC ${euler_include_dirs})
#TARGET_LINK_LIBRARIES(${lib_name} PUBLIC ${euler_dependency_libs})

ADD_SUBDIRECTORY(benchmark)
