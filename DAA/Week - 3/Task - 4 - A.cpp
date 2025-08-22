#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Time Complexity: O(n) on average, O(n^2) in worst case
// Space Complexity: O(1)

int getMedian(vector<int>& arr, int l, int r) {
    sort(arr.begin() + l, arr.begin() + r + 1);
    return arr[l + (r - l) / 2];
}

// Partition around pivot
int partition(vector<int>& arr, int l, int r, int pivot) {
    int pivotIndex = find(arr.begin() + l, arr.begin() + r + 1, pivot) - arr.begin();
    swap(arr[pivotIndex], arr[r]);
    int storeIndex = l;
    for (int i = l; i < r; ++i) {
        if (arr[i] < pivot) {
            swap(arr[i], arr[storeIndex++]);
        }
    }
    swap(arr[storeIndex], arr[r]);
    return storeIndex;
}

int kthSmallestLinear(vector<int> arr, int l, int r, int k) {
    if (k > 0 && k <= r - l + 1) {
        int n = r - l + 1;
        vector<int> medians;

        for (int i = 0; i < n / 5; ++i)
            medians.push_back(getMedian(arr, l + i * 5, l + i * 5 + 4));

        if (n % 5)
            medians.push_back(getMedian(arr, l + (n / 5) * 5, r));

        int medOfMed = (medians.size() == 1) ? medians[0] :
                       kthSmallestLinear(medians, 0, medians.size() - 1, medians.size() / 2);

        int pos = partition(arr, l, r, medOfMed);

        if (pos - l == k - 1)
            return arr[pos];
        if (pos - l > k - 1)
            return kthSmallestLinear(arr, l, pos - 1, k);
        return kthSmallestLinear(arr, pos + 1, r, k - pos + l - 1);
    }
    return INT_MAX;
}

int find90thPercentile(vector<int> arr) {
    int n = arr.size();
    return kthSmallestLinear(arr, 0, n - 1, n * 0.9);
}

int main() {
    vector<pair<vector<int>, int>> data = {
        {{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5},
        {{10, 20, 30, 40, 50, 60, 70, 80, 90, 100}, 7},
        {{5, 1, 9, 3, 14, 7, 8, 2, 6, 4}, 4},
        {{100, 200, 300, 400, 500, 600, 700, 800, 900, 1000}, 3},
        {{23, 46, 69, 92, 115, 138, 161, 184, 207}, 6},
        {{29, 58, 87, 116, 145}, 2},
        {{31}, 1},
        {{15, 22, 13, 27, 18, 30, 25}, 4}
};

for (const auto& pair : data) {
    const vector<int>& arr = pair.first;
    int k = pair.second;

    cout << "Array: ";
    for (int num : arr) cout << num << " ";
    cout << "\nK = " << k;
    cout << "\n→ 90th Percentile: " << find90thPercentile(arr);
    cout << "\n→ " << k << "th smallest element: " << kthSmallestLinear(vector<int>(arr), 0, arr.size() - 1, k);
    cout << "\n--------------------------\n";
}

    return 0;
}