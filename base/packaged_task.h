#pragma once

namespace dist {

// copied from https://stackoverflow.com/a/12335206
template <typename> class packaged_task;

template <typename R, typename ...Args>
class packaged_task<R(Args...)> {
  std::function<R(Args...)> fn;
  promise<R> pr;             // the promise of the result
public:
  packaged_task(packaged_task&& other) = default;
  template <typename ...Ts>
  explicit packaged_task(Ts &&... ts) : fn(std::forward<Ts>(ts)...) { }
  template <typename ...Ts>
  void operator()(Ts &&... ts)
  {
      pr.set_value(fn(std::forward<Ts>(ts)...));  // fulfill the promise
  }
  future<R> get_future() { return pr.get_future(); }
  packaged_task(const packaged_task&)=delete;
  const packaged_task& operator=(const packaged_task&) = delete;
};

}
