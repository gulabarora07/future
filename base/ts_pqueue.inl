#pragma once

namespace dist {

template <typename T, typename Compare>
void ts_pqueue<T, Compare>::push(T item) {
  std::unique_lock<std::mutex> lock(mut);
  pq.push(item);
  cv.notify_one();
}

template <typename T, typename Compare>
bool ts_pqueue<T, Compare>::empty() {
  std::lock_guard<std::mutex> lock(mut);
  return pq.empty();
}

template <typename T, typename Compare>
void ts_pqueue<T, Compare>::pop() {
  std::lock_guard<std::mutex> lock(mut);
  pq.pop();
}

template <typename T, typename Compare>
T ts_pqueue<T, Compare>::top() {
  std::unique_lock<std::mutex> lock(mut);
  if (pq.empty())
    cv.wait(lock, [&](){ return !pq.empty(); });
  return std::move(pq.top());
}

template <typename T, typename Compare>
T ts_pqueue<T, Compare>::top_and_pop() {
  std::unique_lock<std::mutex> lock(mut);
  if (pq.empty())
    cv.wait(lock, [&](){ return !pq.empty(); });
  T item = pq.top();
  pq.pop();
  return std::move(item);
}

}
