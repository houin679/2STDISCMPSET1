#pragma once
// utils.h - Utility functions
#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath>

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now(); // Changed to system_clock
    auto time_t = std::chrono::system_clock::to_time_t(now); // Updated to match system_clock
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

bool isPrime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    for (int i = 3; i <= std::sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

void printStartEnd(const std::string& message) {
    std::cout << message << getCurrentTimestamp() << std::endl;
}

#endif
