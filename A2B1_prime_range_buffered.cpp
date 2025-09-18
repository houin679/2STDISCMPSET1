// prime_range_buffered.cpp
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

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return n == 2;
    int r = static_cast<int>(sqrt(n));
    for (int i = 3; i <= r; i += 2)
        if (n % i == 0) return false;
    return true;
}

vector<pair<int, string>> found; // pair<number, message>

void worker_range(int id, int start, int end) {
    vector<pair<int, string>> local;
    for (int n = start; n <= end; ++n) {
        if (is_prime(n)) {
            ostringstream ss;
            ss << "[" << timestamp_now() << "] Thread-" << id << " found prime: " << n;
            local.emplace_back(n, ss.str());
        }
    }
    // push local to global
    lock_guard<mutex> lg(results_mutex);
    for (auto& p : local) found.push_back(p);
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

int main() {
    read_config("config.txt");
    cout << "Run start: " << timestamp_now() << "\n";
    vector<thread> threads;
    int total = MAX_N - 1; // 2..MAX_N
    int per = total / THREADS;
    int rem = total % THREADS;
    int low = 2;
    for (int i = 0; i < THREADS; ++i) {
        int cnt = per + (i < rem ? 1 : 0);
        int high = (cnt > 0) ? (low + cnt - 1) : (low - 1);
        threads.emplace_back(worker_range, i + 1, low, max(1, high));
        low = high + 1;
    }
    for (auto& t : threads) t.join();

    // sort results by number
    sort(found.begin(), found.end(), [](auto& a, auto& b) { return a.first < b.first; });

    // print all at once
    cout << "\n--- Buffered output (after all threads finished) ---\n";
    for (auto& p : found) cout << p.second << "\n";
    cout << "\nRun end: " << timestamp_now() << "\n";
    return 0;
}
