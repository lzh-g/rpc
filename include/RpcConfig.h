#ifndef _RPC_CONFIG_H_
#define _RPC_CONFIG_H_

#include <unordered_map>
#include <string>

class RpcConfig
{
public:
    // 加载配置文件
    void LoadConfigFile(const char *config_file);

    // 查找key对应的value
    std::string Load(const std::string &key);

private:
    // 去掉字符串前后的空格
    void Trim(std::string &read_buf);

    std::unordered_map<std::string, std::string> config_map;
};

#endif