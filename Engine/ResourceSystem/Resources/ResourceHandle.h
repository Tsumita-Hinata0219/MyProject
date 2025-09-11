#pragma once
#include <cstdint>

// リソースへの参照を安全にするための「ID（整数）」
struct ResourceHandle {
    uint32_t id = UINT32_MAX;
    bool IsValid() const { return id != UINT32_MAX; }
};
