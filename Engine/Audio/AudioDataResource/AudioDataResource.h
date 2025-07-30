#pragma once

#include <xaudio2.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xaudio2.lib")

#include <fstream>
#include <sstream>
#include <cassert>

#include "Math/MyMath.h"
#include "Math/Struct.h"



// サウンドデータ
struct SoundData {
	WAVEFORMATEX wfex;		 // 波形フォーマット
	BYTE* pBuffer;			 // バッファの先頭アドレス
	unsigned int bufferSize; // バッファのサイズ
	IXAudio2SourceVoice* pSourceVoice;
	uint32_t index;
	float volum;
	bool isPlaying;
};


/* AudioResourceData */
class AudioDataResource {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	AudioDataResource(std::string filePath, SoundData soundData);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioDataResource();


#pragma region Get

	/// <summary>
	/// サウンドデータの取得
	/// </summary>
	/// <returns></returns>
	SoundData GetSoundData() { return soundData_; }

#pragma endregion


#pragma region Set

	/// <summary>
	/// サウドデータの設定
	/// </summary>
	void SetSoundData(SoundData soundData) { soundData_ = soundData; }

	/// <summary>
	/// リソースの設定
	/// </summary>
	void SetSoundResource(IXAudio2SourceVoice* pSourceVoice) { soundData_.pSourceVoice = pSourceVoice; }

	/// <summary>
	/// 波形フォーマットの設定
	/// </summary>
	void SetSoundWfex(WAVEFORMATEX wfex) { soundData_.wfex = wfex; }

	/// <summary>
	/// isPlayingの設定
	/// </summary>
	void SetSoundIsPlaying(bool flag) { soundData_.isPlaying = flag; }

#pragma endregion 


private:

	std::string filePath_;
	SoundData soundData_{};
};