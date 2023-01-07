#pragma once

namespace dist {

template <typename priority_t>
size_t threadpool<priority_t>::get_threads(size_t nthreads) {
  return std::max(std::min((size_t)std::thread::hardware_concurrency(),
        nthreads), (size_t)1);
}

template <typename priority_t>
void threadpool<priority_t>::worker() {
  while(running) {
    auto [priority, task] = tasks.top_and_pop();
    std::thread::id this_id = std::this_thread::get_id();
    std::stringstream ss;
    ss << "thread #" << this_id << " taking task of priority " << priority
      << "\n";
    std::cout << ss.str();
    task();
  }
}

template <typename priority_t>
threadpool<priority_t>::threadpool(size_t nthreads_) :
  nthreads(threadpool::get_threads(nthreads_)){
  for (int i = 0; i < nthreads; i++) {
    threads.emplace_back(std::bind(&threadpool::worker, this));
  }
}

template <typename priority_t>
threadpool<priority_t>::~threadpool() {
  running = false;
  std::vector<std::future<void>> results;
  for (int i = 0; i < threads.size(); i++) {
    results.emplace_back(std::move(push_task(
            std::numeric_limits<priority_t>::max(),
            std::function<void()>(std::bind(&threadpool::stop, this)))));
  }
  for (int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }
}

template <typename priority_t>
template <class F, class ...Args>
std::future<std::invoke_result_t<F, Args...>>
threadpool<priority_t>::push_task(priority_t priority,
    F&& func, Args&&... args) {
  using return_t = std::invoke_result_t<F, Args...>;
  std::packaged_task<return_t(Args...)> p(func);
  std::future<return_t> ret = p.get_future();
  auto task = [f = std::move(p),
     args = std::make_tuple(std::forward<Args>(args) ...)]() mutable {
       std::apply([pf = std::move(f)](auto&& ... args) mutable {
        pf(std::forward<Args>(args)...);
      }, std::move(args));
  };
  tasks.push(std::make_pair(priority,
        func_t(make_shared_function(std::move(task)))));
  return std::move(ret);
}

}
