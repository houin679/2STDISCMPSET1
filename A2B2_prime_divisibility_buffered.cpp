// prime_divisibility_buffered.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <atomic>
#include <algorithm>

using namespace std;
using namespace std::chrono;

int THREADS = 4;
int MAX_N = 1000;
mutex results_mutex;

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

void read_config(const string& fname) {
    ifstream f(fname);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.find("threads=") == 0) THREADS = stoi(line.substr(8));
        else if (line.find("max=") == 0) MAX_N = stoi(line.substr(4));
    }
}

bool is_prime_div_threaded(int n, int threads_available) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return (n == 2);
    int sqrt_n = static_cast<int>(sqrt(n));
    vector<int> divisors;
    for (int d = 3; d <= sqrt_n; d += 2) divisors.push_back(d);
    if (divisors.empty()) return true;

    atomic<bool> found_div(false);
    int total_divs = (int)divisors.size();
    int chunks = min(threads_available, total_divs);
    int per = total_divs / chunks;
    int rem = total_divs % chunks;
    int idx = 0;
    vector<thread> workers;

    // results aggregator per number: if any worker finds divisor -> composite
    for (int i = 0; i < chunks; ++i) {
        int cnt = per + (i < rem ? 1 : 0);
        int start = idx;
        int end = idx + cnt - 1;
        idx += cnt;
        workers.emplace_back([&, start, end]() {
            for (int j = start; j <= end; ++j) {
                if (found_div.load()) break;
                int d = divisors[j];
                if (n % d == 0) {
                    found_div.store(true);
                    break;
                }
            }
            });
    }

    for (auto& t : workers) if (t.joinable()) t.join();
    return !found_div.load();
}

int main() {
    read_config("config.txt");
    cout << "Run start: " << timestamp_now() << "\n";
    vector<pair<int, string>> results; // (n, message)
    for (int n = 2; n <= MAX_N; ++n) {
        bool prime = is_prime_div_threaded(n, THREADS);
        ostringstream ss;
        ss << "[" << timestamp_now() << "] ";
        if (prime) ss << "Prime: " << n;
        else ss << "Composite: " << n;
        lock_guard<mutex> lg(results_mutex);
        results.emplace_back(n, ss.str());
    }

    // sort and print after all computations
    sort(results.begin(), results.end(), [](auto& a, auto& b) { return a.first < b.first; });
    cout << "\n--- Buffered output (after all computations) ---\n";
    for (auto& p : results) cout << p.second << "\n";
    cout << "\nRun end: " << timestamp_now() << "\n";
    return 0;
}
