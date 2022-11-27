#include <iostream>
#include <string>
#include "user.pb.h"


//使用在rpc服务发布端(rpc服务提供者)
class UserService : public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service :Login" << std::endl;
        std::cout << "name: " << name << "pwd: " << pwd << std::endl;
    }

    //重写基类UserServiceRpc的虚函数，下面这些方法都是框架直接调用的
    void Login(::google::protobuf::RpcController *controller,
               const ::fixbug::LoginRequest *request,
               ::fixbug::LoginResponse *response,
               ::google::protobuf::Closure *done)
    {
        //框架给业务上报请求参数LogRquest，应用程序获取相应数据做本地业务
        std::string name=request->name();
        std::string pwd=request->pwd();


        //做本地业务
        bool login_result=Login(name,pwd);

        //把响应写入(错误码，错误消息，返回值)
        fixbug::Resultcode*cond=response->mutable_result();
        cond->set_errcode(0);
        cond->set_errmsg("");
        response->set_success(login_result);

        //执行回调操作
        //执行响应对象数据的序列化和反序列化（由框架完成）
        done->Run();
    }
};
int main()
{
}