#ifndef _RPC_PROVIDER_H_
#define _RPC_PROVIDER_H_

#include "zookeeperutil.h"

#include <google/protobuf/service.h>
#include <google/protobuf/descriptor.h>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>

#include <functional>
#include <string>
#include <unordered_map>

class RpcProvider
{
public:
    // 析构，退出事件循环
    ~RpcProvider();
    // 供外部使用，发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);

    // 启动rpc服务节点，提供rpc远程网络服务调用
    void Run();

private:
    // 连接回调函数，处理客户端连接事件
    void OnConnection(const muduo::net::TcpConnectionPtr &conn);

    // 消息回调函数，处理客户端发送的RPC请求
    void OnMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buffer, muduo::Timestamp receive_time);

    // 发送RPC响应给客户端
    void SendRpcResponse(const muduo::net::TcpConnectionPtr &conn, google::protobuf::Message *response);

private:
    struct ServiceInfo
    {
        // 服务类，由protobuf生成，继承自google::protobuf::Service，这样可通过基类指针指向子类对象实现多态
        google::protobuf::Service *service;
        // 服务类的描述信息
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor *> method_map;
    };
    muduo::net::EventLoop event_loop;
    std::unordered_map<std::string, ServiceInfo> service_map; // 保存服务对象和rpc方法
};

#endif // !_RPC_PROVIDER_H_