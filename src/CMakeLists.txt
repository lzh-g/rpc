file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

file(GLOB PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/*.pb.cc)

# 创建动态库
add_library(rpc_core SHARED ${SRC_FILES} ${PROTO_SRCS})

# 链接protobuf库
target_link_libraries(rpc_core PUBLIC protobuf)

target_include_directories(rpc_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

# 添加编译选项
target_compile_options(rpc_core PRIVATE -std=c++11 -Wall)