/*
An e-commerce company needs to sort millions of daily transactions quickly for reporting.
Quick Sort is fast on average but degrades to O(n^2) in worst cases,
while Merge Sort guarantees O(nlogn) but with extra memory overhead.
AHybrid Sorting algorithm applies Quick Sort normally but switches to Merge Sort when recursion
depth exceeds log^2n, achieving both speed and reliability.

1. Implement Quick Sort, Merge Sort, and Hybrid Sort.
2. Run experiments on arrays of sizes 1000, 5000, 10000, 50000.
3. Compare execution times of all three algorithms.
4. Plot input size vs execution time using gnuplot.

Expected Result: 
 Quick Sort: fastest on random data, slow on nearly sorted data. 
 Merge Sort: consistent but slower due to overhead. 
 Hybrid Sort: combines Quick Sort’s speed with Merge Sort’s stability, performing best overall.
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <fstream>

using namespace std;
using namespace std::chrono;

int threshold = 16; // Threshold for switching to insertion sort

void insertionSort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void merge(vector<int>& arr, vector<int>& temp, int left, int mid, int right) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (i = left; i <= right; i++) arr[i] = temp[i];
}

void mergeSort(vector<int>& arr, vector<int>& temp, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, temp, left, mid);
        mergeSort(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high, int depthLimit) {
    if (low < high) {
        if (depthLimit == 0) {
            // Switch to Merge Sort if depth limit is reached
            vector<int> temp(arr.size());
            mergeSort(arr, temp, low, high);
        } else {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1, depthLimit - 1);
            quickSort(arr, pi + 1, high, depthLimit - 1);
        }
    } else if (high - low < threshold) {
        insertionSort(arr, low, high);
    }
}

void hybridSort(vector<int>& arr) {
    int depthLimit = log(arr.size()) * log(arr.size());
    quickSort(arr, 0, arr.size() - 1, depthLimit);
}

vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100000;
    }
    return arr;
}

int main() {
    srand(time(0));
    vector<int> sizes = {10000,50000, 100000, 500000, 1000000, 5000000};
    vector<double> quickSortTimes, mergeSortTimes, hybridSortTimes;

    for (int size : sizes) {
        vector<int> arr = generateRandomArray(size);
        vector<int> arrCopy;

        // Quick Sort
        arrCopy = arr;
        auto start = high_resolution_clock::now();
        quickSort(arrCopy, 0, arrCopy.size() - 1, log(arrCopy.size()) * log(arrCopy.size()));
        auto end = high_resolution_clock::now();
        auto quickSortDuration = duration_cast<milliseconds>(end - start).count();
        quickSortTimes.push_back(quickSortDuration);
        cout << "Quick Sort on size " << size << ": " << quickSortDuration << " ms" << endl;


        // Merge Sort
        arrCopy = arr;
        vector<int> temp(arrCopy.size());
        start = high_resolution_clock::now();
        mergeSort(arrCopy, temp, 0, arrCopy.size() - 1);
        end = high_resolution_clock::now();
        auto mergeSortDuration = duration_cast<milliseconds>(end - start).count();
        mergeSortTimes.push_back(mergeSortDuration);
        cout << "Merge Sort on size " << size << ": " << mergeSortDuration << " ms" << endl;

        // Hybrid Sort
        arrCopy = arr;
        start = high_resolution_clock::now();
        hybridSort(arrCopy);
        end = high_resolution_clock::now();
        auto hybridSortDuration = duration_cast<milliseconds>(end - start).count();
        hybridSortTimes.push_back(hybridSortDuration);
        cout << "Hybrid Sort on size " << size << ": " << hybridSortDuration << " ms" << endl;

        cout << "----------------------------------------" << endl;
    }
    ofstream file("Task_2_timings.csv");
    file << "Size,QuickSort,MergeSort,HybridSort\n";
    for (size_t i = 0; i < sizes.size(); i++) {
        file << sizes[i] << "," << quickSortTimes[i] << "," << mergeSortTimes[i] << "," << hybridSortTimes[i] << "\n";
    }
    file.close();
    return 0;
}
