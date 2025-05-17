#ifndef _RPC_APPLICATION_H_
#define _RPC_APPLICATION_H_

#include "RpcConfig.h"
#include "KRpcChannel.h"
#include "KRpcController.h"

#include <mutex>

// RPC基础类，负责框架的初始化操作
class RpcApplication
{
public:
    // 初始化函数，解析命令行参数并加载配置文件
    static void Init(int argc, char **argv);
    // 获取单例对象的引用，保证全局只有一个实例
    static RpcApplication &GetInstance();
    // 程序退出自动调用，销毁单例对象
    static void deleteInstance();
    // 获取全局配置对象的引用
    static RpcConfig &GetConfig();

private:
    RpcApplication() {}
    ~RpcApplication() {}
    RpcApplication(const RpcApplication &) = delete;
    RpcApplication(RpcApplication &&) = delete;

private:
    static RpcConfig m_config;
    static RpcApplication *m_application; // 全局唯一单例访问对象
    static std::mutex m_mutex;
};

#endif // !_RPC_APPLICATION_H_