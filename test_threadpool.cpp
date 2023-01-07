#include "base/include_stl.h"
#include <vector>
#include <cmath>
#include <thread>
#include <numeric>
#include <iostream>

int main() {
  std::vector<std::future<double>> powers_2;
  {
    dist::threadpool<int> tp(32);
    for (int i = 0; i < 50; i++) {
      powers_2.push_back(std::move(tp.push_task(i, [](double a, double b) {
            return std::pow(a, b);
        }, 2, i)));
    }
  }
  for(int i = 0; i < powers_2.size(); i++) {
    std::cout << "2**" << i << "\t= " << powers_2[i].get() << std::endl;
  }
}
