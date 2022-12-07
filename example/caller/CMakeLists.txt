set(SRC_LIST calluserservice.cc ../user.pb.cc)


#设置生成可执行文件的文件名
add_executable(client ${SRC_LIST})

# 设置所要链接的库文件
target_link_libraries(client mprpc protobuf)