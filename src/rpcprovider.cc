#include "include/rpcprovider.h"
#include "include/mprpcappclication.h"
#include <string>
#include <functional>
void RpcProvider::NotifyService(google::protobuf::Service *service)
{

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