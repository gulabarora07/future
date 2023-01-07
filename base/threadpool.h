#pragma once

namespace dist {

template <typename first_t, typename second_t>
struct compare_pair {
  std::less<first_t> less;
  bool operator() (std::pair<first_t, second_t>& a,
      std::pair<first_t, second_t>& b) {
    return less(a.first, b.first);
  }
};

template<class F>
struct shared_function {
  std::shared_ptr<F> f;
  shared_function() = delete;
  shared_function(F&& f_):f(std::make_shared<F>(std::move(f_))){}
  shared_function(shared_function const&)=default;
  shared_function(shared_function&&)=default;
  shared_function& operator=(shared_function const&)=default;
  shared_function& operator=(shared_function&&)=default;
  template<class...As>
  auto operator()(As&&...as) const {
    return (*f)(std::forward<As>(as)...);
  }
};
template<class F>
shared_function< std::decay_t<F> > make_shared_function( F&& f ) {
  return { std::forward<F>(f) };
}

template <typename priority_t>
class threadpool {
  using func_t = std::function<void()>;
 private:
  void worker();
  static size_t get_threads(size_t nthreads);
  void stop() {}
  std::atomic<bool> running{true};
 public:
  threadpool(size_t nthread_ = 0);
  ~threadpool();
  template <class F, class ...Args>
  std::future<std::invoke_result_t<F, Args...>> push_task(priority_t priority,
      F&& func, Args&&... args);
 private:
  ts_pqueue<std::pair<priority_t, func_t>,
    compare_pair<priority_t, func_t>> tasks;
  std::vector<std::thread> threads;
  size_t nthreads;
};

}
