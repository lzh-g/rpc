#include "../protobuf/user.pb.h"
#include "RpcApplication.h"
#include "RpcProvider.h"

#include <iostream>
#include <string>

/**
 * UserService原本是一个本地服务，提供了两个本地方法：Login 和 GetFriendLists。
 * 现在通过 RPC 框架，这些方法可以被远程调用。
 */
class UserService : public user::UserServiceRpc
{
    // 本地登录方法，处理实际业务逻辑
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    /**
     * 重写基类UserServiceRpc的虚函数，这些方法会被RPC框架调用
     * 1. 调用者(caller)通过RPC框架发送Login请求
     * 2. 服务提供者(callee)接收到请求后，调用下面重写的Login方法
     * caller => Login(LoginRequest) => muduo => callee
     * callee => Login(LoginRequest)
     */
    void Login(google::protobuf::RpcController *controller, const user::LoginRequest *request, user::LoginResponse *response, google::protobuf::Closure *done) override
    {
        // 从请求中获取用户名和密码
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 调用本地业务逻辑处理登录
        bool login_result = Login(name, pwd);

        // 将响应结果写入response对象
        user::ResultCode *code = response->mutable_result();
        code->set_errcode(0);                // 设置错误码为0
        code->set_errmsg("");                // 设置错误信息为空
        response->set_success(login_result); // 设置登录结果

        // 执行回调操作，框架会自动将响应序列化并发送给调用者
        done->Run();
    }
};

int main(int argc, char *argv[])
{
    // 调用框架初始化，解析命令行参数并加载配置文件
    RpcApplication::Init(argc, argv);

    // 创建一个RPC服务提供者对象
    RpcProvider provider;

    // 将UserService对象发布到RPC节点上，使其可以被远程调用
    provider.NotifyService(new UserService());

    // 启动RPC服务节点，进入阻塞状态，等待远程的RPC调用请求
    provider.Run();

    return 0;
}
