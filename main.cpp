#include <iostream>
#include <vector>
#include <list>
#include "./STL_allocator.h"

int main() {
    std::vector <int,Allocator<int>> a(10000);
    for(int i = 0 ; i < 5000 ; i++){
        a.push_back(i);
    }
    std::list <int,Allocator<char>> b(10000);
    for(int i = 0 ; i < 5000 ; i++){
        b.push_back('a');
    }
    //exit(EXIT_SUCCESS);
}