#pragma once

//框架读取配置及文件

#include <unordered_map>

#include <string>





class MprpcConfig
{
public:
    //负责解析加载配置文件
    void Loadconfig(const char *config_file);
    //查询配置信息
    std::string Load(const std::string &key);

private:
    std::unordered_map<std::string, std::string> m_configMap;
};