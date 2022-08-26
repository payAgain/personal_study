cmake_minimum_required (VERSION 3.10) 

project (tutorial)

# 指定执行文件的输出路径
# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 
# 静态库 lib 输出路径
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# 设置cmake输出路径
# set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/build)
# 
# set(CMAKE_CURRENT_BINARY_DIR, ${PROJECT_SOURCE_DIR}/build)

message(status EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})
message(status PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
message(status CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR})
message(status PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})
add_executable(tutorial ../src/tutorial.cpp)