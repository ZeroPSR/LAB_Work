#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_set>
using namespace std;

// HashTable class using chaining (vector of vectors)
class HashTable {
    static const int TABLE_SIZE = 100000;
    vector<vector<int>> table;

    int hash(int key) const {
        return key % TABLE_SIZE;
    }

public:
    // Constructor with vector input
    HashTable(const vector<int>& values) : table(TABLE_SIZE) {
        for (int val : values) {
            insert(val);
        }
    }

    // Insert a value
    void insert(int value) {
        int idx = hash(value);
        table[idx].push_back(value);
    }

    // Boolean search
    bool search(int value) const {
        int idx = hash(value);
        for (int v : table[idx]) {
            if (v == value) return true;
        }
        return false;
    }

    // Destructor
    ~HashTable() {
        for (auto& bucket : table) {
            bucket.clear();
        }
        table.clear();
    }
};

// Returns true if target is found in sorted vector arr
bool binary_search(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return true;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}

// Generate a vector of unique random integers: generate with rand(), if not unique, increment until unique (modulo 1,000,000)
vector<int> generate_unique_random_vector(int size) {
    vector<int> vec;
    vec.reserve(size);
    unordered_set<int> used;
    while ((int)vec.size() < size) {
        int num = rand() % 1000000;
        while (used.count(num)) {
            num = (num + 1) % 1000000;
        }
        used.insert(num);
        vec.push_back(num);
    }
    return vec;
}

// Measure average search time for HashTable using chrono
double measure_hash_table_search(const vector<int>& values) {
    HashTable ht(values);
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
    double elapsed_ms = 0;
    for (int v : values) {
        start = chrono::high_resolution_clock::now();
        ht.search(v);
        end = chrono::high_resolution_clock::now();
        elapsed_ms += chrono::duration<double, milli>(end - start).count();
    }
    return elapsed_ms / values.size();
}

// Measure average search time for binary search using chrono
double measure_binary_search(const vector<int>& values) {
    vector<int> sorted_values = values;
    sort(sorted_values.begin(), sorted_values.end());
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
    double elapsed_ms = 0;
    for (int v : values) {
        start = chrono::high_resolution_clock::now();
        binary_search(sorted_values, v);
        end = chrono::high_resolution_clock::now();
        elapsed_ms += chrono::duration<double, milli>(end - start).count();
    }
    return elapsed_ms / values.size();
}

int main() {
    vector<tuple<int, double, double>> timings;
    for (int input_size = 500; input_size <= 100000; input_size += 500) {
        vector<int> values = generate_unique_random_vector(input_size);
        double avg_ht = measure_hash_table_search(values);
        double avg_bs = measure_binary_search(values);
        timings.emplace_back(input_size, avg_ht, avg_bs);
        cout << "Input size: " << input_size << ", HashTable avg: " << avg_ht << " ms, BinarySearch avg: " << avg_bs << " ms\n";
    }

    FILE* f = fopen("bonus_task_timings.csv", "w");
    if (!f) {
        cerr << "Error: Could not open file for writing." << endl;
        return 1;
    }
    fprintf(f, "InputSize,HashTableAvgTime,BinarySearchAvgTime\n");
    for (const auto& t : timings) {
        int input_size;
        double avg_ht, avg_bs;
        tie(input_size, avg_ht, avg_bs) = t;
        fprintf(f, "%d,%lf,%lf\n", input_size, avg_ht, avg_bs);
    }
    fclose(f);
    cout << "Timing data written to bonus_task_timings.csv" << endl;
    return 0;
}
