cmake_minimum_required (VERSION 3.8)

project (Chapter14-Parallel_Algorithms)

# 使用vcpkg查找GTest
find_package(GTest CONFIG REQUIRED)

file(
  GLOB
  CHAPTER_SRC_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
  )

add_executable (${PROJECT_NAME}
  ${CHAPTER_SRC_FILES}
  )

# 为名为PROJECT_NAME的target开启C++20的支持
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries (${PROJECT_NAME}
  GTest::gtest
)


