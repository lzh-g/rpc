#include "RpcApplication.h"
#include "../protobuf/user.pb.h"
#include "KRpcController.h"
#include "RpcLogger.h"

#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

// 发送RPC请求的函数，模拟客户端调用远程服务
void send_request(int thread_id, std::atomic<int> &success_count, std::atomic<int> &fail_count)
{
    // 创建一个Userservice_Stub对象，用于调用远程的RPC方法
    user::UserServiceRpc_Stub stub(new KRpcChannel(false));

    // 设置RPC方法的请求参数
    user::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");

    // 定义RPC方法的响应参数
    user::LoginResponse response;
    KRpcController controller; // 创建控制器对象，用于处理RPC调用过程中的错误

    // 调用远程的Login方法
    stub.Login(&controller, &request, &response, nullptr);

    // 检查RPC调用是否成功
    if (controller.Failed())
    {
        // 调用失败
        std::cout << controller.ErrorText() << std::endl;
        fail_count++;
    }
    else
    {
        // 调用成功
        if (0 == response.result().errcode())
        {
            // 检查响应中的错误码
            std::cout << "rpc login response success: " << response.success() << std::endl;
            success_count++;
        }
        else
        {
            // 响应中有错误
            std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
            fail_count++;
        }
    }
}

int main(int argc, char *argv[])
{
    // 初始化RPC框架，解析命令行参数并加载配置文件
    RpcApplication::Init(argc, argv);

    // 创建日志对象
    RpcLogger logger("MyRPC");

    const int thread_count = 1000;     // 并发线程数
    const int requests_per_thread = 2; // 每个线程发送的请求数

    std::vector<std::thread> threads;  // 存储线程对象的容器
    std::atomic<int> success_count(0); // 成功请求的计数器
    std::atomic<int> fail_count(0);    // 失败请求的计数器

    auto start_time = std::chrono::high_resolution_clock::now(); // 记录测试开始时间

    for (int i = 0; i < thread_count; i++)
    {
        threads.emplace_back([argc, argv, i, &success_count, &fail_count, requests_per_thread]()
                             { for (int j = 0; j < requests_per_thread;j++)
                             {
                                 send_request(i, success_count, fail_count);
                             } });
    }

    // 等待所有线程执行完毕
    for (auto &t : threads)
    {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // 输出统计结果
    LOG(INFO) << "Total requests: " << thread_count * requests_per_thread;          // 总请求数
    LOG(INFO) << "Success count: " << success_count;                                // 成功请求数
    LOG(INFO) << "Fail count: " << fail_count;                                      // 失败请求数
    LOG(INFO) << "Elapsed time: " << elapsed.count() << " seconds";                 // 测试耗时
    LOG(INFO) << "QPS: " << (thread_count * requests_per_thread) / elapsed.count(); // 计算 QPS（每秒请求数）

    return 0;
}
