cmake_minimum_required(VERSION 3.12)

project(Chapter14-Parallel_Algorithms_Benchmarks)

# 使用vcpkg查找benchmark
find_package(benchmark CONFIG REQUIRED)

file(GLOB BM_SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unneeded-internal-declaration")
endif()

foreach(CPP_FILE_PATH ${BM_SRC_FILES})
  get_filename_component(CPP_FILE ${CPP_FILE_PATH} NAME)
  string(REPLACE ".cpp" "" EXE_NAME ${CPP_FILE})
  add_executable(${EXE_NAME} ${CPP_FILE})
  # 为名为EXE_NAME的target开启C++20的支持
  set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20)
  target_link_libraries(${EXE_NAME} PRIVATE benchmark::benchmark)
endforeach(CPP_FILE_PATH ${BM_SRC_FILES})
