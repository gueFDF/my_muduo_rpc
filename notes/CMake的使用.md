CMake常用操作
```cmake
# 设置cmake最低版本
cmake_minimum_required(VERSION 3.0)
# 设置项目名称
project(cmaketest)
# 配置编译选项
#CMAKE_CXX_COMPILE_FEATURES是默认编译选项，下面是在默认编译选项上加-g选项
set(CMAKE_CXX_COMPILE_FEATURES ${CMAKE_CXX_COMPILE_FEATURES} -g)

# 配置头文件搜索路径
include_directories(/usr/local/include)

#配置库文件搜索路径

link_directories(/usr/local/lib)


# 设置需要编译的源文件列表
set(SRC_LIST muduo_server.cpp)

# .指定路径下的所有源文件名放入变量名SRC_LIST里面
aux_source_directory(. SRC_LIST)

# 以及所要编译的源文件，编译后生成的可执行文件名
add_executable(server ${SRC_LIST})


#表示server这个目标程序需要连接的库文件

target_link_libraries(server -lmuduo_base -lmuduo_net -lpthread)
```



# CMake常用的预定义变量
```cmake
PROJECT_NAME : 通过 project() 指定项目名称
PROJECT_SOURCE_DIR : 工程的根目录
PROJECT_BINARY_DIR : 执行 cmake 命令的目录
CMAKE_CURRENT_SOURCE_DIR : 当前 CMakeList.txt 文件所在的目录
CMAKE_CURRENT_BINARY_DIR : 编译目录，可使用 add subdirectory 来修改
EXECUTABLE_OUTPUT_PATH : 二进制可执行文件输出位置
LIBRARY_OUTPUT_PATH : 库文件输出位置
BUILD_SHARED_LIBS : 默认的库编译方式 ( shared 或 static ) ，默认为 static
CMAKE_C_FLAGS : 设置 C 编译选项
CMAKE_CXX_FLAGS : 设置 C++ 编译选项
CMAKE_CXX_FLAGS_DEBUG : 设置编译类型 Debug 时的编译选项
CMAKE_CXX_FLAGS_RELEASE : 设置编译类型 Release 时的编译选项
CMAKE_GENERATOR : 编译器名称
CMAKE_COMMAND : CMake 可执行文件本身的全路径
CMAKE_BUILD_TYPE : 工程编译生成的版本， Debug / Releas
```