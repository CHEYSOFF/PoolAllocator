#include "lib/Allocator.cpp"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <chrono>

constexpr size_t pools_amount = 5;
constexpr size_t pools_size[5] = {1024, 1024, 1024, 1024, 1024};

int main() {

//  CAllocator<int, pools_amount, pools_size> a;
//  int *e = a.allocate(1);
//  *e = 5;
//  std::cout << *e;
  auto start1 = std::chrono::high_resolution_clock::now();
  std::vector<int, CAllocator<int, pools_amount, pools_size>> vec11;
  for(int i = 0; i < 100; i++) {
    vec11.push_back(i);
  }
  std::vector<int, CAllocator<int, pools_amount, pools_size>> vec12;
  for(int i = 0; i < 100; i++) {
    vec12.push_back(i);
  }
  std::vector<int, CAllocator<int, pools_amount, pools_size>> vec13;
  for(int i = 0; i < 100; i++) {
    vec13.push_back(i);
  }
  std::vector<int, CAllocator<int, pools_amount, pools_size>> vec14;
  for(int i = 0; i < 100; i++) {
    vec14.push_back(i);
  }
  std::vector<int, CAllocator<int, pools_amount, pools_size>> vec15;
  for(int i = 0; i < 100; i++) {
    vec15.push_back(i);
  }
  auto end1 = std::chrono::high_resolution_clock::now();
  auto start2 = std::chrono::high_resolution_clock::now();
  std::vector<int> vec21;
  for(int i = 0; i < 100; i++) {
    vec21.push_back(i);
  }
  std::vector<int> vec22;
  for(int i = 0; i < 100; i++) {
    vec22.push_back(i);
  }
  std::vector<int> vec23;
  for(int i = 0; i < 100; i++) {
    vec23.push_back(i);
  }
  std::vector<int> vec24;
  for(int i = 0; i < 100; i++) {
    vec24.push_back(i);
  }
  std::vector<int> vec25;
  for(int i = 0; i < 100; i++) {
    vec25.push_back(i);
  }
  auto end2 = std::chrono::high_resolution_clock::now();

  auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
  auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
  std::cout << duration1.count() << " " << duration2.count();
}
// 106 279
// 104 182
// 105 257
// 106 209
// 104 161
//
// 105 217