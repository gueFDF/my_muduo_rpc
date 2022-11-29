#include "rpcprovider.h"
#include "mprpcappclication.h"
#include "rpcheader.pb.h"
#include <cstdint>
#include <cstring>
#include <functional>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include <google/protobuf/stubs/callback.h>
#include <muduo/net/Endian.h>


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
  if(!conn->connected())
  {
    //连接断开
    conn->shutdown();
  }
}

void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp time)
{
    //  //接受buffer字节流
    std::string recv_buf=buffer->retrieveAllAsString();

    //获取长度（需转换网络字节序）
    uint32_t be32=0;
    ::memcpy(&be32,recv_buf.c_str(),sizeof(be32));
    uint32_t header_size=muduo::net::sockets::networkToHost32(be32);
    //获取数据流
    std::string rpc_header_str=recv_buf.substr(4,header_size);
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;  //也就是用户蹭的protobuf
    if(rpcHeader.ParseFromString(rpc_header_str))
    {
        //反序列化成功
        service_name=rpcHeader.service_name();
        method_name=rpcHeader.method_name();
        args_size=rpcHeader.args_size();
    }
    else
    {
        //反序列化失败
        std::cout<<"rpc_header_str is error : "<<rpc_header_str<<std::endl;
        return;
    }
    //获取参数字节流
    std::string args_str=recv_buf.substr(4+header_size,args_size);


    //获取server对象和method方法
    auto it=m_serviceMap.find(service_name);//查询服务
    if(it==m_serviceMap.end())
    {
      //所查询服务不存在
      std::cout<<"所查询服务不存在"<<std::endl;
      return;
    }
    auto mit=it->second.m_methodMap.find(method_name);
    if(mit==it->second.m_methodMap.end())
    {
      //所查询方法不存在
      std::cout<<"所查询方法不存在"<<std::endl;
      return;
    }
    google::protobuf::Service*server=it->second.m_service;
    const google::protobuf::MethodDescriptor*methodDes=mit->second;
   
    //生成rpc请求的resquest和相应的response参数
    google::protobuf::Message* request=server->GetRequestPrototype(methodDes).New();
    google::protobuf::Message*response=server->GetResponsePrototype(methodDes).New();


    //给methodd的closure的回调函数
    google::protobuf::Closure *done =google::protobuf::NewCallback<RpcProvider,
                                                                  const muduo::net::TcpConnectionPtr&,
                                                                  google::protobuf::Message*>
      (this,&RpcProvider::SendRpcresponse ,conn, response);
    //在框架调用远端方法    
    server->CallMethod(methodDes, nullptr,request, response, done);
}


void RpcProvider::SendRpcresponse(const muduo::net::TcpConnectionPtr&conn,google::protobuf::Message*response)
{
  std::string response_str;
  if(response->SerializeToString(&response_str))
  {
    //序列成功
    conn->send(response_str);
  }
  else 
  {
    //序列化失败
    std::cout<<"序列化失败"<<std::endl;
  }
   conn->shutdown();//模拟短链接
}