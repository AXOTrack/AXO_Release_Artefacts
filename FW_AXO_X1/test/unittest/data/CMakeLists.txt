cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(data)

#FILE(GLOB app_sources src/*.cpp)
FILE(GLOB app_sources src/test_measurement.cpp
                      src/test_adxl_store.cpp
                      src/test_hw_metrics.cpp
                      src/test_hit_detect.cpp
                      src/test_metric.cpp
                      src/test_rms.cpp
                      src/test_rms_store.cpp
                      ##src/test_pdata.cpp
                      src/stubs.cpp
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/data/hit_detect.cpp
	../../common/src/data/metric.cpp
	../../common/src/data/hw_metrics.cpp
	../../common/src/data/measurement.cpp
	../../common/src/data/rms.cpp
	../../common/src/data/rms_store.cpp
	../../common/src/data/adxl_store.cpp
	##../../common/src/data/pdata.cpp
    ../../common/src/rtos/mbox.cpp
    ../../common/src/rtos/task.cpp
    ../../common/src/rtos/timer.cpp
)

##############################################################################################
#
# NOTE: Because of high complex dependencies module pdata will be tested in other test suite!
#
##############################################################################################


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

