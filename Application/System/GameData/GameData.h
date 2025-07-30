#pragma once



/* シーンをまたいで使用したい値を保存しておくクラス */
class GameData {

private: // シングルトン

	GameData() = default;
	~GameData() = default;
	GameData(const GameData&) = delete;
	const GameData& operator=(const GameData&) = delete;


public:

	static GameData* GetInstance() {
		static GameData instance;
		return &instance;
	}


#pragma region SelectStageNumber

	// 取得
	int Get_StageSelectNum() const { return this->stageSelectNum_; }
	// 設定
	void Set_StageSelectNum(int setNum) { this->stageSelectNum_ = setNum; }
	// 次のステージ
	void NextStageSet() { this->stageSelectNum_++; }

#pragma endregion

#pragma region JsonFileName

	// 追加s
	void AddStageJsonFilePath(const std::string& path) {
		stageJsonFilePaths_.push_back(path);
	}
	// 特定のファイルパスの取得
	std::string GetStageJsonFilePathAt(int index) const {
		if (index < 0 || index >= static_cast<int>(stageJsonFilePaths_.size())) {
			throw std::out_of_range("Index out of range in GetStageJsonFilePathAt");
		}
		return stageJsonFilePaths_[index];
	}

#pragma endregion


private:	

	// 選択したステージ番号
	int stageSelectNum_ = 1;

	// ステージのJsonファイル名
	std::vector<std::string> stageJsonFilePaths_;

};
