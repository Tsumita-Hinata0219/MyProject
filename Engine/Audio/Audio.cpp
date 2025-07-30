#include "Audio.h"



/// <summary>
/// インスタンスの取得
/// </summary>
Audio* Audio::GetInstance() {
	static Audio instance;
	return &instance;
}



/// <summary>
/// 初期化処理
/// </summary>
void Audio::Initialize() {

	// XAudio2エンジンのインスタンス生成
	HRESULT result = XAudio2Create(&Audio::GetInstance()->xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイス生成
	result = Audio::GetInstance()->xAudio2_->CreateMasteringVoice(&Audio::GetInstance()->masterVoice_);
	assert(SUCCEEDED(result));
}



/// <summary>
/// 音声データの解放
/// </summary>
void Audio::Finalize() {

	Audio::GetInstance()->SoundUnload();
	Audio::GetInstance()->xAudio2_.Reset();
}



/// <summary>
/// Soundデータを読み込む
/// </summary>
uint32_t Audio::LoadSound(const std::string& filePath) {

	uint32_t index = 0;

	if (CheckAudioDatas(filePath)) {

		Audio::GetInstance()->soundDataNum_++;

		/* 1. ファイルオープン */

		// ファイルの入力ストリームのインスタンス
		std::ifstream file;
		// .wavファイルをバイナリーモードで開く
		file.open("Resources/Audio/" + filePath, std::ios_base::binary);
		// ファイルオープン失敗を検出する
		assert(SUCCEEDED(file.is_open()));



		/* 2. wavデータ読み込み */

		// RIFFヘッダーの読み込み
		RiffHeader riff{};
		file.read((char*)&riff, sizeof(riff));

		// ファイルがRIFFかチェック
		if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
			assert(0);
		}
		// タイプがWAVEかチェック
		if (strncmp(riff.type, "WAVE", 4) != 0) {
			assert(0);
		}

		// Formatチャンクの読み込み
		FormatChunk format{};

		// チャンクヘッダーの確認
		file.read((char*)&format, sizeof(ChunkHeader));
		if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
			assert(0);
		}

		// チャンク本体の読み込み
		assert(format.chunk.size <= sizeof(format.fmt));
		file.read((char*)&format.fmt, format.chunk.size);

		// Dataチャンクの読み込み
		ChunkHeader data{};
		file.read((char*)&data, sizeof(data));

		// JUNKチャンクを検出した場合
		while (_strnicmp(data.id, "junk", 4) == 0 || _strnicmp(data.id, "bext", 4) == 0 || _strnicmp(data.id, "LIST", 4) == 0) {
			// 読み取り位置をJUNKチャンクの終わりまで進める
			file.seekg(data.size, std::ios_base::cur);
			// 再読み込み
			file.read((char*)&data, sizeof(data));
		}
		if (strncmp(data.id, "data", 4) != 0) {
			assert(0);
		}

		// Dataチャンクのデータ部(波形データ)の読み込み
		char* pBuffer = new char[data.size];
		file.read(pBuffer, data.size);



		/* 3. ファイルクローズ */

		file.close();



		/* 4. 読み込んだ音声データをreturn */

		// returnするための音声データ
		SoundData soundData{};
		soundData.wfex = format.fmt;
		soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
		soundData.bufferSize = data.size;
		soundData.index = Audio::GetInstance()->soundDataNum_;
		soundData.volum = 1.0f;
		Audio::GetInstance()->AudioDatas_[filePath] = make_unique<AudioDataResource>(filePath, soundData);

		index = Audio::GetInstance()->soundDataNum_;
	}
	else {

		index = Audio::GetInstance()->AudioDatas_[filePath].get()->GetSoundData().index;
	}

	return index;
}



/// <summary>
/// サウンド再生
/// </summary>
void Audio::PlayOnSound(uint32_t soundDataNum, bool loopFlag, float volum = 1.0f) {

	for (const auto& [key, s] : Audio::GetInstance()->AudioDatas_) {
		key;


		if (s.get()->GetSoundData().index == soundDataNum) {

			HRESULT result{};


			// 既存の再生を停止して解放
			if (s.get()->GetSoundData().pSourceVoice) {
				s.get()->GetSoundData().pSourceVoice->Stop();
				s.get()->GetSoundData().pSourceVoice->FlushSourceBuffers();
				s.get()->GetSoundData().pSourceVoice->DestroyVoice();
				s.get()->SetSoundIsPlaying(false);
			}


			// 波形フォーマットをもとにSoundVoiceの生成
			IXAudio2SourceVoice* pSourcevoice = {};
			WAVEFORMATEX wfex = s.get()->GetSoundData().wfex;
			result = Audio::GetInstance()->xAudio2_->CreateSourceVoice(&pSourcevoice, &wfex);
			assert(SUCCEEDED(result));
			s.get()->SetSoundResource(pSourcevoice);
			s.get()->SetSoundWfex(wfex);


			// 再生する波形データの設定
			XAUDIO2_BUFFER buf{};
			buf.pAudioData = s.get()->GetSoundData().pBuffer;
			buf.AudioBytes = s.get()->GetSoundData().bufferSize;
			buf.Flags = XAUDIO2_END_OF_STREAM;
			if (loopFlag) {
				// ループ
				buf.LoopCount = XAUDIO2_LOOP_INFINITE;
			}


			// 波形データの再生
			result = s.get()->GetSoundData().pSourceVoice->SubmitSourceBuffer(&buf);
			result = s.get()->GetSoundData().pSourceVoice->SetVolume(volum);
			result = s.get()->GetSoundData().pSourceVoice->Start();
			s.get()->SetSoundIsPlaying(true);


			assert(SUCCEEDED(result));

			return;
		}
	}
}



/// <summary>
/// サウンド停止
/// </summary>
void Audio::StopOnSound(uint32_t soundDataNum) {


	for (const auto& [key, s] : Audio::GetInstance()->AudioDatas_) {
		key;

		if (s.get()->GetSoundData().index == soundDataNum) {

			HRESULT result{};

			if (s.get()->GetSoundData().pSourceVoice) {

				result = s.get()->GetSoundData().pSourceVoice->Stop();
				s.get()->SetSoundIsPlaying(false);
				assert(SUCCEEDED(result));
			}
		}
	}

	return;
}



/// <summary>
/// サウンド再生中か
/// </summary>
bool Audio::IsPlaying(uint32_t soundDataNum) {

	for (const auto& [key, s] : Audio::GetInstance()->AudioDatas_) {
		key;

		if (s.get()->GetSoundData().index == soundDataNum) {

			if (s.get()->GetSoundData().pSourceVoice) {

				return s.get()->GetSoundData().isPlaying;
			}
		}
	}

	return false;
}



/// <summary>
/// サウンドのボリュームの設定
/// </summary>
void Audio::SetSoundVolum(UINT soundDataNum, float volum = 1.0f) {

	for (const auto& [key, s] : Audio::GetInstance()->AudioDatas_) {
		key;

		if (s.get()->GetSoundData().index == soundDataNum) {

			HRESULT result{};

			result = s.get()->GetSoundData().pSourceVoice->SetVolume(volum);

			assert(SUCCEEDED(result));
		}
	}
}



/// <summary>
/// 音声データの解放
/// </summary>
void Audio::SoundUnload() {

	Audio::GetInstance()->AudioDatas_.clear();
	Audio::GetInstance()->xAudio2_.Reset();
}



/// <summary>
/// 一回読み込んだものは読み込まない
/// </summary>
bool Audio::CheckAudioDatas(std::string filePath) {

	// filePaht同じものがあるならそれを返す
	if (Audio::GetInstance()->AudioDatas_.find(filePath) == Audio::GetInstance()->AudioDatas_.end()) {

		return true;
	}
	return false;
}
