#include <iostream>
using namespace std;
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdio>
#include <cstdlib>

void selectionSort(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }
        if (min_idx != i) {
            swap(arr[i], arr[min_idx]);
        }
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest])
        largest = l;
    if (r < n && arr[r] > arr[largest])
        largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void bubbleSort(vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


#define NUM_SIZES 200
#define MIN_SIZE 500
#define SIZE_STEP 500

void copyArray(const vector<int>& src, vector<int>& dest) {
    dest = src;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    double selectionTimes[NUM_SIZES], heapTimes[NUM_SIZES], bubbleTimes[NUM_SIZES];
    int sizes[NUM_SIZES];
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;


    for (int i = 0; i < NUM_SIZES; i++) {
        int n = MIN_SIZE + i * SIZE_STEP;
        sizes[i] = n;
        vector<int> arr(n);
        vector<int> arrCopy(n);
        for (int j = 0; j < n; j++) arr[j] = rand() % 2000;

        copyArray(arr, arrCopy);
        start = chrono::high_resolution_clock::now();
        selectionSort(arrCopy);
        end = chrono::high_resolution_clock::now();
        selectionTimes[i] = chrono::duration<double, milli>(end - start).count();

        copyArray(arr, arrCopy);
        start = chrono::high_resolution_clock::now();
        heapSort(arrCopy);
        end = chrono::high_resolution_clock::now();
        heapTimes[i] = chrono::duration<double, milli>(end - start).count();

        copyArray(arr, arrCopy);
        start = chrono::high_resolution_clock::now();
        bubbleSort(arrCopy);
        end = chrono::high_resolution_clock::now();
        bubbleTimes[i] = chrono::duration<double, milli>(end - start).count();

        // Print progress indicator
        cout << "Completed: " << (i + 1) << "/" << NUM_SIZES << " (n=" << n << ")" << endl;
    }

    FILE* f = fopen("task_1_timings.csv", "w");
    if (!f) {
        cerr << "Error: Could not open file for writing." << endl;
        return 1;
    }
    fprintf(f, "InputSize,SelectionSort(ms),HeapSort(ms),BubbleSort(ms)\n");
    for (int i = 0; i < NUM_SIZES; i++) {
        fprintf(f, "%d,%lf,%lf,%lf\n", sizes[i], selectionTimes[i], heapTimes[i], bubbleTimes[i]);
    }
    fclose(f);

    cout << "Timing data written to task_1_timings.csv" << endl;
    return 0;
}