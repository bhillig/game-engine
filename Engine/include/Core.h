#pragma once

#include <memory>
#include <functional>

#define CORE_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1) 

namespace Core
{

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

}
