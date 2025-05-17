cmake_minimum_required(VERSION 3.8)
project(rpc)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找protobuf包
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# 设置全局链接库
set(LIBS protobuf pthread zookeeper_mt muduo_net muduo_base glog)

# 添加子目录
add_subdirectory(src)
add_subdirectory(example)