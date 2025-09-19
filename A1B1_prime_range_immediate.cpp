#include "modes.h"
#include "config.h"
#include "prime_utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

mutex cout_mutex;

void worker_range_immediate(int id, int start, int end) {
    for (int n = start; n <= end; ++n) {
        if (is_prime(n)) {
            lock_guard<mutex> lg(cout_mutex);
            cout << "[" << timestamp_now() << "] Thread-" << id
                << " found prime: " << n << "\n";
        }
    }
}

void run_range_immediate() {
    cout << "Run start: " << timestamp_now() << "\n";
    vector<thread> threads;
    int total = MAX_N - 1;
    int per = total / THREADS;
    int rem = total % THREADS;
    int low = 2;
    for (int i = 0; i < THREADS; ++i) {
        int cnt = per + (i < rem ? 1 : 0);
        int high = (cnt > 0) ? (low + cnt - 1) : (low - 1);
        threads.emplace_back(worker_range_immediate, i + 1, low, max(1, high));
        low = high + 1;
    }
    for (auto& t : threads) t.join();
    cout << "Run end: " << timestamp_now() << "\n";
}
