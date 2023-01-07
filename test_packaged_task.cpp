#include "base/include.h"
#include <vector>
#include <cmath>
#include <thread>
#include <numeric>
#include <iostream>

// unique function to avoid disambiguating the std::pow overload set
int f(int x, int y) { return std::pow(x,y); }

void task_lambda()
{
  dist::packaged_task<int(int,int)> task([](int a, int b) {
      return std::pow(a, b);
  });
  dist::future<int> result = task.get_future();

  task(2, 9);

  std::cout << "task_lambda:\t" << result.get() << '\n';
}

void task_bind()
{
  dist::packaged_task<int()> task(std::bind(f, 2, 11));
  dist::future<int> result = task.get_future();

  task();

  std::cout << "task_bind:\t" << result.get() << '\n';
}

void task_thread()
{
  dist::packaged_task<int(int,int)> task(f);
  dist::future<int> result = task.get_future();
  std::thread task_td([&](int a, int b) {task(a, b);}, 2, 10);
  task_td.join();
  std::cout << "task_thread:\t" << result.get() << '\n';
}

int main()
{
  task_lambda();
  task_bind();
  task_thread();
}
