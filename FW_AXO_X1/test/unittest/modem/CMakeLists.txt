cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(modem)

FILE(GLOB app_sources src/test_lte_monitor.cpp
                      src/stubs.cpp
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/modem/lte_monitor.cpp
    ../../common/src/rtos/mbox.cpp
    ../../common/src/rtos/task.cpp
    ../../common/src/rtos/timer.cpp
)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

