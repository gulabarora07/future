#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <functional>
#include <stdexcept>

namespace dist {
// forward declaration
template<typename T> class future;
template<typename T> class promise;
}

#include "shared_state.h"
#include "future.h"
#include "promise.h"
#include "packaged_task.h"

#include "future.inl"
#include "promise.inl"
