#pragma once

#include "Math/MyMath.h"

template <typename T>
bool TryLock(const std::weak_ptr<T>& wp, std::shared_ptr<T>& out) {
	out = wp.lock();
	return static_cast<bool>(out);
}
