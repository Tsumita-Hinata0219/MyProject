#include "AudioDataResource.h"



// コンストラクタ
AudioDataResource::AudioDataResource(std::string filePath, SoundData soundData) {

	filePath_ = filePath;
	soundData_ = soundData;
}


// デストラクタ
AudioDataResource::~AudioDataResource() {

	delete[] soundData_.pBuffer;
	soundData_.pBuffer = 0;
	soundData_.bufferSize = 0;
	soundData_.wfex = {};
	soundData_.volum = 0.0f;
	soundData_.isPlaying = false;
}
