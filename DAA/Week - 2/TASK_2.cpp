#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>

using namespace std;

// Fixed Pivot Quick Sort (last element as pivot)
int partitionFixed(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortFixed(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionFixed(arr, low, high);
        quickSortFixed(arr, low, pi - 1);
        quickSortFixed(arr, pi + 1, high);
    }
}

// Randomized Quick Sort (random pivot)
int partitionRandom(vector<int>& arr, int low, int high) {
    int pivotIdx = low + rand() % (high - low + 1);
    swap(arr[pivotIdx], arr[high]);
    return partitionFixed(arr, low, high);
}

void quickSortRandom(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partitionRandom(arr, low, high);
        quickSortRandom(arr, low, pi - 1);
        quickSortRandom(arr, pi + 1, high);
    }
}

void fillSorted(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) arr[i] = i + 1;
}

void fillReverseSorted(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) arr[i] = arr.size() - i;
}

void fillRandom(vector<int>& arr) {
    for (int i = 0; i < arr.size(); i++) arr[i] = rand()%1000000;
}

void measureAndPrint(const vector<int>& input, void(*sortFunc)(vector<int>&, int, int), const string& label, int n) {
    vector<int> arr = input;
    auto start = chrono::high_resolution_clock::now();
    sortFunc(arr, 0, n - 1);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();
    cout << label << ", n=" << n << ": " << ms << " ms" << endl;
}

int main() {
    srand((unsigned int)time(nullptr));

    #define NUM_SIZES 50
    #define MIN_SIZE 500
    #define SIZE_STEP 500
    int sizes[NUM_SIZES];
    double RQS_rand[NUM_SIZES], LQS_rand[NUM_SIZES];
    double RQS_sort[NUM_SIZES], LQS_sort[NUM_SIZES];
    double RQS_revs[NUM_SIZES], LQS_revs[NUM_SIZES];
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
    for (int i = 0; i < NUM_SIZES; i++) {
        int n = MIN_SIZE + i * SIZE_STEP;
        sizes[i] = n;
        vector<int> arr(n);
        for (int j = 0; j < n; j++) arr[j] = rand()%1000000;

        // --- Randomized array timings ---
        vector<int> arrCopy = arr;
    start = chrono::high_resolution_clock::now();
    quickSortRandom(arrCopy, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    RQS_rand[i] = chrono::duration<double, milli>(end - start).count();

    arrCopy = arr;
    start = chrono::high_resolution_clock::now();
    quickSortFixed(arrCopy, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    LQS_rand[i] = chrono::duration<double, milli>(end - start).count();

    // --- Sorted array timings ---
    sort(arr.begin(), arr.end());
    arrCopy = arr;
    start = chrono::high_resolution_clock::now();
    quickSortRandom(arrCopy, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    RQS_sort[i] = chrono::duration<double, milli>(end - start).count();

    arrCopy = arr;
    start = chrono::high_resolution_clock::now();
    quickSortFixed(arrCopy, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    LQS_sort[i] = chrono::duration<double, milli>(end - start).count();

    // --- Reverse sorted array timings ---
    reverse(arr.begin(), arr.end());
    arrCopy = arr;
    start = chrono::high_resolution_clock::now();
    quickSortRandom(arrCopy, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    RQS_revs[i] = chrono::duration<double, milli>(end - start).count();

    arrCopy = arr;
    start = chrono::high_resolution_clock::now();
    quickSortFixed(arrCopy, 0, n - 1);
    end = chrono::high_resolution_clock::now();
    LQS_revs[i] = chrono::duration<double, milli>(end - start).count();

        // Print progress indicator
        cout << "Completed: " << (i + 1) << "/" << NUM_SIZES << " (n=" << n << ")" << endl;
    }

    FILE* f = fopen("task_2_timings.csv", "w");
    if (!f) {
        cerr << "Error: Could not open file for writing." << endl;
        return 1;
    }
    fprintf(f, "InputSize,RQS_rand(ms),LQS_rand(ms),RQS_sort(ms),LQS_sort(ms),RQS_revs(ms),LQS_revs(ms)\n");
    for (int i = 0; i < NUM_SIZES; i++) {
        fprintf(f, "%d,%lf,%lf,%lf,%lf,%lf,%lf\n", sizes[i], RQS_rand[i], LQS_rand[i], RQS_sort[i], LQS_sort[i], RQS_revs[i], LQS_revs[i]);
    }
    fclose(f);
    cout << "Timing data written to task_2_timings.csv" << endl;
    return 0;
}
