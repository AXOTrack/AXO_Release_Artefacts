cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(util)

FILE(GLOB app_sources #src/test_certmgr.cpp
                      src/test_popcount.cpp
                      src/test_hash.cpp
                      src/test_ecdsasign.cpp
                      src/test_keygen.cpp
                      src/stubs.cpp
)
target_sources(app PRIVATE ${app_sources}
	#../../common/src/util/certmgr.cpp
	../../common/src/util/popcount.cpp
    ../../common/src/util/hash.cpp
    ../../common/src/util/ecdsasign.cpp
    ../../common/src/util/keygen.cpp
)

# NOTE: certmgr is only temporary source, will be removed in the furture if migration to Thingsboard
#       has been completed -> Therefore no unit test neccessary

zephyr_include_directories(../../include)
zephyr_include_directories(../../common/src)
zephyr_include_directories(../../app-axo-x1/src)

zephyr_compile_options(-w -Wno-pointer-arith -Wno-unused-variable -Wno-psabi)

