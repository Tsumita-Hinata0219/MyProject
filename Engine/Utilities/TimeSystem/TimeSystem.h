#pragma once


class TimeSystem  {

private:

	// シングルトン
	TimeSystem () = default;
	~TimeSystem () = default;
	TimeSystem (const TimeSystem &) = delete;
	TimeSystem & operator=(const TimeSystem &) = delete;
	
public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static TimeSystem* GetInstance() {
		static TimeSystem instance;
		return &instance;
	}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

#pragma region Accessor

	// ゲーム開始からの時間
	float Get_SinceStart()const { return this->timeSinceStart_; }

#pragma endregion


private:

	/// <summary>
	/// ImGuiの描画
	/// </summary>
	void DrawImGui();


private:

	// ゲーム開始からの時間
	float timeSinceStart_ = 0.0f;

};
