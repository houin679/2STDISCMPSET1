/*
#include "modes.h"
#include "config.h"
#include "prime_utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <cmath>

using namespace std;

mutex cout_mutex_div;

void test_divisors_for_number(int n) {
    if (n <= 1) return;
    if (n <= 3) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }
    if (n % 2 == 0) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found composite: " << n
            << " (divisible by 2)\n";
        return;
    }
    int sqrt_n = static_cast<int>(sqrt(n));
    vector<int> divisors;
    for (int d = 3; d <= sqrt_n; d += 2) divisors.push_back(d);
    if (divisors.empty()) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }

    atomic<bool> found_div(false);
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
                    lock_guard<mutex> lg(cout_mutex_div);
                    cout << "[" << timestamp_now() << "] Thread-" << (i + 1)
                        << " found composite: " << n << " (divisible by " << d << ")\n";
                    return;
                }
            }
            });
    }

    for (auto& t : workers) t.join();

    if (!found_div.load()) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main confirmed prime: " << n << "\n";
    }
}

void run_divisibility_immediate() {
    cout << "Run start: " << timestamp_now() << "\n";
    for (int n = 2; n <= MAX_N; ++n) {
        test_divisors_for_number(n);
    }
    cout << "Run end: " << timestamp_now() << "\n";
}


*/






/*




#include "modes.h"
#include "config.h"
#include "prime_utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <cmath>

using namespace std;

mutex cout_mutex_div;

void test_divisors_for_number(int n) {
    if (n <= 1) return;
    if (n <= 3) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }
    if (n % 2 == 0) {
        // Composite output removed
        // lock_guard<mutex> lg(cout_mutex_div);
        // cout << "[" << timestamp_now() << "] Thread-main found composite: " << n
        //     << " (divisible by 2)\n";
        return;
    }
    int sqrt_n = static_cast<int>(sqrt(n));
    vector<int> divisors;
    for (int d = 3; d <= sqrt_n; d += 2) divisors.push_back(d);
    if (divisors.empty()) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }

    atomic<bool> found_div(false);
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
                    // Composite output removed
                    // lock_guard<mutex> lg(cout_mutex_div);
                    // cout << "[" << timestamp_now() << "] Thread-" << (i + 1)
                    //     << " found composite: " << n << " (divisible by " << d << ")\n";
                    return;
                }
            }
            });
    }

    for (auto& t : workers) t.join();

    if (!found_div.load()) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main confirmed prime: " << n << "\n";
    }
}

void run_divisibility_immediate() {
    cout << "Run start: " << timestamp_now() << "\n";
    for (int n = 2; n <= MAX_N; ++n) {
        test_divisors_for_number(n);
    }
    cout << "Run end: " << timestamp_now() << "\n";
}


*/















#include "modes.h"
#include "config.h"
#include "prime_utils.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <cmath>

using namespace std;

mutex cout_mutex_div;

void test_divisors_for_number(int n) {
    if (n <= 1) return;
    if (n <= 3) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }
    if (n % 2 == 0) {
        // Composite output removed
        return;
    }
    int sqrt_n = static_cast<int>(sqrt(n));
    vector<int> divisors;
    for (int d = 3; d <= sqrt_n; d += 2) divisors.push_back(d);
    if (divisors.empty()) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-main found prime: " << n << "\n";
        return;
    }

    atomic<bool> found_div(false);
    atomic<int> last_checker(-1);  // track last thread id
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
                    return;
                }
            }
            // if this thread finishes checking, record it as the last checker
            last_checker.store(i);
            });
    }

    for (auto& t : workers) t.join();

    if (!found_div.load()) {
        lock_guard<mutex> lg(cout_mutex_div);
        cout << "[" << timestamp_now() << "] Thread-" << (last_checker.load() + 1)
            << " confirmed prime: " << n << "\n";
    }
}

void run_divisibility_immediate() {
    cout << "Run start: " << timestamp_now() << "\n";
    for (int n = 2; n <= MAX_N; ++n) {
        test_divisors_for_number(n);
    }
    cout << "Run end: " << timestamp_now() << "\n";
}