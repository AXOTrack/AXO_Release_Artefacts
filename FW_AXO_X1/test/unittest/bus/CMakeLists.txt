cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(bus)

FILE(GLOB app_sources src/test_i2c.cpp
                      src/test_spi.cpp
                      #src/test_mb_client.cpp
                      #src/test_mb_server.cpp
                      src/stubs.cpp
                      src/test_zz_exit.cpp
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/bus/i2c/i2cbus.cpp
	../../common/src/bus/spi/spibus.cpp
    #../../common/src/bus/mb/modbus_client.cpp
	#../../common/src/bus/mb/modbus_server.cpp
)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
#zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

