#include "Allocator.h"
#include <cstdlib>
#include <iostream>
#include <set>

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
CAllocator<T, pool_amount, pools_size_>::CAllocator() {
  usage_amount = (size_t *) malloc((sizeof(size_t)));
  *usage_amount = 1;
  if (pool_amount <= 0) {
    throw std::bad_alloc{};
  }
  pools_ = (T **) malloc(pool_amount * sizeof(T *));
  for (int i = 0; i < pool_amount; i++) {
    pools_[i] = (T *) malloc(pools_size_[i] * sizeof(T));
  }
  pools_first_ = (size_t *) malloc(pool_amount * sizeof(size_t *));
  for (int i = 0; i < pool_amount; i++) {
    pools_first_[i] = 0;
  }
  free_chunks_ = (bool **) malloc(pool_amount * sizeof(bool *));
  for (int i = 0; i < pool_amount; i++) {
    free_chunks_[i] = (bool *) malloc(pools_size_[i] * sizeof(bool));
    for (int j = 0; j < pools_size_[i]; j++) {
      free_chunks_[i][j] = true;
    }
  }
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
CAllocator<T, pool_amount, pools_size_>::CAllocator(CAllocator<T, pool_amount, pools_size_> &other) : pools_(other.pools_),
                                                                            pools_first_(other.pools_first_),
                                                                            free_chunks_(other.free_chunks_),
                                                                            usage_amount(other.usage_amount) {
  (*usage_amount)++;
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
template<typename B>
CAllocator<T, pool_amount, pools_size_>::CAllocator(CAllocator<B, pool_amount, pools_size_> &other)  : pools_((T **) other.pools_),
                                                                             pools_first_(other.pools_first_),
                                                                             free_chunks_(other.free_chunks_),
                                                                             usage_amount(other.usage_amount) {
  (*usage_amount)++;
  for (int i = 0; i < pool_amount; i++) {
    pools_[i] = (T *) pools_[i];
  }
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
CAllocator<T, pool_amount, pools_size_>::CAllocator(CAllocator<T, pool_amount, pools_size_> &&other)  : pools_(other.pools_),
                                                                              pools_first_(other.pools_first_),
                                                                              free_chunks_(other.free_chunks_),
                                                                              usage_amount(other.usage_amount) {
  (*usage_amount)++;
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
template<typename B>
CAllocator<T, pool_amount, pools_size_>::CAllocator(CAllocator<B, pool_amount, pools_size_> &&other): pools_((T **) other.pools_),
                                                                            pools_first_(other.pools_first_),
                                                                            free_chunks_(other.free_chunks_),
                                                                            usage_amount(other.usage_amount) {
  (*usage_amount)++;
  for (int i = 0; i < pool_amount; i++) {
    pools_[i] = (T *) pools_[i];
  }
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
CAllocator<T, pool_amount, pools_size_>::~CAllocator() {
  (*usage_amount)--;
  if (*usage_amount == 0) {
    for (int i = 0; i < pool_amount; i++) {
      free(pools_[i]);
      free(free_chunks_[i]);
    }
    free(pools_);
    free(free_chunks_);
    free(pools_first_);
    free(usage_amount);
  }
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
void CAllocator<T, pool_amount, pools_size_>::deallocate(T *p, size_t n) {
  for (int i = 0; i < pool_amount; i++) {
    if (p >= pools_[i] && p + n * sizeof(T) < pools_[i] + pools_size_[i] * sizeof(T)) {
      size_t first_index = p - pools_[i];
      size_t last_index = first_index + n * sizeof(T);
      for (size_t j = first_index; j < last_index; j++) {
        free_chunks_[i][j] = true;
      }
      for (size_t j = last_index; j < pools_first_[i]; j++) {
        if (!free_chunks_[i][j]) {
          return;
        }
      }
      pools_first_[i] = first_index;
      return;
    }
  }
  std::cerr << "Wrong pointer\n";
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
T *CAllocator<T, pool_amount, pools_size_>::allocate(size_t n) {
  size_t len = n * sizeof(T);
  size_t pool_num = 0;
  while (pools_size_[pool_num] - pools_first_[pool_num] < len) {
    pool_num++;
    if (pool_num >= pool_amount) {
      throw std::bad_alloc{};
    }
  }

  size_t first_index = pools_first_[pool_num];

  T *ans = pools_[pool_num] + first_index;

  for (size_t j = first_index; j < first_index + len; j++) {
    free_chunks_[pool_num][j] = false;
  }
  pools_first_[pool_num] += len;
  return ans;

}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
size_t CAllocator<T, pool_amount, pools_size_>::max_size() {
  size_t max_size_ = 0;
  for (int i = 0; i < pool_amount; i++) {
    max_size_ = std::max(max_size_, pools_size_[i] - pools_first_[i]);
  }
  return max_size_;
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
void CAllocator<T, pool_amount, pools_size_>::OutputFreeChunks() {
  for (int i = 0; i < pool_amount; i++) {
    std::cout << pools_first_[i] << ": ";
    for (int j = 0; j < pools_size_[i]; j++) {
      std::cout << (int) free_chunks_[i][j];
    }
    std::cout << '\n';
  }
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
bool CAllocator<T, pool_amount, pools_size_>::operator==(CAllocator<T, pool_amount, pools_size_> &rhs) {
  std::set<std::pair<T *, int>> lhs_pools;
  for (int i = 0; i < pool_amount; i++) {
    lhs_pools.insert({pools_[i], pools_size_[i]});
  }
  for (int i = 0; i < pool_amount; i++) {
    if (lhs_pools.find({rhs.pools_[i], rhs.pools_size_[i]}) == lhs_pools.end()) {
      return false;
    }
  }
  return true;
}

template<typename T, size_t pool_amount_1, size_t pool_amount_2, T (&pools_size_)[pool_amount_1]>
bool operator==(CAllocator<T, pool_amount_1, pools_size_> &lhs, CAllocator<T, pool_amount_2, pools_size_> &rhs) {
  if (pool_amount_1 != pool_amount_2) {
    return false;
  }
  return lhs.operator==(rhs);
}

template<typename T, size_t pool_amount_1, size_t pool_amount_2, T (&pools_size_)[pool_amount_1]>
bool operator!=(CAllocator<T, pool_amount_1, pools_size_> &lhs, CAllocator<T, pool_amount_2, pools_size_> &rhs) {
  return !(lhs == rhs);
}

template<typename T, size_t pool_amount, const size_t (&pools_size_)[pool_amount]>
CAllocator<T, pool_amount, pools_size_> &CAllocator<T, pool_amount, pools_size_>::operator=(const CAllocator<T, pool_amount, pools_size_> &other) {
  if (&other == this) {
    return *this;
  }
  (*this)(other);
  return *this;
}
