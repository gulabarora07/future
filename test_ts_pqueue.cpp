#include "base/include.h"
#include <vector>
#include <cmath>
#include <thread>
#include <numeric>
#include <iostream>

void push(dist::ts_pqueue<int>& q, int item) {
  q.push(item);
}

void pop_and_print(dist::ts_pqueue<int>& q, std::mutex& print_mut) {
  std::lock_guard<std::mutex> lk(print_mut);
  int item = q.top_and_pop();
  std::cout << item << std::endl;
}

int main() {
  std::vector<int> test = { 3, 4, 5, 6, 8, 1, 2, 10, 9, 7 };
  std::vector<std::thread> test_threads;
  dist::ts_pqueue<int> q;
  std::mutex print_mut;
  for (int i = 0; i < test.size(); i++) {
    test_threads.emplace_back(push, std::ref(q), test[i]);
  }
  for (int i = 0; i < test.size(); i++) {
    test_threads[i].join();
  }
  test_threads.clear();
  for (int i = 0; i < test.size(); i++) {
    test_threads.emplace_back(pop_and_print, std::ref(q), std::ref(print_mut));
  }
  for (int i = 0; i < test.size(); i++) {
    test_threads[i].join();
  }
}
