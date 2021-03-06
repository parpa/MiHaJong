﻿#include "sound.h"

#include "../common/bgmid.h"
#include "../common/strcode.h"
#include "func.h"
#include "ruletbl.h"
#include <vector>
#include "regex.h"

#ifdef None
#undef None
#endif

namespace {
	enum bgmMode {None, Vorbis, Midi,};
	std::vector<bgmMode> BGM_Mode;
}

/* BGM読み込み */
void sound::util::bgmload(unsigned ID, const char* filename, bool looped) {
	if (BGM_Mode.size() <= ID) BGM_Mode.resize(ID + 1, None);
	CodeConv::tostringstream o;
	std::string oggfile = std::string("bgm\\") + std::string(filename) + std::string(".ogg");
	std::string midifile = std::string("bgm\\") + std::string(filename) + std::string(".mid");
	if (exist(oggfile.c_str())) {
		if (LoadVorbis(ID, oggfile.c_str(), looped ? 1 : 0) == 0) {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".ogg] の読み込みを完了しました。");
			info(o.str().c_str());
			BGM_Mode[ID] = Vorbis;
		} else {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".ogg] の読み込みに失敗しました。");
			warn(o.str().c_str());
		}
	}
	else {
		o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".ogg] はありません。");
		if (LoadMidi(ID, midifile.c_str(), looped ? 1 : 0) == 0) {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".mid] を読み込みました。");
			info(o.str().c_str());
			BGM_Mode[ID] = Midi;
		} else {
			o << _T("BGMファイル [") << CodeConv::EnsureTStr(filename) << _T(".mid] の読み込みに失敗しました。");
			warn(o.str().c_str());
		}
	}
	return;
}

/* サウンド読み込み */
void sound::util::soundload(unsigned ID, const char* filename, bool looped) {
	CodeConv::tostringstream o;
	if (sound::LoadWave(ID, filename, looped ? 1 : 0) == 0) {
		o << _T("音声ファイル [") << CodeConv::EnsureTStr(filename) << _T("] を読み込みました。");
		info(o.str().c_str());
	} else {
		o << _T("音声ファイル [") << CodeConv::EnsureTStr(filename) << _T("] の読み込みに失敗しました。");
		warn(o.str().c_str());
	}
	return;
}

/* BGM停止 */
void sound::util::bgmstop() {
	info(_T("BGM再生を停止します。"));
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		if (BGM_Mode[i] != None)
			Stop(i);
	return;
}

/* ＢＧＭ再生 */
void sound::util::bgmplay(unsigned ID) {
	CodeConv::tostringstream o;
	bgmstop();
	if (sound::Play(ID) == 0) {
		o << _T("BGM [") << ID << _T("] 番を再生します。");
		info(o.str().c_str());
	} else {
		o << _T("BGM [") << ID << _T("] 番を再生できませんでした。");
		warn(o.str().c_str());
	}
	return;
}

/* 音量設定を反映 */
void sound::util::setvolume() {
	using namespace sound;
	auto getvolume = [] (std::string ruleTag) -> double {
		const std::string chipRule(RuleData::chkPreference(ruleTag));
		REGEX::smatch matchDat; int volperc = 100;
		if (REGEX::regex_match(chipRule, matchDat, REGEX::regex("vol_(\\d+)")))
			volperc = atoi(matchDat[1].str().c_str()); // ルール設定文字列から整数を抽出
		return (double)volperc / 100.0;
	};
	for (unsigned i = IDs::BgmStart; i <= IDs::BgmEnd; i++)
		if (BGM_Mode[i] != None)
			SetVolume(i, getvolume("bgmvolume"));
	for (unsigned i = IDs::SndStart; i <= IDs::SndEnd; i++)
		SetVolume(i, getvolume("sndvolume"));
	for (unsigned i = IDs::VoxStart; i <= IDs::VoxEnd; i++)
		SetVolume(i, getvolume("sndvolume"));
}
