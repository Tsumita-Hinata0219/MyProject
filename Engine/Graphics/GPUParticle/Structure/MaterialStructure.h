#pragma once

#include "Math/MyMath.h"


namespace GpuParticle {

namespace Material {

// FadeOutに関する構造体
struct FadeOut {
    float start; // フェード開始時の ratio（例: 0.7）
    float end; // フェード終了時の ratio（例: 1.0）
    float power; // フェードの強さ（ease調整用、例: 1.0 = 線形, 2.0 = 緩やか, 0.5 = 急）
};

}
}
