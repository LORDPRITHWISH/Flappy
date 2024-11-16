#include <iostream>
#include<vector>
// #include <cstdlib>
// #include <ctime>

int main() {
    // std::vector<std::vector<int>> v = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    // v.push_back({10, 11, 12});

    // for(std::vector<int> i : v){
    //     i[0]*=10;
    // }

    // for (std::vector<int> i : v) {
    //     for (int j : i) {
    //         std::cout << j << ' ';
    //     }
    //     std::cout << '\n';
    // }

    std::vector<int> a = {1,2,3,4,5,6,99};
    std::cout<<a[a.size()-1];


    return 0;
}