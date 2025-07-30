#pragma once

#include "System/IObject/IObject.h"
#include "../../GameObject.h"

#include "JsonManager/JsonEntityData.h"


/* Floorクラス */
class Floor : public IObject {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Floor() { attribute_ = { Attributes::Category::TERRAIN, Attributes::Type::SOLID }; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Floor() {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	///  更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D() override;
	void Draw2DFront() override;
	void Draw2DBack() override;

	/// <summary>
	/// 衝突判定コールバック関数
	/// </summary>
	void onCollision([[maybe_unused]] IObject* object) override;

	/// <summary>
	/// Jsonで読み込んだ情報を受け取る
	/// </summary>
	void LoadEntityData(const std::vector<std::unique_ptr<EntityData>>& data) {

		for (const auto& entityData : data) {
			if (entityData) {
				// SRTの設定
				trans_.srt = entityData->srt;
			}
		}
	}


	void SetScale(const Vector3& setScale) { this->trans_.srt.scale = setScale; }
	void SetTranslate(const Vector3& setTranslate) { this->trans_.srt.translate = setTranslate; }

private:

	// モデル
	std::unique_ptr<Model> model_;

	// Light
	DirectionalLightData light_;

	// ワールドトランスフォーム
	Transform trans_{};
};

