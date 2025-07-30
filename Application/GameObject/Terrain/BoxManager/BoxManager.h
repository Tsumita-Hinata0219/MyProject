#pragma once

#include "../../GameObject.h"
#include "Interface/IBoxObject.h"
#include "Barrier/BarrierBox.h"
#include "Destructible/DestructibleBox.h"

#include "JsonManager/JsonEntityData.h"


/* BoxManagerクラス */
class BoxManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BoxManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BoxManager() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw3D();

	/// <summary>
	/// Jsonで読み込んだ情報を受け取る
	/// </summary>
	void LoadEntityData(const std::vector<std::unique_ptr<EntityData>>& datas);

	/// <summary>
	/// 新しいボックスを作る
	/// </summary>
	void CreateNewBox(BoxType type, const SRTData& setSRT);

private:

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();


private:

	// Boxのリスト
	std::list<std::shared_ptr<IBoxObject>> boxs_;

};

