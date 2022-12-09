#include "mprpcappclication.h"
#include "mprpcchannel.h"
#include "user.pb.h"
#include <iostream>

int main(int argc, char *argv[]) {
  //整个程序启动以后，想使用mprpc享受rpc服务调用，一定需要前调用框架初始化函数
  MprpcApplication::Init(argc, argv);
  //创建channel对象
  fixbug::UserServiceRpc_Stub sub(new MprpcChannel);
  // rpc方法的调用参数
  fixbug::LoginRequest request;
  request.set_name("zhangsan");
  request.set_pwd("11111111");

  // rpc方法的响应
  fixbug::LoginResponse response;
  //发起rpc方法调用
  sub.Login(nullptr, &request, &response, nullptr);
  // fixbug::UserServiceRpc_Stub

  //一次rpc调用完成
  if (response.result().errcode() == 0) //调用成功
  {
    printf("登陆成功\n");
  } else { //调用失败
    //打印错误消息
    std::cout << response.result().errmsg() << std::endl;
  }
  return 0;
}