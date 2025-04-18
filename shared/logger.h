#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>

inline void log(const std::string& msg) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    std::cout << "[" << std::put_time(&localTime, "%H:%M:%S") << "] " << msg << std::endl;
}
