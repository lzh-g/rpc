#ifndef _RPC_CHANNEL_H_
#define _RPC_CHANNEL_H_

#include "zookeeperutil.h"

#include <google/protobuf/service.h>

// 客户端进行方法调用时，统一接收的类
class KRpcChannel : public google::protobuf::RpcChannel
{
public:
    // 构造函数，支持延迟连接
    KRpcChannel(bool connectNow);

    virtual ~KRpcChannel() {}

    // RPC调用核心方法，负责将客户端请求序列化并发送到服务端，提示接收服务端响应
    void CallMethod(const google::protobuf::MethodDescriptor *method, google::protobuf::RpcController *controller, const google::protobuf::Message *request, google::protobuf::Message *response, google::protobuf::Closure *done) override;

private:
    // 创建新的socket连接
    bool newConnect(const char *ip, uint16_t port);

    // 从zookeeper查询服务地址
    std::string QueryServiceHost(ZkClient *zkclient, std::string service_name, std::string method_name, int &idx);

private:
    int m_clientfd;
    std::string service_name;
    std::string m_ip;
    uint16_t m_port;
    std::string method_name;
    int m_idx; // 划分ip和port下标
};

#endif // !_RPC_CHANNEL_H_