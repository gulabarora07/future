#pragma once

namespace dist {

struct base_mutable_struct {
  mutable std::mutex mut;
  mutable std::condition_variable cv;
};

template<typename T>
struct shared_state : public base_mutable_struct {
  enum status{INIT, SATISFIED, ERRORED} state;
  bool future_exists{false};
  bool promise_exists{false};
  T value;
  std::exception* e;
};

}
