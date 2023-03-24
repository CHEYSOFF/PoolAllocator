#pragma once
#include <map>
#include <cstdlib>
template<typename T, size_t pool_amount>
class CAllocator {
 public:
  typedef T value_type;
  typedef T *pointer;
  CAllocator(size_t *pools_size);
  ~CAllocator();
  T* allocate(size_t n);
  void deallocate(T* p, size_t n);
 private:
  T* pools_[pool_amount];
  size_t *pools_size_;
  size_t pools_first_[pool_amount];
};






