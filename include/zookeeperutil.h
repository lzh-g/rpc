#ifndef _ZOOKEEPER_UTIL_H_
#define _ZOOKEEPER_UTIL_H_

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

// 封装的zookeeper客户端
class ZkClient
{
public:
    ZkClient();

    ~ZkClient();

    // ZkClient启动连接ZkServer
    void Start();

    // 在ZkServer中创建一个节点
    void Create(const char *path, const char *data, int datalen, int state = 0);

    // 根据参数指定的znode节点路径，获取znode节点值
    std::string GetData(const char *path);

private:
    zhandle_t *m_zhandle; // zookeeper客户端句柄
};

#endif // !_ZOOKEEPER_UTIL_H_