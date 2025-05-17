#include "RpcApplication.h"

#include <cstdlib>
#include <unistd.h>

RpcConfig RpcApplication::m_config; // 全局配置对象

std::mutex RpcApplication::m_mutex; // 线程安全互斥锁

RpcApplication *RpcApplication::m_application = nullptr; // 单例对象指针，初始为空

void RpcApplication::Init(int argc, char **argv)
{
    // 命令行参数少于2个
    if (argc < 2)
    {
        std::cout << "格式：command -i <配置文件路径>" << std::endl;
        exit(EXIT_FAILURE);
    }

    int o;
    std::string config_file;
    // 使用getopt解析命令行参数，-i表示指定配置文件
    while (-1 != (o = getopt(argc, argv, "i:")))
    {
        switch (o)
        {
        case 'i':
            // 参数是-i，后面的值就是配置文件路径
            config_file = optarg; // 将配置文件路径保存到config_file
            break;

        case '?':
            // 出现未知参数，提示正确格式并退出
            std::cout << "格式：command -i <配置文件路径>" << std::endl;
            exit(EXIT_FAILURE);
            break;

        case ':':
            // -i后面没有跟参数，提示正确格式并退出
            std::cout << "格式：command -i <配置文件路径>" << std::endl;
            exit(EXIT_FAILURE);
            break;

        default:
            break;
        }
    }

    m_config.LoadConfigFile(config_file.c_str());
}

RpcApplication &RpcApplication::GetInstance()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_application == nullptr)
    {
        m_application = new RpcApplication();
        atexit(deleteInstance); // 注册atexit函数，程序退出时自动销毁单例对象
    }
    return *m_application;
}

void RpcApplication::deleteInstance()
{
    if (m_application)
    {
        delete m_application;
    }
}

RpcConfig &RpcApplication::GetConfig()
{
    return m_config;
}
