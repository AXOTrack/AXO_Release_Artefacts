cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(net)


# NOTE: stubs.cpp must be first module, for linker links stub functions 
# before lib functions
FILE(GLOB app_sources src/stubs.cpp    
                      src/test_http_connector.cpp
                      src/test_lte_connector.cpp
                      src/test_mqtt_connector.cpp
                      
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/net/http_connector.cpp
	../../common/src/net/lte_connector.cpp
	../../common/src/net/mqtt_connector.cpp
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

