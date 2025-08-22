#include <iostream>
#include <vector>
using namespace std;

//Time Complexity: O(n^2)
//Space Complexity: O(1)

pair<int, int> subarrayStatsWithSumK(const vector<int>& nums, int k) {
    int n = nums.size();
    int maxLen = 0;
    int totalCount = 0;

    for (int start = 0; start < n; ++start) {
        int sum = 0;
        for (int end = start; end < n; ++end) {
            sum += nums[end];
            if (sum == k) {
                totalCount++;
                maxLen = max(maxLen, end - start + 1);
            }
        }
    }

    return {maxLen, totalCount};
}

int main() {
    vector<pair<vector<int>, int>> testCases = {
        {{1, 2, 3}, 3},
        {{1, 1, 1}, 2},
        {{-1, -1, 1}, 0},
        {{3, 4, 7, 2, -3, 1, 4, 2}, 7},
        {{1, -1, 5, -2, 3}, 3},
        {{}, 0},
        {{0, 0, 0, 0}, 0}
    };

    for (int i = 0; i < testCases.size(); ++i) {
        const auto& [nums, k] = testCases[i];
        auto [maxLen, totalCount] = subarrayStatsWithSumK(nums, k);

        cout << "Test Case #" << i + 1 << ":\n";
        cout << "Array: ";
        for (int num : nums) cout << num << " ";
        cout << "\nTarget Sum : " << k << "\n";
        cout << "Longest Subarray Length: " << maxLen << "\n";
        cout << "Total Subarrays with Sum = k: " << totalCount << "\n";
        cout << "--------------------------\n";
    }

    return 0;
}