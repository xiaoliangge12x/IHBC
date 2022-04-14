# 指定CMAKE最低版本
cmake_minimum_required(VERSION 3.4.1)

# 配置编译器(注，一定要放在工程之前)， 否则不生效
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# 工程名
project(tsm)

# 显示编译命令
set(CMAKE_VERBOSE_MAKEFILE OFF)

# 指定生成路径
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output/bin)

# 调整编译模式
set(CMAKE_BUILD_TYPE "Debug")

# 设置LOG打印
# add_compile_options(-D_NEED_LOG)

# 添加选项
add_definitions(-w)
set(CMAKE_C_FLAGS -pthread)

# 进入下一层目录
add_subdirectory(src)
