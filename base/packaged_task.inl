#pragma once

namespace dist {

template <typename R, typename ...Args>
packaged_task<R(Args...)>::packaged_task(packaged_task&& other) :
  fn(std::move(other.fn)), pr(std::move(other.pr)) {}

template <typename R, typename ...Args>
packaged_task<R(Args...)>::packaged_task(std::function<R(Args...)> fn_) :
  fn(fn_) {}

template <typename R, typename ...Args>
template <typename ...Ts>
void packaged_task<R(Args...)>::operator()(Ts &&... ts) {
    pr.set_value(fn(std::forward<Ts>(ts)...));  // fulfill the promise
}

template <typename R, typename ...Args>
future<R> packaged_task<R(Args...)>::get_future() {
  return pr.get_future();
}

}
