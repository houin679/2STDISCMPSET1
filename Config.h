#pragma once

// config.h - Configuration header
#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

struct Config {
    int num_threads;
    int search_limit;

    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);

                if (key == "num_threads") {
                    num_threads = std::stoi(value);
                }
                else if (key == "search_limit") {
                    search_limit = std::stoi(value);
                }
            }
        }
        return true;
    }
};

#endif
