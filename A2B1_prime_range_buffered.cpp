#include "modes.h"
#include "config.h"
#include "prime_utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sstream>

using namespace std;

mutex results_mutex_range;
vector<pair<int, string>> found_range;

void worker_range_buffered(int id, int start, int end) {
    vector<pair<int, string>> local;
    for (int n = start; n <= end; ++n) {
        if (is_prime(n)) {
            ostringstream ss;
            ss << "[" << timestamp_now() << "] Thread-" << id
                << " found prime: " << n;
            local.emplace_back(n, ss.str());
        }
    }
    lock_guard<mutex> lg(results_mutex_range);
    for (auto& p : local) found_range.push_back(p);
}

void run_range_buffered() {
    cout << "Run start: " << timestamp_now() << "\n";
    vector<thread> threads;
    int total = MAX_N - 1;
    int per = total / THREADS;
    int rem = total % THREADS;
    int low = 2;
    for (int i = 0; i < THREADS; ++i) {
        int cnt = per + (i < rem ? 1 : 0);
        int high = (cnt > 0) ? (low + cnt - 1) : (low - 1);
        threads.emplace_back(worker_range_buffered, i + 1, low, max(1, high));
        low = high + 1;
    }
    for (auto& t : threads) t.join();

    sort(found_range.begin(), found_range.end(),
        [](auto& a, auto& b) { return a.first < b.first; });

    cout << "\n--- Buffered output ---\n";
    for (auto& p : found_range) cout << p.second << "\n";
    cout << "\nRun end: " << timestamp_now() << "\n";
}
