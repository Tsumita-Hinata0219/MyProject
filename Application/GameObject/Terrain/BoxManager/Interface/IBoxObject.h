#pragma once

#include <list>
#include <memory>

#include "System/IObject/IObject.h"
#include "../../../GameObject.h"


// 前方宣言
class BoxManager;
struct SRTData;

// Boxのタイプ
enum class BoxType {
	Barrier,
	Destructible,
};


/* Boxの基底クラス */
class IBoxObject : public IObject {

public:

	/// <summary>
	/// 仮想デストラク
	/// </summary>
	virtual ~IBoxObject() = default;


#pragma region Accessor アクセッサ

	// Manager
	virtual void SetManager(BoxManager* setManager) { this->manager_ = setManager; }

	// IsDead
	bool IsDead() const { return this->isDead_; }

	// SRT
	virtual void SetSRT(const SRTData& setSRT) = 0;
	virtual void SetInitSRT(const SRTData& setSRT) { this->initSRT_ = setSRT; }

	// Scale
	virtual void SetScale(const Vector3& setScale) = 0;

	// Rotate
	virtual void SetRotate(const Vector3& setRotate) = 0;

	// Translate
	virtual void SetTranslate(const Vector3& setTranslate) = 0;

#pragma endregion 


protected:

	// マネージャー
	BoxManager* manager_ = nullptr;

	// SRT
	SRTData initSRT_{};

	// HP
	int HP_ = 0;

	// 死亡フラグ
	bool isDead_ = false;

};