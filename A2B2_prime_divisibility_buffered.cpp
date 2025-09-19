#include "modes.h"
#include "config.h"
#include "prime_utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;

mutex results_mutex_div;

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

    for (int i = 0; i < chunks; ++i) {
        int cnt = per + (i < rem ? 1 : 0);
        int start = idx;
        int end = idx + cnt - 1;
        idx += cnt;
        workers.emplace_back([&, start, end]() {
            for (int j = start; j <= end; ++j) {
                if (found_div.load()) break;
                if (n % divisors[j] == 0) {
                    found_div.store(true);
                    break;
                }
            }
            });
    }

    for (auto& t : workers) t.join();
    return !found_div.load();
}

void run_divisibility_buffered() {
    cout << "Run start: " << timestamp_now() << "\n";
    vector<pair<int, string>> results;

    for (int n = 2; n <= MAX_N; ++n) {
        bool prime = is_prime_div_threaded(n, THREADS);
        ostringstream ss;
        ss << "[" << timestamp_now() << "] ";
        if (prime) ss << "Prime: " << n;
        else ss << "Composite: " << n;

        lock_guard<mutex> lg(results_mutex_div);
        results.emplace_back(n, ss.str());
    }

    sort(results.begin(), results.end(),
        [](auto& a, auto& b) { return a.first < b.first; });

    cout << "\n--- Buffered output ---\n";
    for (auto& p : results) cout << p.second << "\n";
    cout << "\nRun end: " << timestamp_now() << "\n";
}
