#include "lib/Allocator.cpp"
#include <iostream>
#include <cstdlib>

int main() {
  const size_t pools_amount = 2;
  size_t pools_size[pools_amount] = {40, 40};
  CAllocator<int, pools_amount> a(pools_size);
  int *b[20];
  for(int i = 0; i < 20; i++) {
    b[i] = a.allocate(1);
//    *b[i] = i;
    std::cout << i << ' ' << b[i] << std::endl;
  }
}