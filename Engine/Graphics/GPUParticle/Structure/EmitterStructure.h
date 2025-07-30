#pragma once

#include "Math/MyMath.h"


namespace Emitter {

namespace Data {

// Emitterの基底構造体
struct IEmitData {
    Vector4 translate{}; // 座標
};

// Sphere 派生構造体
struct SphereEmit : IEmitData {
    Vector4 radius{}; // 半径
};

// Box 派生構造体
struct BoxEmit : IEmitData {
    Vector4 min{}; // 
    Vector4 max{}; // 
};

// 範囲
struct EmitRange {
    // SR
    Vector4 scaleMin;
    Vector4 scaleMax;
    Vector4 rotateMin;
    Vector4 rotateMax;
    // Color
    Vector4 colorMin;
    Vector4 colorMax;
    // Motion
    Vector4 velocityMin;
    Vector4 velocityMax;
    // Life
    float baseLifeTime = 0.0f;
    float lifeTimeMin = 0.0f;
    float lifeTimeMax = 0.0f;
};

// 射出
struct EmitConfig {
    uint32_t spawnCount; // 生成数
    float spawnInterval = 0.0f; // インターバル
    float elapsedTime = 0.0f; // 射出タイム
    uint32_t isEmitting; // 射出許可フラグ (0: 停止, 1: 許可)
};

// 乱数のシード情報
struct RandomSeed {
    float gameTime = 0.0f;
    float dynamicTime = 0.0f;
};

}
}