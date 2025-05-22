#include "zookeeperutil.h"
#include "RpcApplication.h"
#include "RpcLogger.h"

#include <mutex>
#include <condition_variable>

std::mutex cv_mutex;        // 全局锁，保护共享变量的线程安全
std::condition_variable cv; // 条件变量，用于线程间通信
bool is_connected = false;  // 标记zookeeper客户端是否连接成功

// 全局的watcher观察器，用于接收zookeeper服务器的通知
void globel_watcher(zhandle_t *zh, int type, int status, const char *path, void *watcherCtx)
{
    // 回调消息类型和会话相关的事件
    if (type == ZOO_SESSION_EVENT)
    {
        // zookeeper客户端和服务器连接成功
        if (status == ZOO_CONNECTED_STATE)
        {
            std::lock_guard<std::mutex> lock(cv_mutex); // 加锁保护
            is_connected = true;                        // 标记连接成功
        }
    }
    cv.notify_all(); // 通知所有等待的线程
}

ZkClient::ZkClient() : m_zhandle(nullptr)
{
}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle); // 关闭zookeeper连接
    }
}

void ZkClient::Start()
{
    // 从配置文件中读取zookeeper服务器的IP和端口
    std::string host = RpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = RpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port; // 拼接字符串

    /**
     * zookeeper_mt: 多线程版本
     * zookeeper的API客户端程序提供了3个线程：
     * 1. API调用线程
     * 2. 网络I/O线程(使用pthread_create和poll)
     * 3. watcher回调线程(使用pthread_create)
     */

    // 使用zookeeper_init初始化一个zookeeper客户端对象，异步建立与服务器的连接
    m_zhandle = zookeeper_init(connstr.c_str(), globel_watcher, 6000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle)
    {
        LOG(ERROR) << "zookeeper_init error";
        exit(EXIT_FAILURE);
    }

    // 等待连接成功
    std::unique_lock<std::mutex> lock(cv_mutex);
    cv.wait(lock, []
            { return is_connected; });     // 阻塞等待，直到连接成功
    LOG(INFO) << "zookeeper_init success"; // 记录日志，表示连接成功
}

void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128]; // 用于存储创建的节点路径
    int bufferlen = sizeof(path_buffer);

    // 检查节点是否已经存在
    int flag = zoo_exists(m_zhandle, path, 0, nullptr);
    // 节点不存在
    if (flag == ZNONODE)
    {
        // 创建指定的zookeeper节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        // 创建成功
        if (flag == ZOK)
        {
            LOG(INFO) << "znode create success... path:" << path;
        }
        else
        {
            LOG(ERROR) << "znode create failed... path: " << path;
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char *path)
{
    char buf[64]; // 用于存储节点数据
    int bufferlen = sizeof(buf);

    // 获取指定节点的数据
    std::cout << path << std::endl;
    int flag = zoo_get(m_zhandle, path, 0, buf, &bufferlen, nullptr);
    // 获取失败
    if (flag != ZOK)
    {
        LOG(ERROR) << "zoo_get error";
        return "";
    }
    else // 获取成功
    {
        return buf;
    }
    return "";
}
