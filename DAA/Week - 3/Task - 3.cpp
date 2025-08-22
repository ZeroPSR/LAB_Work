#include <iostream>
#include <vector>
using namespace std;

// Time Complexity: O(n^2)
// Space Complexity: O(1)

void findZeroSumSubarray(const vector<int>& arr) {
    int n = arr.size();
    for (int start = 0; start < n; ++start) {
        int sum = 0;
        for (int end = start; end < n; ++end) {
            sum += arr[end];
            if (sum == 0) {
                cout << "Subarray with sum 0 found: [ ";
                for (int i = start; i <= end; ++i)
                    cout << arr[i] << " ";
                cout << "]" << endl;
                return;
            }
        }
    }
    cout << "No subarray with sum 0 found." << endl;
}

int main() {
    vector<vector<int>> testCases = {
        {6, -1, -3, 4, -2, 2, 4, 6, -12, -7}, 
        {0, 0, 5, 5, 0, 0}, 
        {1, 2, -3, 4, 5},
        {1, 2, 3, 4},  
        {0},    
        {-3, 1, 2},                           
        {10, -10},    
        {1, -1, 2, -2, 3, -3} ,
        {-1, 1, -1, 1},  
        {5, 5, -10, 5},  
        {2, -2, 3, -3, 4}
    };

    for (int i = 0; i < testCases.size(); ++i) {
        cout << "\nTest Case " << i + 1 << ": [ ";
        for (int num : testCases[i]) cout << num << " ";
        cout << "]" << endl;
        findZeroSumSubarray(testCases[i]);
    }

    return 0;
}