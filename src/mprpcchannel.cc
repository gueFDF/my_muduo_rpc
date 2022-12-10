#include "mprpcchannel.h"
#include "mprpcappclication.h"
#include "muduo/net/Buffer.h"
#include "rpcheader.pb.h"
#include <arpa/inet.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <google/protobuf/descriptor.h>
#include <iostream>
#include <muduo/net/Endian.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done) {
  const google::protobuf::ServiceDescriptor *sd =
      method->service(); //获取服务的descript,用来获取服务名
  std::string server_name = sd->name();
  std::string method_name = method->name();

  //获取response反序列化后的长度
  uint32_t args_size = 0;
  std::string argvs;

  if (request->SerializeToString(&argvs)) {
    args_size = argvs.size();
  }

  //根据自定义规则，定义rpcheader
  mprpc::RpcHeader rpcHeader;
  rpcHeader.set_service_name(server_name);
  rpcHeader.set_method_name(method_name);
  rpcHeader.set_args_size(args_size);
  std::string rpc_header_str;
  uint32_t header_size = 0;
  if (rpcHeader.SerializeToString(&rpc_header_str)) {
    header_size = rpc_header_str.size();
    std::cout << "header_size:  " << header_size << std::endl;
  } else {
    std::cout << "rpcChannel错误" << std::endl;
    return;
  }
  //整合header_size+server_name+method_name+args_size+args

  //转换网络字节序
  uint32_t be32;
  be32 = muduo::net::sockets::hostToNetwork32(header_size);
  std::string send_request;
  send_request.append((char *)&be32, sizeof(be32));
  // std::copy((char *)&be32, (char *)&be32 + sizeof(be32),
  // send_request.begin());
  // std::copy()

  send_request += rpc_header_str;
  send_request += argvs;
  // tcp连接,创建套接字
  int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clientfd) //创建失败
  {
    std::cout << "socket error" << std::endl;
    exit(0);
  }

  struct sockaddr_in server_addr;

  std::string ip =
      MprpcApplication::GetInitstance().GetConfig().Load("rpcserverip");
  uint16_t port = atoi(MprpcApplication::GetInitstance()
                           .GetConfig()
                           .Load("rpcserverport")
                           .c_str());
  //绑定地址
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr.s_addr);
  std::cout << "ip:" << ip << std::endl;
  std::cout << "port:" << port << std::endl;

  //连接
  if (-1 ==
      connect(clientfd, (struct sockaddr *)&server_addr, sizeof server_addr)) {
    //失败
    perror("connect error");
    close(clientfd);
    exit(0);
  }
  //发送rpc请求
  std::cout << "send_request" << send_request << std::endl;
  int recv_size;
  if (-1 == (send(clientfd, send_request.c_str(), send_request.size(), 0))) {
    close(clientfd);
    perror("send error");
    return;
  }

  //接收rpc响应
  char recv_buf[1024];
  uint32_t size;
  //先接收长度，然后在接受消息内容
  if (-1 == (recv(clientfd, (char *)&size, sizeof(uint32_t), 0))) {
    close(clientfd);
    perror("recv error");
    return;
  }
  size = ntohl(size);
  std::cout<<"size: "<<size<<std::endl;
  if (-1 == (recv(clientfd, recv_buf, size, 0))) {
    close(clientfd);
    perror("recv error");
    return;
  }

  std::string response_str(recv_buf, 0, recv_size);
  if (response->ParseFromString(response_str)) {
    close(clientfd);
    perror("parse error");
    return;
  }

  close(clientfd);
  return;
}