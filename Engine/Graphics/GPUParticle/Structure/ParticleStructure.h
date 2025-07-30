#pragma once

#include "Math/MyMath.h"


namespace GpuParticle {

// Particleを表す構造体
struct PropertyCS {
	Vector3 scale{};
	Vector3 rotate{};
	Vector3 translate{};
	Matrix4x4 matWorld{};
	Vector4 color{};
	Vector3 velocity;
	uint32_t isAlive;
};

// Particleの運動に関する構造体
struct ParticleMotion {
	Vector3 acceleration; // 加速度
	float damping; // 速度の減衰率（0.0～1.0）
	Vector3 angularVelocity; // 角速度
	Vector3 force; // 外力
	float drag; // 空気抵抗
};

// Particleの生存時間に関する構造体
struct ParticleLifeTime {
	float current; // 現在の時間
	float start;   // スタート時の時間
	float end;	   // 終了時間
	float ratio;   // 進行具合
};

// PreView
struct PreView {
	Matrix4x4 viewProjection;
	Matrix4x4 billboardMatrix;
};

}