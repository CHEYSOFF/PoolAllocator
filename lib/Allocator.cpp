#include "Allocator.h"
#include <cstdlib>
#include <iostream>

template<typename T, size_t pool_amount>
CAllocator<T, pool_amount>::CAllocator(size_t *pools_size) : pools_size_(pools_size){
  if(pool_amount <= 0) {
    throw std::bad_alloc{};
  }

  for(int i = 0; i < pool_amount; i++) {
    pools_[i] = (T*)malloc(pools_size[i] * sizeof(T));
  }
  for(int i = 0; i < pool_amount; i++) {
    pools_first_[i] = 0;
  }
}

template<typename T, size_t pool_amount>
void CAllocator<T, pool_amount>::deallocate(T *p, size_t n) {
  free(p);
}

template<typename T, size_t pool_amount>
CAllocator<T, pool_amount>::~CAllocator() {
  for(int i = 0; i < pool_amount; i++) {
    deallocate(pools_[i], pools_size_[i]);
  }
}

template<typename T, size_t pool_amount>
T *CAllocator<T, pool_amount>::allocate(size_t n) {
  size_t len = n * sizeof(T);
  int pool_num = 0;
  while (pools_size_[pool_num] - pools_first_[pool_num] < len) {
    pool_num++;
    if(pool_num >= pool_amount) {
      throw std::bad_alloc{};
    }
  }

  size_t first_index = pools_first_[pool_num];

  T* ans = pools_[pool_num] + first_index;
  pools_first_[pool_num] += len;

  return ans;

}