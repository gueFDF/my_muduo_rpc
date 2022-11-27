#pragma once

// mprpc框架的基础类
//设计成单例模式
class MprpcApplication
{
public:
    static void Init(int argc, char **argv);
    static MprpcApplication &GetInitstance();

private:
    MprpcApplication(){};
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
};