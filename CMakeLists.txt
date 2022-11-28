# 设置cmake的最低版本和项目名称
cmake_minimum_required(VERSION 3.16.3)
project(mprpc)

# 设置可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 设置项目文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 设置编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(/usr/local/include)

# 设置项目文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

link_directories(/usr/local/lib)


# src包含了mprpc框架的所有相关代码
add_subdirectory(src)


# example包含了mprpc框架使用的实例代码
add_subdirectory(example)