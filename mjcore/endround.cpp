#include "endround.h"

#include "ruletbl.h"
#include "func.h"
#include "logging.h"
#include <functional>
#include <array>
#include <tuple>
#include <cassert>
#include "../sound/sound.h"
#include "bgmid.h"
#include "../graphic/graphic.h"
#include "tileutil.h"
#include "haifu.h"
#include "chat.h"
#include "agari.h"
#include "envtbl.h"

// 食い変え判定用の gameStat->AgariSpecialStat 番号
#define AGARI_KUIKAE 999

namespace { // 内部処理に使う関数
	bool all_player(const GameTable* gameStat, std::function<bool (const PlayerTable*)> f) {
		bool flag = true;
		if (chkGameType(gameStat, Sanma4)) {
			for (PLAYER_ID i = 0; i < PLAYERS; ++i)
				flag = flag && ((playerwind(gameStat, i, gameStat->GameRound) == sNorth) || f(&(gameStat->Player[i])));
		} else if (chkGameType(gameStat, SanmaT)) {
			for (PLAYER_ID i = 0; i < 3; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		} else {
			for (PLAYER_ID i = 0; i < PLAYERS; ++i)
				flag = flag && f(&(gameStat->Player[i]));
		}
		return flag;
	}
	bool chksifeng(const GameTable* gameStat) {
		bool flag = false;
		std::array<tileCode, 4> winds = {EastWind, SouthWind, WestWind, NorthWind,};
		for (auto k = winds.begin(); k != winds.end(); ++k) {
			bool tmpflag = true;
			if (chkGameType(gameStat, Sanma4)) {
				for (PLAYER_ID i = 0; i < PLAYERS; ++i)
					tmpflag = tmpflag && ((playerwind(gameStat, i, gameStat->GameRound) == sNorth) || (gameStat->Player[i].Discard[1].tcode.tile == *k));
			} else if (chkGameType(gameStat, SanmaT)) {
				for (PLAYER_ID i = 0; i < 3; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == *k);
			} else {
				for (PLAYER_ID i = 0; i < PLAYERS; ++i)
					tmpflag = tmpflag && (gameStat->Player[i].Discard[1].tcode.tile == *k);
			}
			flag = flag || tmpflag;
		}
		return flag;
	}

	void logKuikae(const GameTable* gameStat, bool chonbo) {
		CodeConv::tostringstream o;
		o << _T("喰い変えを検出しました。") <<
			(chonbo ? _T("チョンボ") : _T("和了り放棄")) << _T("として扱います。打牌 [") <<
			(int)gameStat->CurrentDiscard.tile << _T("] 鳴いた牌 [") <<
			(int)gameStat->PreviousMeld.Discard << _T("]");
		info(o.str().c_str());
		sound::Play(sound::IDs::sndCuohu);
	}
	bool chkKuikae(GameTable* gameStat) { // 喰い替えの場合の処理
		if (((gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Discard) || // 現物の食い変えになっている場合か
			(gameStat->CurrentDiscard.tile == gameStat->PreviousMeld.Stepped)) && // 筋食い変えになっている場合で
			(!gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki)) { // まだアガリ放棄になっていないなら
				if (RuleData::chkRule("kuikae", "agari_houki") || RuleData::chkRule("kuikae", "agari_houki_if_in_kind")) {
					/* 和了り放棄とする設定 */
					logKuikae(gameStat, false);
					for (unsigned i = 0; i < 2; ++i) {
						mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Kuikae);
						mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
						mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::AgariHouki);
						mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
					}
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
					mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
					mihajong_graphic::GameStatus::updateGameStat(gameStat);
					/* 和了り放棄は以降強制ツモ切り、強制不聴扱いとなります */
					gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
					/* TODO: 移植するかもしれないし廃止するかもしれない if (GetWatchModeFlag(GameEnv) == 0) {statmes "和了り放棄：強制ツモ切りされます"}*/
					/* TODO: これについて確認すること vanish2@ */
				} else if (RuleData::chkRule("kuikae", "chombo") || RuleData::chkRule("kuikae", "chombo_if_in_kind")) {
					/* 直ちに錯和とする設定 */
					logKuikae(gameStat, true);
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Chonbo);
					mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
					/* 局を打ち切り、満貫罰符の支払いに進む */
					return true;
				}
		}
		return false;
	}

	void chkTahai(GameTable* gameStat) {
		/* 多牌や少牌をしていないかのチェック */
		unsigned tmptilecnt = 0;
		for (unsigned i = 0; i < NUM_OF_TILES_IN_HAND; ++i)
			if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[i].tile != NoTile)
				++tmptilecnt;
		tmptilecnt += gameStat->Player[gameStat->CurrentPlayer.Active].MeldPointer * 3;
		if ((tmptilecnt != (NUM_OF_TILES_IN_HAND - 1)) && (!gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki)) {
			// 多牌や少牌の場合の処理(通常起きることはないはずだが…)
			if (tmptilecnt > (NUM_OF_TILES_IN_HAND - 1))
				warn(_T("多牌を検出しました。和了り放棄として扱いますが、摸打の処理で不整合が起きていた可能性があります。"));
			else
				warn(_T("少牌を検出しました。和了り放棄として扱いますが、摸打の処理で不整合が起きていた可能性があります。"));
			sound::Play(sound::IDs::sndCuohu);
			for (unsigned i = 0; i < 2; ++i) {
				if (tmptilecnt > (NUM_OF_TILES_IN_HAND - 1))
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Tahai);
				else
					mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::Shouhai);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
				mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::AgariHouki);
				mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCallFade); Sleep(1500);
			}
			mihajong_graphic::calltext::setCall(gameStat->CurrentPlayer.Active, mihajong_graphic::calltext::None);
			mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneNone);
			mihajong_graphic::GameStatus::updateGameStat(gameStat);
			gameStat->Player[gameStat->CurrentPlayer.Active].AgariHouki = true;
			/* TODO: 移植するかもしれないし廃止するかもしれない if (GetWatchModeFlag(GameEnv) == 0) {statmes "和了り放棄：強制ツモ切りされます"} */
			/* TODO: これについて確認すること vanish2@ */
		}
		return;
	}
}

EndType endround::checkroundabort(GameTable* gameStat) { // 局終了条件の判定
	/* 四開槓なら流す */
	if (RuleData::chkRuleApplied("four_kong_ryuukyoku") && (gameStat->KangNum == 4) &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->NumberOfQuads < 4;})))
		return Suukaikan;
	// 四人立直・三人立直の判定
	if (RuleData::chkRuleApplied("four_riichi_ryuukyoku") &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->RichiFlag.RichiFlag;})))
		return SuuchaRiichi;
	// 四風連打・三風連打の判定
	if (RuleData::chkRuleApplied("four_wind_ryuukyoku") &&
		(all_player(gameStat, [](const PlayerTable* plDat) {return plDat->DiscardPointer == 1;})) &&
		(chksifeng(gameStat)))
		return SuufonRenda;

	for (PLAYER_ID i = 0; i < PLAYERS; ++i) gameStat->Player[i].Hand[NUM_OF_TILES_IN_HAND - 1].tile = NoTile; // バグ防止のため
	if (chkKuikae(gameStat)) { // 喰い替えの場合の処理
		gameStat->AgariSpecialStat = AGARI_KUIKAE;
		return Chonbo;
	}
	/* 多牌や少牌をしていないかのチェック */
	chkTahai(gameStat);
	/* 荒牌の場合ここで終了する(河底牌は吃ポンできないがロンはできる) */
	if (gameStat->TilePointer == (gameStat->RinshanPointer - (gameStat->DeadTiles - 1)))
		return Ryuukyoku;
	/* 何事もなかった場合 */
	return Continuing;
}

// -------------------------------------------------------------------------

namespace {
	inline void writeChat(const CodeConv::tstring& ResultDesc) {
		chat::appendchat((CodeConv::tstring(_T("*** ")) + ResultDesc + CodeConv::tstring(_T("\n"))).c_str());
	}

	std::array<bool, PLAYERS> chkNagashiMangan(const GameTable* gameStat, EndType& RoundEndType) { /* 流し満貫の判定 */
		std::array<bool, PLAYERS> NagashiManganFlag = {false,};
		if (RoundEndType == Ryuukyoku) {
			for (unsigned i = 0; i < ACTUAL_PLAYERS; ++i) {
				if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, gameStat->GameRound) == sNorth))
					continue; // 四人三麻の場合北家は無視
				if (RuleData::chkRuleApplied("nagashi_mangan") && isNagashiMangan(gameStat, i)) {
					NagashiManganFlag[i] = true; RoundEndType = NagashiMangan;
				}
			}
		}
		return NagashiManganFlag;
	}

	void ryuukyokuScreen(unsigned soundNum, CodeConv::tstring* ResultDesc, unsigned subsceneCode, unsigned waittime = 3000, unsigned bgmNum = sound::IDs::musRyuukyoku) {
		using namespace mihajong_graphic;
		using namespace CodeConv;
		if (ResultDesc) {
			sound::Play(soundNum);
			writeChat(*ResultDesc);
			Subscene(subsceneCode);
			Sleep(3000);
		}
		for (PLAYER_ID i = 0; i < PLAYERS; ++i)
			calltext::setCall(i, calltext::None);
		sound::Play(sound::IDs::sndPingju);
		sound::Play(bgmNum);
		Subscene(tblSubsceneRyuukyoku);
		Sleep(waittime);
	}

	unsigned checkTenpai(GameTable* gameStat, CodeConv::tstring& ResultDesc, unsigned OrigTurn) {
		unsigned TenpaiCnt = 0;
		for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, OrigTurn) == sNorth))
				continue; // 四人三麻の北家は無視
			if (isTenpai(gameStat, i)) { // 聴牌の時
				++TenpaiCnt; gameStat->Player[i].HandStat = handExposed;
			} else if (gameStat->Player[i].RichiFlag.RichiFlag) { // 錯和(不聴)リーチの時
				gameStat->Player[i].HandStat = handExposed;
			} else { // 不聴の時は伏せる
				gameStat->Player[i].HandStat = handHidden;
			}
		}
		CodeConv::tstring TenpaiCountTxt;
		switch (TenpaiCnt) {
			case 0: TenpaiCountTxt = _T("全員不聴"); break;
			case 1: TenpaiCountTxt = _T("１人聴牌"); break;
			case 2: TenpaiCountTxt = _T("２人聴牌"); break;
			case 3: TenpaiCountTxt = _T("３人聴牌"); break;
			case 4: TenpaiCountTxt = _T("４人聴牌"); break;
		}
		ResultDesc = _T("荒牌流局、") + TenpaiCountTxt;
		/* TODO: これいらないか確認 statmes "流局 "+TenpaiCountTxt */
		writeChat(ResultDesc + _T("です"));
		mihajong_graphic::GameStatus::updateGameStat(gameStat);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneChkTenpai);
		Sleep(5000);
		return TenpaiCnt;
	}

	void transferNotenBappu(GameTable* gameStat, unsigned OrigTurn, unsigned TenpaiCnt) {
		using namespace endround::transfer;
		resetDelta();
		for (PLAYER_ID i = 0; i < ACTUAL_PLAYERS; ++i) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, i, OrigTurn) == sNorth))
				continue; // 四人三麻の北家は無視
			if (chkGameType(gameStat, AllSanma)) { // 三麻の場合
				if ((TenpaiCnt > 0) && (TenpaiCnt < 3)) {
					if (isTenpai(gameStat, i))
						addDelta(i, 3000 / TenpaiCnt);
					else addDelta(i, -3000 / (signed)(3 - TenpaiCnt));
				}
			} else { // 四麻の場合
				if ((TenpaiCnt > 0) && (TenpaiCnt < 4)) {
					if (isTenpai(gameStat, i))
						addDelta(i, 3000 / TenpaiCnt);
					else addDelta(i, -3000 / (signed)(4 - TenpaiCnt));
				}
			}
		}
		if ((TenpaiCnt > 0) && (TenpaiCnt < ACTUAL_PLAYERS))
			transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValNotenBappu, 2500);
	}

	void ryuukyokuProc(GameTable* gameStat, bool RenchanFlag) {
		/* TODO: この辺再考のこと
		repeat NUM_OF_PLAYERS
			setCall cnt, ""
		loop
		setCenterTitle ""
		if (RenchanFlag) {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "連荘"
		} else {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "親流れ"
		}
		snd_play SND_PAGE
		redrscreen
		redraw 1
		*/
		++(gameStat->Honba);
		if (!RenchanFlag) ++(gameStat->GameRound);
		gameStat->AgariChain = 0; gameStat->LastAgariPlayer = -1;
		return;
	}
}

void endround::endround(GameTable* gameStat, EndType roundEndType, unsigned OrigTurn, unsigned OrigHonba) {
	EndType RoundEndType = roundEndType;
	{
		CodeConv::tostringstream o;
		o << _T("局を終了 終了コード [") << (int)RoundEndType << _T(']');
	}
	/* TODO: この辺確認
	statmes ""
	vanish2@ */
	/* 流し満貫の判定 */
	auto NagashiManganFlag = chkNagashiMangan(gameStat, RoundEndType);
	/************/
	/* 和了処理 */
	/************/
	if ((RoundEndType == Agari) || (RoundEndType == Chonbo)) {
		/* TODO: 和了り時の処理 agariproc RoundEndType, GameStat, GameEnv, tmpUraFlag, tmpAliceFlag, ResultDesc */
	}
	CodeConv::tstring ResultDesc; bool RenchanFlag = false;
	switch (RoundEndType) {
	/**************/
	/* 荒牌流局時 */
	/**************/
	case Ryuukyoku:
		/* TODO: これ多分いらない statmes "流局です" */
		ResultDesc = _T("荒牌流局");
		writeChat(ResultDesc);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);
		transferNotenBappu(gameStat, OrigTurn,
			checkTenpai(gameStat, ResultDesc, OrigTurn));

		for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			// 錯和立直（不聴立直）の者がいた場合
			if ((!isTenpai(gameStat, cnt)) && (gameStat->Player[cnt].RichiFlag.RichiFlag)) {
				transferChonboPenalty(gameStat, cnt);
				Sleep(500);
			}
		}
		
		if (RuleData::chkRule("round_continuation", "renchan_if_ready")) {
			if (isTenpai(gameStat, gameStat->GameRound % PLAYERS)) RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_always")) {
			RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_ready_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % PLAYERS) ||
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		} else if (RuleData::chkRule("round_continuation", "renchan_if_mahjong_until_final_round")) {
			if (isTenpai(gameStat, gameStat->GameRound % PLAYERS) &&
				((gameStat->GameLength / ACTUAL_PLAYERS) <= (gameStat->LoopRound * roundLoopRate() + gameStat->GameRound) / ACTUAL_PLAYERS))
				RenchanFlag = true;
		}
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* 和了成立時 */
	/**************/
	case Agari: /* 未実装 */
#if 0
		repeat NUM_OF_PLAYERS
			setCall cnt, ""
		loop
		setCenterTitle ""
		if ((playerwind(getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getRound(GameStat)) == PLAYER_EAST)&&(chkRule("round_continuation", "renchan_never") == 0)) {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "連荘"
		} else {
			setCall getRound(GameStat)\NUM_OF_PLAYERS, "親流れ"
		}
		snd_play SND_PAGE
		redrscreen
		redraw 1
		
		if ((playerwind(getCurrentPlayer(GameStat, CURRENTPLAYER_AGARI), getRound(GameStat)) == PLAYER_EAST)&&(chkRule("round_continuation", "renchan_never") == 0)) {
			incHonba GameStat
		} else {
			setHonba GameStat, 0: incRound GameStat
		}
		setDeposit GameStat, 0
		// 八連荘成立時、カウンタをリセット
		if (getAgariChain(GameStat) == 8) {
			setAgariChain GameStat, 0
		}
#endif
		break;
	/**************/
	/* 錯和発生時 */
	/**************/
	case Chonbo: /* 未実装 */
		/* TODO: これ setAgariChain GameStat, 0: setLastAgariPlayer GameStat, -1 */
		break;
	/**************/
	/* 九種流局時 */
	/**************/
	case KyuushuKyuuhai:
		if      (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sEast ) % PLAYERS))
			ResultDesc = _T("東家の九種九牌");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sSouth) % PLAYERS))
			ResultDesc = _T("南家の九種九牌");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sWest ) % PLAYERS))
			ResultDesc = _T("西家の九種九牌");
		else if (gameStat->CurrentPlayer.Active == ((gameStat->GameRound + sNorth) % PLAYERS))
			ResultDesc = _T("北家の九種九牌");
		else ResultDesc = _T("九種九牌"); // ←決して実行されないはず
		writeChat(ResultDesc);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u);

		if (RuleData::chkRule("nine_terminals", "next_dealer") == 0)
			RenchanFlag = ((!RuleData::chkRule("nine_terminals", "renchan_if_dealer_kyuushu")) || (gameStat->CurrentPlayer.Active == (gameStat->GameRound % PLAYERS)));
		else
			RenchanFlag = false;
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* 四槓流局時 */
	/**************/
	case Suukaikan:
		/* TODO: これの是非を確認 statmes "流局(四開槓)" */
		ResultDesc = _T("四開槓");
		ryuukyokuScreen(sound::IDs::voxSikang, &ResultDesc, mihajong_graphic::tblSubsceneSikang);
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_kong_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* 三家和の時 */
	/**************/
	case TripleRon:
		Sleep(1300);
		/* TODO: こいつらの処分を決めてください
		statmes "流局(二家和)"
		statmes "流局(三家和)"
		*/
		ResultDesc = chkGameType(gameStat, AllSanma) ? _T("二家和") : _T("三家和");
		ryuukyokuScreen(sound::IDs::voxSanjiahu, &ResultDesc, mihajong_graphic::tblSubsceneTripleRon);

		if (RuleData::chkRule("triple_mahjong", "renchan_if_nondealer_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi != (gameStat->GameRound % PLAYERS));
		else if (RuleData::chkRule("triple_mahjong", "renchan_if_north_furikomi") || RuleData::chkRule("triple_mahjong", "renchan_if_west_furikomi"))
			RenchanFlag = (gameStat->CurrentPlayer.Furikomi == ((gameStat->GameRound + (chkGameType(gameStat, AllSanma) ? sWest : sNorth)) % PLAYERS));
		else if (RuleData::chkRule("triple_mahjong", "same_dealer"))
			RenchanFlag = true;
		else if (RuleData::chkRule("triple_mahjong", "next_dealer"))
			RenchanFlag = false;
		ryuukyokuProc(gameStat, RenchanFlag);
		break;
	/**************/
	/* 四風流局時 */
	/**************/
	case SuufonRenda:
		/* TODO: こいつらの進退を決めてあげてください
		statmes "流局(三風連打)"
		statmes "流局(四風連打)"
		*/
		ResultDesc = chkGameType(gameStat, AllSanma) ? _T("三風連打") : _T("四風連打");
		ryuukyokuScreen(sound::IDs::voxSifeng, &ResultDesc, mihajong_graphic::tblSubsceneSifeng);
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_wind_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* 四人立直時 */
	/**************/
	case SuuchaRiichi:
		/* TODO: これの処分がまだ保留中
		statmes "流局(三家立直)"
		statmes "流局(四家立直)"
		*/
		ResultDesc = chkGameType(gameStat, AllSanma) ? _T("三家立直") : _T("四家立直");
		ryuukyokuScreen(sound::IDs::voxSifeng, &ResultDesc, mihajong_graphic::tblSubsceneFourRiichi, 1500u);
		checkTenpai(gameStat, ResultDesc, OrigTurn);
		for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
			if (chkGameType(gameStat, Sanma4) && (playerwind(gameStat, cnt, gameStat->GameRound) == sNorth))
				continue;
			// 錯和立直（不聴立直）の者がいた場合
			if (!isTenpai(gameStat, cnt)) {
				transferChonboPenalty(gameStat, cnt);
				Sleep(500);
			}
		}
		ryuukyokuProc(gameStat, !RuleData::chkRule("four_riichi_ryuukyoku", "next_dealer"));
		break;
	/**************/
	/* 流し満貫時 */
	/**************/
	case NagashiMangan: /* 未実装 */
		/* TODO: 本当にいらないか確認 statmes "流局です" */
		const bool agariBgmSet = !(NagashiManganFlag[gameStat->PlayerID] || EnvTable::Instantiate()->WatchModeFlag); // 自分の流し満貫ならtrue
		const unsigned bgmNum =
			RuleData::chkRule("nagashi_mangan", "yakuman") ? (agariBgmSet ? sound::IDs::musAgariSelf3 : sound::IDs::musAgariFurikomi3)
			: (agariBgmSet ? sound::IDs::musAgariSelf2 : sound::IDs::musAgariFurikomi2);
		ryuukyokuScreen(0u, nullptr, 0u, 1500u, bgmNum);
		/* TODO: これは本当にいらないのかどうか statmes "流し満貫が成立しました" */
		transfer::resetDelta();
		{
			CodeConv::tstring ResultDesc(_T(""));
			for (PLAYER_ID cnt = 0; cnt < ACTUAL_PLAYERS; ++cnt) {
				if (isNagashiMangan(gameStat, cnt)) {
					mihajong_graphic::calltext::setCall(cnt, mihajong_graphic::calltext::NagashiMangan);
					if (!ResultDesc.empty()) ResultDesc += _T("、");
					switch (playerwind(gameStat, cnt, gameStat->GameRound)) {
						case sEast:  ResultDesc += _T("東家"); break;
						case sSouth: ResultDesc += _T("南家"); break;
						case sWest:  ResultDesc += _T("西家"); break;
						case sNorth: ResultDesc += _T("北家"); break;
					}
					LNum AgariPointRaw = 0;
					if      (RuleData::chkRule("nagashi_mangan", "mangan"))  AgariPointRaw = 2000;
					else if (RuleData::chkRule("nagashi_mangan", "haneman")) AgariPointRaw = 3000;
					else if (RuleData::chkRule("nagashi_mangan", "baiman"))  AgariPointRaw = 4000;
					else if (RuleData::chkRule("nagashi_mangan", "yakuman")) AgariPointRaw = 8000;
					LNum agariPoint; // ダミー。この変数は使用しない。
					agari::calcAgariPoints(gameStat, agariPoint, AgariPointRaw, transfer::getDelta(), cnt);
				}
			}
			ResultDesc += _T("の流し満貫");
			writeChat(ResultDesc);
		}
		sound::Play(sound::IDs::sndPage);
		mihajong_graphic::Subscene(mihajong_graphic::tblSubsceneCall);
		Sleep(1500);
		transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValNagashiMangan, 1500);
		ryuukyokuProc(gameStat, true);
		break;
	/**************/
	/* 四槓流局時 */
	/**************/
	case Uukaikan:
		/* TODO: こいつをリストラするかどうか決めること statmes "流局(四開槓)" */
		ResultDesc = _T("四開槓(５回目の槓での流局)");
		ryuukyokuScreen(sound::IDs::voxSikang, &ResultDesc, mihajong_graphic::tblSubsceneSikang);
		ryuukyokuProc(gameStat, !RuleData::chkRule("fifth_kong", "next_dealer"));
		break;
	}
	// TODO: これ haifuwritebuffer GameStat, GameEnv, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag, ResultDesc, RoundEndType
	return;
}

// -------------------------------------------------------------------------

void endround::transferChonboPenalty(GameTable* gameStat, PLAYER_ID targetPlayer) {
	transfer::resetDelta();
	LNum AgariPoint = 0, AgariPointRaw = 2000;
	agari::calcAgariPoints(gameStat, AgariPoint, AgariPointRaw, transfer::getDelta(), targetPlayer);
	transfer::negateDelta();
	/* なぜわざわざ一旦プラスで求めて符号を反転するという回りくどいことをしているのかというと
	   点パネの計算時に天井函数(数値として大きい方に丸める)的な処理を行っているため、
	   引数をマイナスで与えると(特に三麻で丸取り設定にしてるときとか)チョンボ料が減る虞があるからです */
	transfer::transferPoints(gameStat, mihajong_graphic::tblSubsceneCallValChonboBappu, 1500);
	return;
}

// -------------------------------------------------------------------------

bool endround::nextRound(GameTable* gameStat, EndType RoundEndType, unsigned int OrigTurn) { // 次の局へ(終了する場合はtrue)
	// ハコ割れ終了
	if (RuleData::chkRuleApplied("buttobi_border"))
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (isDobon(gameStat, i)) return true;
	// 天辺終了
	if (RuleData::chkRuleApplied("teppen"))
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (isTeppen(gameStat, i)) return true;
	// 和了り止め
	if (RuleData::chkRuleApplied("agariyame")) {
		if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((OrigTurn + gameStat->LoopRound * roundLoopRate()) == gameStat->GameLength) &&
			((RoundEndType == Agari) || (RuleData::chkRule("agariyame", "yes_also_ready")))) {
				PlayerRankList Rank = calcRank(gameStat);
				if ((Rank[gameStat->GameRound % PLAYERS] == 1) &&
					(gameStat->Player[gameStat->GameRound % PLAYERS].PlayerScore >= (LNum)BasePoint()))
					return true;
		}
	}
	// 三麻の場合
	if (chkGameType(gameStat, SanmaT) &&
		((gameStat->GameRound % PLAYERS) == 3))
		++(gameStat->GameRound);
	// 南入した場合……
	if (gameStat->GameRound == 4) {
		if (RuleData::chkRule("game_length", "east_north_game")) // 東北廻しのとき
			gameStat->GameRound = 12;
		else if (RuleData::chkRule("game_length", "east_west_game")) // 東西廻しのとき
			gameStat->GameRound = 8;
		else if (RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game")) // 東々廻しのとき
			gameStat->GameRound = 16;
	}
	// 通常の半荘終了時（トップが３００００点未満だと西入サドンデス）
	if ((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) > gameStat->GameLength) {
		bool flag = false;
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].PlayerScore >= (LNum)BasePoint())
				return true;
		// 延長戦なし設定
		if (RuleData::chkRule("sudden_death_type", "no")) return true;
	}
	// 延長戦の長さに制限がある場合
	if (RuleData::chkRule("sudden_death_type", "one_extra_round")) {
		if ((gameStat->GameRound == 16) && // 東々廻しのとき
			(RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game"))) {
				if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) >= (gameStat->GameLength + roundLoopRate())))
					return true;
		} else { // それ以外のとき
			if (((gameStat->GameRound + gameStat->LoopRound * roundLoopRate()) >= (gameStat->GameLength + 5)))
				return true;
		}
	}
	// 北場終了の場合は帰り東へ
	if (gameStat->GameRound == roundLoopRate()) {
		if (RuleData::chkRule("sudden_death_type", "no")) { // 延長戦無しで終了
			return true;
		} else { // 返り東
			++(gameStat->LoopRound); gameStat->GameRound = 0;
		}
	}
	// 焼き鳥復活ルールの場合
	if (RuleData::chkRuleApplied("yakitori") && RuleData::chkRuleApplied("yakitori_again")) {
		bool flag = true;
		for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
			if (gameStat->Player[i].YakitoriFlag) flag = false;
		if (flag)
			for (PLAYER_ID i = 0; i < (chkGameType(gameStat, SanmaT) ? 3 : 4); ++i)
				gameStat->Player[i].YakitoriFlag = true;
	}
	return false;
}

// -------------------------------------------------------------------------

namespace {
	InfoByPlayer<LNum> delta;

	std::tuple<bool, signed short> checkExponent(PLAYER_ID player, unsigned group, unsigned digit) {
		if (((LargeNum)delta[player]).digitGroup[group] / (int)std::pow(10.0, (int)digit) != 0) {
			if (digit == 0) {
				assert(group != 0);
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] % 10) * 100 + ((LargeNum)delta[player]).digitGroup[group - 1] / 1000000);
			} else if (digit == 1) {
				assert(group != 0);
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] % 100) * 10 + ((LargeNum)delta[player]).digitGroup[group - 1] / 10000000);
			} else {
				return std::make_tuple(true,
					(((LargeNum)delta[player]).digitGroup[group] / ((int)std::pow(10.0, (int)digit - 2))) % 1000);
			}
		}
		else if ((group == 0) && (digit == 2))
			return std::make_tuple(true, ((LargeNum)delta[player]).digitGroup[0]);
		else return std::make_tuple(false, 0);
	}

	void setTransferParam() {
		bool finishFlag = false; signed short mantissa = 0;
		for (PLAYER_ID i = 0; i < PLAYERS; ++i) {
			mihajong_graphic::callvalue::CallValue callVal = {0, 0u};
			for (int j = DIGIT_GROUPS - 1; j >= 0; --j) {
				for (int k = (j == DIGIT_GROUPS - 1) ? 9 : 7; k >= 0; --k) {
					std::tie(finishFlag, mantissa) = checkExponent(i, j, k);
					if (finishFlag) {
						callVal.Exponent = j * 8 + k;
						callVal.Mantissa = mantissa;
						break;
					}
				}
				if (finishFlag) break;
			}
			if (mantissa != 0)
				mihajong_graphic::callvalue::setVal(i, callVal.Mantissa, callVal.Exponent);
			else
				mihajong_graphic::callvalue::setVal(i, 0, 0u);
		}
	}
}

InfoByPlayer<LNum>& endround::transfer::getDelta() {
	return delta;
}
void endround::transfer::resetDelta() {
	for (PLAYER_ID i = 0; i < PLAYERS; ++i)
		delta[i] = 0;
}
void endround::transfer::addDelta(PLAYER_ID player, const LNum& deltaVal) {
	delta[player] += deltaVal;
}
void endround::transfer::negateDelta() {
	for (PLAYER_ID i = 0; i < PLAYERS; ++i)
		delta[i] *= -1;
}
void endround::transfer::transferPoints(GameTable* gameStat, unsigned subscene, unsigned wait) {
	setTransferParam();
	mihajong_graphic::Subscene(subscene);
	Sleep(wait);
	for (PLAYER_ID i = 0; i < PLAYERS; ++i)
		gameStat->Player[i].PlayerScore += delta[i];
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
}
