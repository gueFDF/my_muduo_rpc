set(SRC_LIST userservice.cc ../user.pb.cc)

#设置头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/../)
include_directories(${PROHECT_SOURCE_DIR}/../../src/include)


#设置生成可执行文件的文件名
add_executable(provider ${SRC_LIST})

# 设置所要链接的库文件
target_link_libraries(provider mprpc protobuf)