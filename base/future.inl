#pragma once

namespace dist {

template<typename T>
future<T>::future(shared_state<T>* state_):state(state_) {
  std::lock_guard<std::mutex> lk(state->mut);
  state->future_exists = true;
}

template<typename T>
future<T>::future():state(new shared_state<T>) {
  state->future_exists = true;
}

template<typename T>
future<T>::future(future&& other) {
  this->state = other.state;
  other.state = nullptr;
}

template<typename T>
future<T>::~future() {
std::unique_lock<std::mutex> lk(state->mut);
  state->future_exists = false;
  if (!state->promise_exists) {
    lk.unlock();
    delete state;
  }
}

template<typename T>
bool future<T>::locked_valid() {
  return state->state == shared_state<T>::status::SATISFIED ||
    state->state == shared_state<T>::status::ERRORED;
}

template<typename T>
T future<T>::get() {
  wait();
  return std::move(state->value);
}

template<typename T>
void future<T>::wait() {
  std::unique_lock lk(state->mut);
  if(!locked_valid())
    state->cv.wait(lk, [&]() {return locked_valid();});
}

template<typename T>
template<typename duration_t>
void future<T>::wait_for(std::chrono::duration<duration_t> time) {
  std::unique_lock lk(state->mut);
  return locked_valid() || state->cv.wait_for(lk, time,
      [&]() {return locked_valid();});
}

template<typename T>
void future<T>::wait_until(
    std::chrono::time_point<std::chrono::system_clock> tp) {
  std::unique_lock lk(state->mut);
  return locked_valid() || state->cv.wait_until(lk, tp,
      [&]() {return locked_valid();});
}

template<typename T>
bool future<T>::valid() {
  std::lock_guard lk(state->mut);
  return locked_valid();
}

}
