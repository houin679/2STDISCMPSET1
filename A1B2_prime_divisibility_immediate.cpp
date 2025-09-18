// prime_divisibility_immediate.cpp
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

using namespace std;
using namespace std::chrono;

int THREADS = 4;
int MAX_N = 1000;
mutex cout_mutex;

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

// For each candidate n, spawn up to THREADS worker threads to test divisors.
// Workers will print composite findings immediately (thread id + timestamp).
// If none find divisors, the last finishing worker prints that n is prime.

void read_config(const string& fname) {
    ifstream f(fname);
    if (!f) return;
    string line;
    while (getline(f, line)) {
        if (line.find("threads=") == 0) THREADS = stoi(line.substr(8));
        else if (line.find("max=") == 0) MAX_N = stoi(line.substr(4));
    }
}

void test_divisors_for_number(int n) {
    if (n <= 1) return;
    if (n <= 3) {
        lock_guard<mutex> lg(cout_mutex);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }
    if (n % 2 == 0) {
        lock_guard<mutex> lg(cout_mutex);
        cout << "[" << timestamp_now() << "] Thread-main found composite: " << n << " (divisible by 2)\n";
        return;
    }
    int sqrt_n = static_cast<int>(sqrt(n));
    vector<int> divisors;
    for (int d = 3; d <= sqrt_n; d += 2) divisors.push_back(d);
    if (divisors.empty()) {
        lock_guard<mutex> lg(cout_mutex);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }

    atomic<bool> found_div(false);
    atomic<int> remaining((int)divisors.size()); // number of divisor entries left to process across workers

    // Partition divisors into up to THREADS contiguous chunks
    int total_divs = (int)divisors.size();
    int chunks = min(THREADS, total_divs);
    int per = total_divs / chunks;
    int rem = total_divs % chunks;
    int idx = 0;
    vector<thread> workers;

    for (int i = 0; i < chunks; ++i) {
        int cnt = per + (i < rem ? 1 : 0);
        int start = idx;
        int end = idx + cnt - 1;
        idx += cnt;

        workers.emplace_back([&, i, start, end]() {
            for (int j = start; j <= end; ++j) {
                if (found_div.load()) break;
                int d = divisors[j];
                if (n % d == 0) {
                    found_div.store(true);
                    lock_guard<mutex> lg(cout_mutex);
                    cout << "[" << timestamp_now() << "] Thread-" << (i + 1) << " found composite: " << n << " (divisible by " << d << ")\n";
                    break;
                }
            }
            // decrement remaining and if this is the last to finish and no divisor found => prime
            if (remaining.fetch_sub((end - start + 1)) <= (end - start + 1)) {
                // this worker is effectively the one that causes remaining to reach <=0
                if (!found_div.load()) {
                    lock_guard<mutex> lg(cout_mutex);
                    cout << "[" << timestamp_now() << "] Thread-" << (i + 1) << " confirmed prime: " << n << "\n";
                }
            }
            });
    }

    for (auto& t : workers) if (t.joinable()) t.join();
}

int main() {
    read_config("config.txt");
    cout << "Run start: " << timestamp_now() << "\n";
    for (int n = 2; n <= MAX_N; ++n) {
        test_divisors_for_number(n);
    }
    cout << "Run end: " << timestamp_now() << "\n";
    return 0;
}
