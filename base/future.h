#pragma once

namespace dist {

// move constructible
template<typename T>
class future {
  friend promise<T>;
private:
  shared_state<T> *state;
  // private constructor, only promise can generate it, as it is friend
  future(shared_state<T>* state_);
  bool locked_valid();

public:
  future();
  ~future();
  // move constructor
  future(future&& other);
  T get();
  void wait();
  template<typename duration_t>
  void wait_for(std::chrono::duration<duration_t> time);
  void wait_until(std::chrono::time_point<std::chrono::system_clock> tp);
  bool valid();
  future(const future&) = delete;
  const future& operator=(const future&) = delete;
};

}
