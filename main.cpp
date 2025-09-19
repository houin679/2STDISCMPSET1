#include <iostream>
#include "config.h"
#include "modes.h"

using namespace std;

int main() {
    read_config("config.txt");

    cout << "Select mode:\n";
    cout << "1. Range division : Immediate printing\n";
    cout << "2. Range division : Buffered printing\n";
    cout << "3. Divisibility-threading : Immediate printing\n";
    cout << "4. Divisibility-threading : Buffered printing\n";
    cout << "Choice: ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1: run_range_immediate(); break;
    case 2: run_range_buffered(); break;
    case 3: run_divisibility_immediate(); break;
    case 4: run_divisibility_buffered(); break;
    default: cout << "Invalid choice\n"; break;
    }

    return 0;
}