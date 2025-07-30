#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"


// PlayerBodyのタイプ
enum class PlayerBodyTyep {
	MAIN,
	LEFT,
	RIGHTM,
	Count,
};

// Player前方宣言
class Player;
struct DirectionalLightData;

/* PlayerBodyの基底クラス */
class IPlayerBody : public IObject {

public:

	// 仮想デストラクタ
	virtual ~IPlayerBody() = default;

	// ペアレントを結ぶ
	virtual void SetParent(Transform* parent) = 0;

	// モデルのカラーの設定
	virtual void SetModelColor(const Vector4& setColor) = 0;

	// モデルのライトの設定
	virtual void SetModelLight(const DirectionalLightData& setLight) = 0;
};