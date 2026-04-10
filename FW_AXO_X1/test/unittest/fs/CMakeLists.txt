cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(data)

FILE(GLOB app_sources src/stubs.cpp
                      src/test_flashfs.cpp
)
target_sources(app PRIVATE ${app_sources}
	../../common/src/fs/flashfs.cpp
)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
#zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

