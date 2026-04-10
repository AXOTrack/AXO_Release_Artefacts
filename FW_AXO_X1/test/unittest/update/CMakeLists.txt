cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(data)

FILE(GLOB app_sources src/test_fsm.cpp
                      #src/test_fwinfo.cpp
                      #src/test_update.cpp
                      #src/test_update_ctrl.cpp
                      src/stubs.cpp
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/update/fsm.cpp
	#../../common/src/update/fwinfo.cpp
	#../../common/src/update/update.cpp
	#../../common/src/update/update_ctrl.cpp
    ../../common/src/rtos/mbox.cpp
    ../../common/src/rtos/task.cpp
    ../../common/src/rtos/timer.cpp
)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

