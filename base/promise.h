#pragma once

namespace dist {

template<typename T>
class promise {
private:
  shared_state<T>* state;

public:
  promise();
  promise(promise&&);
  ~promise();
  future<T> get_future();
  void set_value(T value);
  void set_exception(std::exception& e);
  promise(const promise&)=delete;
  const promise& operator=(const promise&) = delete;
};

}
