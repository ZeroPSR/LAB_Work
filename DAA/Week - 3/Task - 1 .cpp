#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

// Finding the maximum subarray sum using Kadane's algorithm
// Time Complexity: O(n)
// Space Complexity: O(1)

tuple<int, int, int> maxSubarraySum(const vector<int>& nums) {
    int maxSum = 0;
    int currentSum = 0;
    int start = 0, end = -1, tmpStart = 0;

    for (int i = 0; i < nums.size(); ++i) {
        currentSum += nums[i];

        if (currentSum > maxSum) {
            maxSum = currentSum;
            start = tmpStart;
            end = i;
        }

        if (currentSum < 0) {
            currentSum = 0;
            tmpStart = i + 1;
        }
    }

    return make_tuple(start, end, maxSum);
}

void runTestCases(const vector<vector<int>>& testCases) {
    for (int i = 0; i < testCases.size(); ++i) {
        const auto& nums = testCases[i];
        auto [start, end, maxSum] = maxSubarraySum(nums);

        cout << "Test Case #" << i + 1 << ":\n";
        cout << "Array: ";
        for (int num : nums) cout << num << " ";
        cout << "\nMaximum Sum: " << maxSum << "\n";

        if (end >= start) {
            cout << "Subarray: ";
            for (int j = start; j <= end; ++j) cout << nums[j] << " ";
            cout << "\n";
        } else {
            cout << "Subarray: (empty)\n";
        }
        cout << "--------------------------\n";
    }
}

int main() {
    vector<vector<int>> testCases = {
        {-2, 1, -3, 4, -1, 2, 1, -5, 4},    
        {-1, -2, -3, -4},   
        {5, -1, 2, -1, 3, -2, 4},   
        {},  
        {0, 0, 0, 0},  
        {1, 2, 3, 4, 5},  
        {-5, -4, 10, -1, 2, -1, 3, -2, 1}, 
        {-3, 4, -1, -2, 1, 5, -3} 
    };

    runTestCases(testCases);
    return 0;
}
