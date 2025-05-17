#include "RpcConfig.h"
#include <memory>

// 加载配置文件，解析配置文件中的键值对
void RpcConfig::LoadConfigFile(const char *config_file)
{
    // 使用智能指针管理文件指针，确保文件在退出时自动关闭
    std::unique_ptr<FILE, decltype(&fclose)> pf(fopen(config_file, "r"), &fclose);

    if (pf == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    char buf[1024];

    // 逐行读取
    while (fgets(buf, 1024, pf.get()) != nullptr)
    {
        std::string read_buf(buf);

        // 忽略注释行(#开头)和空行
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        // 查找键值对分隔符'='
        int index = read_buf.find('=');
        if (index == -1)
        {
            // 没找到
            continue;
        }

        // 提取key
        std::string key = read_buf.substr(0, index);
        Trim(key); // 去掉key前后的空格

        // 查找行尾换行符
        int endindex = read_buf.find('\n', index);
        // 提取value
        std::string value = read_buf.substr(index + 1, endindex - index - 1);
        Trim(value); // 去掉value前后空格

        // 键值对存入map
        config_map.insert({key, value});
    }
}

std::string RpcConfig::Load(const std::string &key)
{
    auto it = config_map.find(key); // 在map中查找key
    if (it == config_map.end())
    {
        // 没找到，返回空串
        return "";
    }

    return it->second;
}

void RpcConfig::Trim(std::string &read_buf)
{
    // 找到字符串第一个不是空格的字符
    int index = read_buf.find_first_not_of(' ');
    if (index != -1)
    {
        read_buf = read_buf.substr(index, read_buf.size() - index); // 截取字符串
    }

    // 找到最后一个不是空格的字符
    index = read_buf.find_last_not_of(' ');
    if (index != -1)
    {
        read_buf = read_buf.substr(0, index + 1); // 截取字符串
    }
}
