#include <vector>
#include <algorithm>
#include <limits>

int parkingRoof(const std::vector<int>& cars, int k){
    std::vector<int> sortedCars = cars;
    std::sort(sortedCars.begin(), sortedCars.end());

    int ans = std::numeric_limits<int>::max();
    for (int i = 0; i <= sortedCars.size() - k; ++i){
        int temp = sortedCars[i + k - 1] - sortedCars[i] + 1;
        ans = std::min(ans, temp);
    }
    return ans;
}
