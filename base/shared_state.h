#pragma once

namespace dist {

template<typename T>
struct shared_state{
  enum status{INIT, SATISFIED, ERRORED} state;
  bool future_exists{false};
  bool promise_exists{false};
  mutable std::mutex mut;
  mutable std::condition_variable cv;
  T value;
  std::exception* e;
};

}
