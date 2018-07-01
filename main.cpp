#include <iostream>
#include <vector>
#include <list>
#include "./STL_allocator.h"

int main() {
    std::vector <int,Allocator<int>> a;
    for(int i = 0 ; i < 500 ; i++){
        a.push_back(i);
    }
    std::list <int,Allocator<int>> b;
    for(int i = 0 ; i < 500 ; i++){
        b.push_back(i);
    }
    //exit(EXIT_SUCCESS);
}