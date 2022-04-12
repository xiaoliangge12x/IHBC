# 源文件
FILE(GLOB SRC_FILE "*.c" "base/*.c" "ihbc/*.c")

MESSAGE(${SRC_FILE})

# 指定头文件路径
set(INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE_PATH})

add_executable(ihbc ${SRC_FILE})
