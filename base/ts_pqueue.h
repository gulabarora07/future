#pragma once

#include <queue>

namespace dist {

template <typename T, typename Compare = std::less<T>>
class ts_pqueue : protected base_mutable_struct {
 protected:
  std::priority_queue<T, std::vector<T>, Compare> pq;
 public:
  ts_pqueue() = default;
  ~ts_pqueue() = default;
  void push(T item);
  bool empty();
  size_t size();
  void pop();
  T top();
  T top_and_pop();
  ts_pqueue(ts_pqueue&&) = delete;
  ts_pqueue(const ts_pqueue&) = delete;
  const ts_pqueue& operator=(const ts_pqueue&) = delete;
};

}
