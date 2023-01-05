#pragma once

namespace dist {

// copied from https://stackoverflow.com/a/12335206
template <typename> class packaged_task;

template <typename R, typename ...Args>
class packaged_task<R(Args...)> {
  std::function<R(Args...)> fn;
  promise<R> pr;             // the promise of the result
public:
  packaged_task(packaged_task&& other);
  packaged_task(std::function<R(Args...)> fn_);
  template <typename ...Ts>
  void operator()(Ts &&... ts);
  future<R> get_future();
  packaged_task(const packaged_task&) = delete;
  const packaged_task& operator=(const packaged_task&) = delete;
};

}
