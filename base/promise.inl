#pragma once

namespace dist {

template<typename T>
promise<T>::promise() : state(new shared_state<T>()) {
  state->promise_exists = true;
}

template<typename T>
promise<T>::promise(promise&& other) {
  state = other.state;
  other.state = nullptr;
};

template<typename T>
promise<T>::~promise() {
  if(state == nullptr)
    return;
  std::unique_lock<std::mutex> lk(state->mut);
  state->promise_exists = false;
  if (!state->future_exists) {
    lk.unlock();
    delete state;
  }
}

template<typename T>
future<T> promise<T>::get_future() {
  std::unique_lock<std::mutex> lk(state->mut);
  if (state->future_exists) {
    throw std::runtime_error("future already returned");
  }
  lk.unlock();
  future<T> temp(state);
  lk.lock();
  return temp;
}

template<typename T>
void promise<T>::set_value(T value) {
  std::unique_lock lk(state->mut);
  if (state->state != shared_state<T>::status::INIT) {
    throw std::runtime_error("state already set");
  }
  state->value = value;
  state->state = shared_state<T>::status::SATISFIED;
  state->cv.notify_one();
}

template<typename T>
void promise<T>:: set_exception(std::exception& e) {
  std::unique_lock lk(state->mut);
  if (state->state != shared_state<T>::status::INIT) {
    throw std::runtime_error("state already set");
  }
  state->e = new std::exception(e);
  state->state = shared_state<T>::status::ERRORED;
  state->cv.notify_one();
}

}
