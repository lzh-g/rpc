# 获取服务端源文件
file(GLOB SERVER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# 获取protobuf生成的.cc
file(GLOB PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf/*.pb.cc)

# 创建服务端可执行文件
add_executable(server ${SERVER_SRCS} ${PROTO_SRCS})

# 设置包含目录
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)

# 链接必要的库
target_link_libraries(server rpc_core ${LIBS})

# 设置编译选项
target_compile_options(server PRIVATE -std=c++11 -Wall)

# 设置可执行文件输出目录
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)