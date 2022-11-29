#include "rpcprovider.h"
#include "mprpcappclication.h"
#include <google/protobuf/descriptor.h>

//发布rpc方法的函数接口
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
  ServiceInfo server_info;
  //获取server的descriptor
  const google::protobuf::ServiceDescriptor *pservicrDes =
      service->GetDescriptor();
  //获取名字
  std::string server_name = pservicrDes->name();
  //获取服务对象的方法数量
  int method_count =pservicrDes->method_count();

  //注册
  for (int i = 0; i < method_count; ++i) {
    const google::protobuf::MethodDescriptor *pmethodDes =pservicrDes->method(i);
    std::string method_name = pmethodDes->name();
    server_info.m_methodMap.insert({method_name, pmethodDes});
  }
  server_info.m_service = service;
  m_serviceMap.insert({server_name, server_info});
}
void RpcProvider::Run()
{
    std::string ip = MprpcApplication::GetInitstance().GetConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::GetInitstance().GetConfig().Load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip, port);

    //创建tcpserver
    muduo::net::TcpServer server(&m_eventloop, address, "Rpcserver");

    //绑定连接回调(链接回调和消息读写回调)
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,
                                           this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage,
                                        this, std::placeholders::_1,
                                        std::placeholders::_2,
                                        std::placeholders::_3));

    //设置muduo库的线程数量
    server.setThreadNum(4);


    //启动网络服务
    server.start();
    m_eventloop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn)
{
}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp time)
{
    
}