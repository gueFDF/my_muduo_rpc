#pragma once
#include "google/protobuf/service.h"
#include <functional>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/TcpServer.h>
#include <string>
#include <unordered_map>

//框架提供的专门服务发布rpc服务的网络对象类

class RpcProvider
{
public:
    //这里是框架提供给外部使用的，可以发布rpc方法的函数接口
  void NotifyService(google::protobuf::Service *service);

  //启动rpc服务节点，开始提供rpc远程网络调用服务
  void Run();

private:
  muduo::net::EventLoop m_eventloop;
  void OnConnection(const muduo::net::TcpConnectionPtr &conn);
  void OnMessage(const muduo::net::TcpConnectionPtr &conn,
                 muduo::net::Buffer *buffer, muduo::Timestamp time);

  //Closure回调操作，用于序列化rpc的相应和网络方发送
  void SendRpcresponse(const muduo::net::TcpConnectionPtr&,google::protobuf::Message*);

  // service服务类型信息
  struct ServiceInfo {
    google::protobuf::Service *m_service; //服务对象
    std::unordered_map<std::string, const google::protobuf::MethodDescriptor *>
        m_methodMap;
  };

  //存储注册成功的服务对象和其服务方法的所有信息
  std::unordered_map<std::string, ServiceInfo> m_serviceMap;
};