#include "TimeSystem.h"


/// <summary>
/// 初期化処理
/// </summary>
void TimeSystem::Init()
{
	// ゲーム開始からの時間の初期化
	timeSinceStart_ = 0.0f;
}


/// <summary>
/// 更新処理
/// </summary>
void TimeSystem::Update()
{
	// 時間を加算
	timeSinceStart_ += 1.0f / 60.0f;

	// 10万秒(約28時間)ごとにリセット
	if (timeSinceStart_ >= 100000.0f) {
		timeSinceStart_ -= 100000.0f;
	}

#ifdef _DEBUG
	DrawImGui();
#endif // _DEBUG
}


/// <summary>
/// ImGuiの描画
/// </summary>
void TimeSystem::DrawImGui()
{
	
}
