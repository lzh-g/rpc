# 获取客户端源文件
file(GLOB CLIENT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# 获取protobuf生成的.cc
file(GLOB PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/*.pb.cc)

# 创建客户端可执行文件
add_executable(client ${CLIENT_SRCS} ${RROTO_SRCS})

# 连接必要的库
target_link_libraries(client rpc_core ${LIBS})

# 设置编译选项
target_compile_options(client PRIVATE -std=c++11 -Wall)

# 设置可执行文件输出目录
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)