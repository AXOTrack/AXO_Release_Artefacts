cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(app-axo-x1)

FILE(GLOB app_sources src/test_version.cpp
                      src/test_main.cpp
                      src/stubs.cpp
)
target_sources(app PRIVATE ${app_sources}
	../../app-axo-x1/src/version.cpp
	../../app-axo-x1/src/main.cpp
)


zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

