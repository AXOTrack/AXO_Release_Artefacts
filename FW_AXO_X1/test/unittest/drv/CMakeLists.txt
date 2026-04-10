cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(bus)

FILE(GLOB app_sources src/stubs.cpp
                      src/test_adc_adconverter.cpp
                      src/test_adc_batt_mgr.cpp
                      src/test_i2c_shtcx.cpp
                      src/test_pm_boost.cpp
                      src/test_pwm_pwm.cpp
                      src/test_spi_adxl37x.cpp
                      src/test_spi_fram.cpp                      
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/drv/adc/adconverter.cpp
	../../common/src/drv/adc/batt_mgr.cpp
	../../common/src/drv/i2c/shtcx.cpp
    ../../common/src/drv/pm/boost.cpp
	../../common/src/drv/pwm/pwm.cpp
	../../common/src/drv/spi/adxl37x.cpp
	../../common/src/drv/spi/fram.cpp
    ../../common/src/rtos/mutex.cpp
    ../../common/src/rtos/mbox.cpp
    ../../common/src/rtos/mpool.cpp
    ../../common/src/rtos/task.cpp
    ../../common/src/rtos/timer.cpp)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
#zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

