#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace std;
using namespace chrono;

// Generate random input
vector<int> generateRandomInputs(int size) {
    srand(static_cast<unsigned>(time(nullptr)));
    vector<int> inputs;
    inputs.reserve(size);
    for (int i = 0; i < size; ++i) {
        inputs.push_back(1 + rand() % 1'000'000);
    }
    return inputs;
}

// Linear Selection (average-case O(n))
int linearSelection(vector<int>& arr, int left, int right, int k) {
    if (left > right || k <= 0 || k > right - left + 1) return -1;

    int range = right - left + 1;
    int pivotIndex = left + rand() % range;
    int pivot = arr[pivotIndex];
    int l = left, r = right;

    while (l <= r) {
        while (arr[l] < pivot) ++l;
        while (arr[r] > pivot) --r;
        if (l <= r) swap(arr[l++], arr[r--]);
    }

    int count = r - left + 1;
    if (k <= count) return linearSelection(arr, left, r, k);
    else return linearSelection(arr, l, right, k - count);
}

// Sorting + Indexing (O(n log n))
int sortAndIndex(vector<int> arr, int k) {
    sort(arr.begin(), arr.end());
    return arr[k - 1];
}

int main() {
    vector<int> inputSizes = {
        50000, 100000, 150000, 200000, 250000,
        300000, 350000, 400000, 450000, 500000
    };

    ofstream csv("timings.csv");
    csv << "InputSize,LinearSelection,Sort+Index\n";

    for (int size : inputSizes) {
        csv << size << ",";
        int k = size / 2; // Median

        vector<int> data = generateRandomInputs(size);

        // Linear Selection
        vector<int> data1 = data;
        auto start1 = high_resolution_clock::now();
        int result1 = linearSelection(data1, 0, size - 1, k);
        auto end1 = high_resolution_clock::now();
        auto time1 = duration_cast<milliseconds>(end1 - start1).count();
        csv << time1 << "," ;

        // Sort + Index
        vector<int> data2 = data;
        auto start2 = high_resolution_clock::now();
        int result2 = sortAndIndex(data2, k);
        auto end2 = high_resolution_clock::now();
        auto time2 = duration_cast<milliseconds>(end2 - start2).count();
        csv << time2 << "\n";

        cout << "Size " << size
             << " → linearSelection: " << time1 << "ms"
             << ", Sort+Index: " << time2 << "ms\n";
    }

    csv.close();

    cout << "\nComparison complete. Results saved to timings.csv\n";

    return 0;
}