# CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目设置
project(MyMath VERSION 1.0.0 DESCRIPTION "我的数学库")

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
# 包含头文件目录
include_directories(include)

# 添加可执行文件（测试程序）
add_executable(test_math tests/test_vec3.cpp)

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)