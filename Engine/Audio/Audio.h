#pragma once

#include "AudioDataResource/AudioDataResource.h"

#include <map>



// チャンクヘッダ
struct ChunkHeader {
	char id[4];	  // チャンク毎のID
	int32_t size; // チャンクサイズ
};
// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk; // "RIFF"
	char type[4];	   // "WAVE"
};
// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk; // "fmt"
	WAVEFORMATEX fmt;  // 波形フォーマット
};


/* Audioクラス */
class Audio {

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 音声データの解放
	/// </summary>
	static void Finalize();

	/// <summary>
	/// Soundデータを読み込む
	/// </summary>
	static uint32_t LoadSound(const std::string& filePath);

	/// <summary>
	/// サウンド再生
	/// </summary>
	static void PlayOnSound(uint32_t soundDataNum, bool loopFlag, float volum);

	/// <summary>
	/// サウンド停止
	/// </summary>
	static void StopOnSound(uint32_t soundDataNum);

	/// <summary>
	/// サウンド再生中か
	/// </summary>
	static bool IsPlaying(uint32_t soundDataNum);

	/// <summary>
	/// サウンドのボリュームの設定
	/// </summary>
	static void SetSoundVolum(UINT soundDataNum, float volum);

	/// <summary>
	/// 音声データの解放
	/// </summary>
	static void SoundUnload();


private: // メンバ関数

	/// <summary>
	/// 一回読み込んだものは読み込まない
	/// </summary>
	static bool CheckAudioDatas(std::string filePath);


private: // メンバ変数

	ComPtr<IXAudio2> xAudio2_ = nullptr;
	IXAudio2MasteringVoice* masterVoice_;
	uint32_t soundDataNum_;
	std::map<std::string, std::unique_ptr<AudioDataResource>>AudioDatas_;

};