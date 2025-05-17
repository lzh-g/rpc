#ifndef _RPC_LOGGER_H_
#define _RPC_LOGGER_H_

#include <glog/logging.h>
#include <string>

class RpcLogger
{
public:
    explicit RpcLogger(const char *arg)
    {
        google::InitGoogleLogging(arg);
        FLAGS_colorlogtostderr = true; // 启用彩色日志
        FLAGS_logtostderr = true;      // 默认输出标准错误
    }
    ~RpcLogger()
    {
        google::ShutdownGoogleLogging();
    }

    // 提供静态日志方法
    static void Info(const std::string &message)
    {
        LOG(INFO) << message;
    }
    static void Warning(const std::string &message)
    {
        LOG(WARNING) << message;
    }
    static void ERROR(const std::string &message)
    {
        LOG(ERROR) << message;
    }
    static void Fatal(const std::string &message)
    {
        LOG(FATAL) << message;
    }

private:
    // 禁用拷贝构造和赋值函数
    RpcLogger(const RpcLogger &) = delete;
    RpcLogger &operator=(const RpcLogger &) = delete;
};

#endif //!_RPC_LOGGER_H_