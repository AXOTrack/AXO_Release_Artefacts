#define private public

#include "stubs.h"

Config* Config::inst() {
    static Config myInstance;
    return &myInstance;
}

Config::Config() { }
Config::~Config() { }
void Config::init() { }

KeyGen* KeyGen::inst() {
    static KeyGen myInstance;
    return &myInstance;
}

KeyGen::KeyGen() { }
KeyGen::~KeyGen() { }
void KeyGen::init() { }

WakeupCnt* WakeupCnt::inst() {
    static WakeupCnt myInstance;
    return &myInstance;
}

WakeupCnt::WakeupCnt() { }
WakeupCnt::~WakeupCnt() { }
void WakeupCnt::init() { }
uint32_t WakeupCnt::getSleepExitCount() {
    return 0;
}

uint32_t WakeupCnt::getPublishMeasCount() {
    return 0;
}

void LteConnector::registerListener(Mbox* listener) { }
void ADConverter::registerListener(uint8_t chmask, Mbox* listener) { }

FlashFS::FlashFS(const char* name) { }

FlashFS& FlashFS::inst() {
    static FlashFS myInstance("fs");
    return myInstance;
}

FlashFS::~FlashFS() { }

bool FlashFS::file_exists(const char* path) {
    return false;
}

void FlashFS::CheckAvailableSpace() { }