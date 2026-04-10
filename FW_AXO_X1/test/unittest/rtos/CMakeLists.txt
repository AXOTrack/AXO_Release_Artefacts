# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(integration)

# NOTE: sema and mutex makes problems !!  To be investigated

#FILE(GLOB app_sources src/*.cpp)
FILE(GLOB app_sources   src/test_mempool.cpp
                        src/test_mbox.cpp
                        src/test_mutex.cpp
                        src/test_mpool.cpp
                        src/test_mpelem.cpp
                        src/test_task.cpp
                        src/test_timer.cpp
                        src/test_sema.cpp
                        src/test_mqueue.cpp
                        src/stubs.cpp)
target_sources(app PRIVATE ${app_sources}
    ../../common/src/rtos/mpelem.cpp
    ../../common/src/rtos/mempool.h
    ../../common/src/rtos/mqueue.h
    ../../common/src/rtos/mpool.cpp
    ../../common/src/rtos/mbox.cpp
    ../../common/src/rtos/sema.cpp
    ../../common/src/rtos/mutex.cpp
    ../../common/src/rtos/task.cpp
    ../../common/src/rtos/timer.cpp
)

zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)
