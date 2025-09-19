#include "config.h"
#include <fstream>
#include <string>

int THREADS = 4;
int MAX_N = 1000;

void read_config(const std::string& fname) {
    std::ifstream f(fname);
    if (!f) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.find("threads=") == 0) THREADS = std::stoi(line.substr(8));
        else if (line.find("max=") == 0) MAX_N = std::stoi(line.substr(4));
    }
}
