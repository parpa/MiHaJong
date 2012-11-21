#include "soundobj.h"

sound::SoundManipulator::SoundManipulator() {
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
		throw "CoInitializeEx失敗！！";

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	if (FAILED(XAudio2Create(&xAudio, flags)))
		throw "XAudio2Create失敗！！";

	if (FAILED(xAudio->CreateMasteringVoice(&mVoice)))
		throw "CreateMasteringVoice失敗！！";
}

sound::SoundManipulator::~SoundManipulator() {
	if (mVoice) {
		mVoice->DestroyVoice(); mVoice = nullptr;
	}
	if (xAudio) {
		xAudio->Release(); xAudio = nullptr;
	}
	CoUninitialize();
}
