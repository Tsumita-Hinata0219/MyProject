#pragma once

#include "Math/MyMath.h"


namespace GpuField {

namespace Data {


// ConstantFieldの構造体
struct ConstantField
{
    Vector3 acceleration; // 加速度
    float damping; // 速度の減衰率（0.0～1.0）
    Vector3 angularVelocity; // 角速度
    float drag; // 空気抵抗
    Vector3 force; // 外力
    float mass; // 質量
    uint32_t isUse; // 使用するか
};

}
}