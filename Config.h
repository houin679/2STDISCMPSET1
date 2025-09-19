#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

extern int THREADS;
extern int MAX_N;

void read_config(const std::string& fname);

#endif
