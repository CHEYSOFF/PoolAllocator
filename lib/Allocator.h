#pragma once
#include <map>
#include <cstdlib>
template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
class CAllocator {
 public:
  typedef T value_type;
  typedef T *pointer;
  typedef size_t size_type;
  template<typename U>
  struct rebind {
    using other = CAllocator<U, pool_amount, pools_size_>;
  };
  CAllocator();
  CAllocator(CAllocator<T, pool_amount, pools_size_> &other);
  template<typename B>
  CAllocator(CAllocator<B, pool_amount, pools_size_> &other);
  CAllocator(CAllocator<T, pool_amount, pools_size_> &&other);
  template<typename B>
  CAllocator(CAllocator<B, pool_amount, pools_size_> &&other);
  ~CAllocator();
  T *allocate(size_t n);
  void deallocate(T *p, size_t n);
  size_t max_size();
  bool operator==(CAllocator<T, pool_amount, pools_size_> &rhs);
  CAllocator &operator=(const CAllocator<T, pool_amount, pools_size_> &other);
 private:
  T **pools_;
  size_t *pools_first_;
  bool **free_chunks_;
  size_t *usage_amount;
  void OutputFreeChunks();
  template<typename B, size_t pool_amount_2, const size_t (&pools_size_2_)[pool_amount_2]>
  friend
  class CAllocator;
};


template<typename T, size_t pool_amount_1, size_t pool_amount_2, T (&pools_size_)[pool_amount_1]>
bool operator==(CAllocator<T, pool_amount_1, pools_size_> &lhs, CAllocator<T, pool_amount_2, pools_size_> &rhs);

template<typename T, size_t pool_amount_1, size_t pool_amount_2, T (&pools_size_)[pool_amount_1]>
bool operator!=(CAllocator<T, pool_amount_1, pools_size_> &lhs, CAllocator<T, pool_amount_2, pools_size_> &rhs);




