#ifndef _RPC_CONTROLLER_H_
#define _RPC_CONTROLLER_H_

#include <google/protobuf/service.h>
#include <string>

// 用于描述RPC调用的控制器，跟踪RPC方法调用的状态、错误信息并提供控制功能(如取消调用)
class KRpcController : public google::protobuf::RpcController
{
public:
    // 初始化控制器状态
    KRpcController();

    // 重置控制器状态
    void Reset();

    // 判断当前RPC调用是否失败
    bool Failed() const;

    // 获取错误信息
    std::string ErrorText() const;

    // 设置RPC调用失败，记录失败原因
    void SetFailed(const std::string &reason);

    // 未实现的功能
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure *collback);

private:
    bool m_failed;         // RPC方法执行过程中的状态
    std::string m_errText; // RPC方法执行过程中的错误信息
};
#endif //!_RPC_CONTROLLER_H_