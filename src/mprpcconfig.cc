#include "mprpcconfig.h"

void MprpcConfig::Loadconfig(const char *config_file)
{
    FILE *pf = fopen(config_file, "r");
    if (nullptr == pf)
    {
        perror("<configfile> is open default\n");
        exit(EXIT_FAILURE);
    }
    while (!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf,512,pf);
        std::string src_buf(buf);

        //去掉前面多余空格
        int  index =src_buf.find_first_not_of(" ");
        if(index!=-1)
        {
            src_buf=src_buf.substr(index,src_buf.size()-index);
        }
        //去掉后面多余空格
        index=src_buf.find_last_not_of(" ");
        if(index!=-1)
        {
            src_buf=src_buf.substr(0,index+1);
        }
        //判断注释
        if(src_buf[0]=='#'||src_buf.empty())
        {
            continue;
        }
        //解析配置项
        index=src_buf.find('=');
        if(index==-1)
        {
            //该条配置项不合法
            continue;
        }
        std::string key=src_buf.substr(0,index);
        std::string value=src_buf.substr(index+1,src_buf.size()-index);

        m_configMap.insert(std::pair<std::string,std::string>(key,value));


    }
}
std::string MprpcConfig::Load(const std::string &key)
{
    auto it=m_configMap.find(key);
    if(it==m_configMap.end())
    {
        return nullptr;
    }
    return it->second;
}