#include "prime_utils.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace std::chrono;

string timestamp_now() {
    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    tm tm{};
#ifdef _MSC_VER
    localtime_s(&tm, &t);
#else
    tm = *localtime(&t);
#endif
    ostringstream ss;
    ss << put_time(&tm, "%F %T");
    return ss.str();
}

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return n == 2;
    int r = static_cast<int>(sqrt(n));
    for (int i = 3; i <= r; i += 2)
        if (n % i == 0) return false;
    return true;
}
