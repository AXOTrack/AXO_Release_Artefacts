cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(pm)

FILE(GLOB app_sources src/stubs.cpp
                      src/test_boot_cnt.cpp                      
                      src/test_power_mgr.cpp                      
                      src/test_power_mgr_x1.cpp                      
                      src/test_reset_ctrl.cpp                      
                      src/test_reset_reason.cpp                      
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/pm/boot_cnt.cpp
	../../common/src/pm/power_mgr.cpp
	../../common/src/pm/power_mgr_x1.cpp
	../../common/src/pm/reset_ctrl.cpp
	../../common/src/pm/reset_reason.cpp
    ../../common/src/rtos/mutex.cpp
    ../../common/src/rtos/mbox.cpp
    ../../common/src/rtos/mpool.cpp
    ../../common/src/rtos/task.cpp
    ../../common/src/rtos/timer.cpp
)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

