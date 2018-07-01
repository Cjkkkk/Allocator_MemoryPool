// testallocator.cpp : 定义控制台应用程序的入口点。
//

#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <limits>

//#include "myallocator.h"
//#include "y_alloc.h"
template<class T>
using MyAllocator = std::allocator<T>;
//using MyAllocator = y_alloc<T>;
//using MyAllocator = y_mp_alloc<T>;
using Point2D = std::pair<int, int>;

const int TestSize = 10000;
const int smallSize = 1000;
const int largeSize = TestSize;
const int PickSize = 1000;

int main()
{
  auto start = std::chrono::high_resolution_clock::now();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> smalldis(1, smallSize);
  std::uniform_int_distribution<> largedis(1, largeSize);

  // vector creation
  using IntVec = std::vector<int, MyAllocator<int>>;
  std::vector<IntVec, MyAllocator<IntVec>> vecints(TestSize);
  int smallVecs = TestSize * 0.85;
  for (int i = 0; i < smallVecs; i++)
    vecints[i].resize(smalldis(gen));

  //test large size memory allocation
  for (int i = smallVecs; i < TestSize; i++)
	  vecints[i].resize(largedis(gen));
  
  using PointVec = std::vector<Point2D, MyAllocator<Point2D>>;
  std::vector<PointVec, MyAllocator<PointVec>> vecpts(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecpts[i].resize(smalldis(gen));

  // vector resize
  for (int i = 0; i < PickSize; i++)
  {
    int idx = smalldis(gen) - 1;
    int size = smalldis(gen);
    vecints[idx].resize(size);
    vecpts[idx].resize(size);
  }

  // vector element assignment
  {
    int val = 10;
    int idx1 = largedis(gen) - 1;
    int idx2 = vecints[idx1].size() / 2;
    vecints[idx1][idx2] = val;
    if (vecints[idx1][idx2] == val)
      std::cout << "correct assignment in vecints: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
  }
  {
    Point2D val(11, 15);
    int idx1 = largedis(gen) - 1;
    int idx2 = vecpts[idx1].size() / 2;
    vecpts[idx1][idx2] = val;
    if (vecpts[idx1][idx2] == val)
      std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " seconds." << std::endl;

  return 0;
}

