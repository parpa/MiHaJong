#include "table.h"
#include "../resource.h"
#include "../rule.h"
#include "../sprite.h"
#include <iomanip>
#include "../../sound/sound.h"
#include "../../common/bgmid.h"
#include "../event.h"
#include "../../common/discard.h"
#include "../utils.h"
#include <functional>

#include "table/yamahai.h"
#include "table/tehai.h"
#include "table/nakihai.h"
#include "table/sutehai.h"
#include "table/showdice.h"
#include "table/richibou.h"
#include "table/chicha.h"
#include "table/nakibtn.h"
#include "table/gari.h"

#include "table/naki_id.h"

namespace mihajong_graphic {
	
GameTableScreen::GameTableScreen(ScreenManipulator* const manipulator) : TableProtoScene(manipulator) {
	LoadTexture(&tBorder, MAKEINTRESOURCE(IDB_PNG_TBLBORDER), 1080, 1080);
	LoadTexture(&tBaize, MAKEINTRESOURCE(IDB_PNG_TBLBAIZE), 1080, 1080);
	buttonReconst = new ButtonReconst(this);
	trayReconst = new TrayReconst(this);
	richibouReconst = new RichibouReconst(this);
	diceReconst = new DiceReconst(this);
	yamahaiReconst = new YamahaiReconst(this);
	tehaiReconst = new TehaiReconst(this);
	nakihaiReconst = new NakihaiReconst(this);
	sutehaiReconst = new SutehaiReconst(this);
	gariReconst = new GariReconst(this);
	Reconstruct(GameStatus::retrGameStat());
	const unsigned logWidth = (unsigned)std::floor(0.5f + // VC++2010ではround()が使えない
		(float)(((signed)Geometry::WindowWidth - (signed)Geometry::WindowHeight) / Geometry::WindowScale() - 36)) / 9u;
	logWindow = new logwnd::LogWindow(caller->getHWnd(), caller->getDevice(),
		1100, 100, logWidth, 20);
	mySubScene = new TableSubsceneNormal(manipulator->getDevice());
	myTextRenderer = new TextRenderer(manipulator->getDevice());
	tileSelectMode = 0;
}

GameTableScreen::~GameTableScreen() {
	delete mySubScene;
	delete logWindow;
	delete gariReconst;
	delete sutehaiReconst;
	delete nakihaiReconst;
	delete tehaiReconst;
	delete yamahaiReconst;
	delete diceReconst;
	delete richibouReconst;
	delete trayReconst;
	delete buttonReconst;
	delete myTextRenderer;
	if (tBorder) tBorder->Release();
	if (tBaize) tBaize->Release();
}

void GameTableScreen::ReconstructPlayer(const GameTable* gameStat, PlayerID targetPlayer, PlayerID trueTargetPlayer) {
	yamahaiReconst->Reconstruct(gameStat, targetPlayer, trueTargetPlayer);
	tehaiReconst->Reconstruct(gameStat, targetPlayer);
	nakihaiReconst->Reconstruct(gameStat, targetPlayer);
	sutehaiReconst->Reconstruct(gameStat, targetPlayer);
}

void GameTableScreen::Reconstruct(const GameTable* gameStat) {
	if (gameStat->gameType & Yonma) {
		for (PlayerID i = 0; i < 4; i++)
			ReconstructPlayer(gameStat, i, i);
	} else if (gameStat->gameType & Sanma4) {
		PlayerID tobePlayed[4][4] = {
			{0, 1, 2, 3}, {3, 1, 2, 0}, {1, 3, 2, 0}, {1, 2, 3, 0},
		};
		for (PlayerID i = 0; i < 4; i++) {
			PlayerID j(tobePlayed[gameStat->GameRound % Players][i]);
			if (j < 3) ReconstructPlayer(gameStat, j, i);
		}
	} else {
		for (PlayerID i = 0; i < 3; i++)
			ReconstructPlayer(gameStat, i, i);
	}
}

/* 供託点棒などの情報を表示 */
void GameTableScreen::ShowStatus(const GameTable* gameStat) {
	const wchar_t* const WindName = L"東南西北白發中";
	const wchar_t* const Numeral = L"一二三四五六七八九十";
	const wchar_t* const FWDigit = L"０１２３４５６７８９";
	CodeConv::tostringstream o;
	o << CodeConv::EnsureTStr(std::wstring(WindName + gameStat->GameRound / 4, WindName + gameStat->GameRound / 4 + 1));
	if (rules::chkRule("game_length", "twice_east_game") || rules::chkRule("game_length", "east_only_game")) { // 東場のみのルール
		const unsigned roundnum = gameStat->LoopRound * 4 + gameStat->GameRound;
		if (roundnum < 10)
			o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
		else
			o << (roundnum + 1);
	} else { // 一般のルール
		const unsigned roundnum = gameStat->GameRound % 4;
		o << CodeConv::EnsureTStr(std::wstring(Numeral + roundnum, Numeral + roundnum + 1));
	}
	o << _T("局 ");
	if (gameStat->Honba >= 10) o << std::setw(2) << (gameStat->Honba) << _T("本場");
	else if (gameStat->Honba > 0) o << CodeConv::EnsureTStr(std::wstring(FWDigit + gameStat->Honba, FWDigit + gameStat->Honba + 1)) << _T("本場");
	else o << _T("平　場");
	myTextRenderer->NewText(0, o.str(), Geometry::BaseSize + 5, 5, 0.875f);

	o.str(_T(""));
	const int tiles = gameStat->RinshanPointer - gameStat->TilePointer - gameStat->ExtraRinshan - gameStat->DeadTiles + 1;
	if (tiles >= 100) o << _T("残--牌");
	else o << _T("残") << std::setw(2) << tiles << _T("牌");
	o << _T(" 供託");
	if (gameStat->Deposit) o << std::setw(2) << gameStat->Deposit << _T("本");
	else o << _T("なし");
	myTextRenderer->NewText(1, o.str(), Geometry::BaseSize + 5, 35, 0.875f);

	myTextRenderer->Render();
}

/* 卓を表示 ここから */
void GameTableScreen::cls() {
	caller->getDevice()->Clear(0, nullptr, D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 128, 0), 1.0f, 0); // バッファクリア
}

void GameTableScreen::RenderTable() {
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBaize, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	SpriteRenderer::instantiate(caller->getDevice())->ShowSprite(tBorder, 0, 0, Geometry::BaseSize, Geometry::BaseSize);
	if (GameStatus::isModified())
		Reconstruct(GameStatus::retrGameStat());
	richibouReconst->Render();
	diceReconst->Render();
	trayReconst->Render();
	yamahaiReconst->Render(); // 0
	tehaiReconst->Render(); // 144
	nakihaiReconst->Render(); // 200
	sutehaiReconst->Render(); // 264
	gariReconst->Render();
	buttonReconst->Render();
}

void GameTableScreen::RenderSideBar() {
	ShowSidebar();
	ShowStatus(GameStatus::gameStat());
	ShowScorePanel();
	logWindow->Render();
}

void GameTableScreen::checkTimeout() {
	if ((mySubScene) && (mySubScene->timeout() <= 0) && (buttonReconst->areEnabled().any() || tehaiReconst->isCursorEnabled())) { /* タイムアウトの処理 */
		const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
		if (isNakiSel) { // 鳴き選択中の時
			ui::UIEvent->set(naki::nakiNone); // 牌の番号を設定
		} else if (buttonReconst->getButtonSet() == ButtonReconst::btnSetTsumo) {
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Tsumohai().tile != NoTile)
				ui::UIEvent->set(NumOfTilesInHand - 1); // ツモ切り
			else ui::UIEvent->set(0); // 鳴いた直後の場合
		}
		tehaiReconst->setTileCursor();
		buttonReconst->setCursor();
	}
}

void GameTableScreen::Render() {
	cls();
	RenderTable();
	subSceneCS.trySyncDo<void>(nullptr, [this]() -> void {
		checkTimeout();
		mySubScene->Render();
	});
	RenderSideBar();
}

void GameTableScreen::SetSubscene(unsigned int scene_ID) {
	subSceneCS.syncDo<void>([this, scene_ID]() -> void {
		buttonReconst->ChangeButtonSet(GameTableScreen::ButtonReconst::btnSetNormal);
		tehaiReconst->enable();
		tileSelectMode = 0;
		delete mySubScene; tehaiReconst->setTileCursor();
		switch (static_cast<TableSubsceneID>(scene_ID)) {
		case tblSubsceneBeginning:
			mySubScene = new TableSubsceneBeginning(caller->getDevice());
			break;
		case tblSubsceneHonba:
			mySubScene = new TableSubsceneTitlingHonba(caller->getDevice());
			break;
		case tblSubsceneRyuukyoku:
			mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("流局"));
			break;
		case tblSubsceneSifeng:
			mySubScene = new TableSubsceneMsg(caller->getDevice(),
				(GameStatus::gameStat()->gameType & AllSanma) ? _T("三風連打") : _T("四風連打"));
			break;
		case tblSubsceneTripleRon:
			mySubScene = new TableSubsceneMsg(caller->getDevice(),
				(GameStatus::gameStat()->gameType & AllSanma) ? _T("二家和") : _T("三家和"));
			break;
		case tblSubsceneSikang:
			mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("四開槓"));
			break;
		case tblSubsceneFourRiichi:
			mySubScene = new TableSubsceneMsg(caller->getDevice(),
				(GameStatus::gameStat()->gameType & AllSanma) ? _T("三家立直") : _T("四家立直"));
			break;
		case tblSubsceneChonbo:
			mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("錯和"));
			break;
		case tblSubsceneAlice:
			mySubScene = new TableSubsceneMsg(caller->getDevice(), _T("アリス判定"));
			break;
		case tblSubsceneCall:
			mySubScene = new TableSubsceneCall(caller->getDevice());
			break;
		case tblSubsceneCallFade:
			mySubScene = new TableSubsceneCallFade(caller->getDevice());
			break;
		case tblSubsceneCallCut:
			mySubScene = new TableSubsceneCallCut(caller->getDevice());
			break;
		case tblSubsceneCallChankanPre:
			mySubScene = new TableSubsceneCallChankanPre(caller->getDevice());
			break;
		case tblSubsceneCallChankan:
			mySubScene = new TableSubsceneCallChankanRon(caller->getDevice());
			break;
		case tblSubsceneCallVal:
			mySubScene = new TableSubsceneCallValue(caller->getDevice());
			break;
		case tblSubsceneCallValNotenBappu:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("不聴罰符"));
			break;
		case tblSubsceneCallValAgariten:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("和了点"));
			break;
		case tblSubsceneCallValTsumibou:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("積棒精算"));
			break;
		case tblSubsceneCallValChip:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("祝儀精算"));
			break;
		case tblSubsceneCallValKyoutaku:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("供託精算"));
			break;
		case tblSubsceneCallValChonboBappu:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("錯和罰符"));
			break;
		case tblSubsceneCallValNagashiMangan:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("流し満貫"));
			break;
		case tblSubsceneCallValDobon:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("飛び罰符"));
			break;
		case tblSubsceneCallValKitamakura:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("北枕罰符"));
			break;
		case tblSubsceneCallValYakuman:
			mySubScene = new TableSubsceneCallValue(caller->getDevice(), _T("役満祝儀"));
			break;
		case tblSubsceneChkTenpai:
			mySubScene = new TableSubsceneCheckTenpai(caller->getDevice());
			break;
		case tblSubscenePlayerDahai:
			mySubScene = new TableSubscenePlayerDahai(caller->getDevice());
			(void)GameStatus::retrGameStat();
			// カーソルとボタンの設定
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			while (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Hand[tehaiReconst->getTileCursor()].tile == NoTile)
				tehaiReconst->decrTileCursor(); // 鳴いた直後の時のカーソル初期位置
			buttonReconst->btnSetForDahai();
			tehaiReconst->enable();
			if (GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag)
				for (int i = 0; i < (NumOfTilesInHand - 1); ++i)
					tehaiReconst->disable(i);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			// リーチ後オートツモ切り
			if ((GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].RichiFlag.RichiFlag) &&
				buttonReconst->areEnabled().none())
				ui::UIEvent->set(NumOfTilesInHand - 1);
			else // 自摸番が来たら音を鳴らす
				sound::Play(sound::IDs::sndBell);
			break;
		case tblSubscenePlayerNaki:
			mySubScene = new TableSubscenePlayerNaki(caller->getDevice());
			goto setNakiButton;
		case tblSubscenePlayerChankan:
			mySubScene = new TableSubscenePlayerNakiChankan(caller->getDevice());
			goto setNakiButton;
		setNakiButton:
			// カーソルとボタンの設定
			buttonReconst->btnSetForNaki();
			buttonReconst->setCursor(buttonReconst->isEnabled(GameTableScreen::ButtonReconst::btnRon) ? GameTableScreen::ButtonReconst::btnRon : GameTableScreen::ButtonReconst::btnPass);
			buttonReconst->reconstruct();
			if (buttonReconst->areEnabled().none())
				ui::UIEvent->set(naki::nakiNone);
			else // 音を鳴らす
				sound::Play(sound::IDs::sndSignal);
			break;
		case tblSubsceneAgari:
			mySubScene = new TableSubsceneAgariScreen(caller->getDevice());
			break;
		case tblSubsceneAgariUradora:
			mySubScene = new TableSubsceneAgariScreenUradora(caller->getDevice());
			break;
		default:
			mySubScene = new TableSubsceneNormal(caller->getDevice());
			break;
		}
	});
}

// -------------------------------------------------------------------------

void GameTableScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
	auto cursorMoved = [&]() -> void {
		sound::Play(sound::IDs::sndCursor);
		tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
		buttonReconst->reconstruct();
	};
	const PlayerTable* const plDat = &(GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID]);
	auto directTileCursor = [&](int cursorPos) -> void {
		if ((od->dwData) && ((tehaiReconst->isCursorEnabled()) || (buttonReconst->isCursorEnabled())) && (plDat->Hand[cursorPos].tile != NoTile)) {
			if (tehaiReconst->getTileCursor() == cursorPos) {
				FinishTileChoice();
			} else {
				buttonReconst->setCursor();
				tehaiReconst->setTileCursor(cursorPos);
				cursorMoved();
			}
		}
	};
	switch (od->dwOfs) {
	/* ボタン選択/牌選択 モード切り替え */
	case DIK_UP: case DIK_K: // 牌選択モードに切り替え
		if ((od->dwData) && (buttonReconst->isCursorEnabled()) && (!isNakiSel)) {
			tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			buttonReconst->setCursor();
			cursorMoved();
		}
		break;
	case DIK_DOWN: case DIK_J: // ボタン選択モードに切り替え
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	/* カーソル移動 */
	case DIK_LEFT: case DIK_H:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->decrTileCursor() < 0) tehaiReconst->setTileCursor(NumOfTilesInHand - 1);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		else if ((od->dwData) && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->decCursor() < 0) buttonReconst->setCursor(ButtonReconst::btnMAXIMUM - 1);
			cursorMoved();
		}
		break;
	case DIK_RIGHT: case DIK_L:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled())) {
			do {
				if (tehaiReconst->incrTileCursor() >= NumOfTilesInHand) tehaiReconst->setTileCursor(0);
			} while (plDat->Hand[tehaiReconst->getTileCursor()].tile == NoTile);
			cursorMoved();
		}
		if ((od->dwData) && (buttonReconst->isCursorEnabled())) {
			if (buttonReconst->incCursor() >= ButtonReconst::btnMAXIMUM) buttonReconst->setCursor(0);
			cursorMoved();
		}
		break;
	/* カーソル位置の直截指定 */
	/* ASSUMING JAPANESE KEYBOARD: 1 2 3 4 5 6 7 8 9 0 - ^ ￥ BS */
	case DIK_1:          directTileCursor( 0); break;
	case DIK_2:          directTileCursor( 1); break;
	case DIK_3:          directTileCursor( 2); break;
	case DIK_4:          directTileCursor( 3); break;
	case DIK_5:          directTileCursor( 4); break;
	case DIK_6:          directTileCursor( 5); break;
	case DIK_7:          directTileCursor( 6); break;
	case DIK_8:          directTileCursor( 7); break;
	case DIK_9:          directTileCursor( 8); break;
	case DIK_0:          directTileCursor( 9); break;
	case DIK_MINUS:      directTileCursor(10); break;
	case DIK_CIRCUMFLEX: directTileCursor(11); break;
	case DIK_YEN:        directTileCursor(12); break;
	case DIK_BACK:       directTileCursor(13); break;
	/* 決定キー */
	case DIK_RETURN: case DIK_SPACE: case DIK_Z:
		if ((od->dwData) && (tehaiReconst->isCursorEnabled()))
			FinishTileChoice();
		else if ((od->dwData) && (buttonReconst->isCursorEnabled()))
			buttonReconst->ButtonPressed();
		break;
	}
}

void GameTableScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const bool isNakiSel = (buttonReconst->getButtonSet() == ButtonReconst::btnSetNormal) && buttonReconst->areEnabled().any();
	const int scaledX = X / Geometry::WindowScale() * (Geometry::WindowWidth * 0.75f / Geometry::WindowHeight);
	const int scaledY = Y / Geometry::WindowScale();
	const int region = whichRegion(scaledX, scaledY);
	const bool isCursorEnabled = tehaiReconst->isCursorEnabled() || buttonReconst->isCursorEnabled();
	const bool isValidTile = (region >= 0) && (region < NumOfTilesInHand) &&
		isCursorEnabled && (!isNakiSel) &&
		(GameStatus::gameStat()->Player[GameStatus::gameStat()->PlayerID].Hand[region].tile != NoTile);
	const bool isButton = (region >= ButtonReconst::ButtonRegionNum) &&
		(region < ButtonReconst::ButtonRegionNum + ButtonReconst::btnMAXIMUM) &&
		isCursorEnabled;
	switch (od->dwOfs) {
	case DIMOFS_X: case DIMOFS_Y: // マウスカーソルを動かした場合
		if ((region != tehaiReconst->getTileCursor()) && (isValidTile)) {
			tehaiReconst->setTileCursor(region);
			buttonReconst->setCursor();
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
		} else if ((region != (ButtonReconst::ButtonRegionNum + buttonReconst->getCursor())) && (isButton)) {
			tehaiReconst->setTileCursor();
			buttonReconst->setCursor(region - ButtonReconst::ButtonRegionNum);
			sound::Play(sound::IDs::sndCursor);
			tehaiReconst->Reconstruct(GameStatus::gameStat(), GameStatus::gameStat()->PlayerID);
			buttonReconst->reconstruct();
		}
		break;
	case DIMOFS_BUTTON0: // マウスクリック
		if ((isValidTile) && (od->dwData))
			FinishTileChoice();
		else if ((isButton) && (od->dwData))
			buttonReconst->ButtonPressed();
		break;
	case DIMOFS_BUTTON1: // マウス右クリック
		if ((od->dwData) && isNakiSel) { // 鳴き選択中の時
			sound::Play(sound::IDs::sndClick);
			ui::UIEvent->set(naki::nakiNone); // 牌の番号を設定
		}
		break;
	}
}

/* 捨牌を決定する */
void GameTableScreen::FinishTileChoice() {
	sound::Play(sound::IDs::sndClick);
	if (tehaiReconst->isCursorEnabled() && tehaiReconst->isEnabled(tehaiReconst->getTileCursor())) {
		const Int8ByTile TileCount = utils::countTilesInHand(GameStatus::gameStat(), GameStatus::gameStat()->CurrentPlayer.Active);
		if ((tileSelectMode == DiscardTileNum::Ankan) && (TileCount[GameStatus::gameStat()->statOfActive().Hand[tehaiReconst->getTileCursor()].tile] == 1))
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(DiscardTileNum::Kakan * DiscardTileNum::TypeStep)); // 加槓の場合
		else
			ui::UIEvent->set((unsigned)tehaiReconst->getTileCursor() + (unsigned)(tileSelectMode * DiscardTileNum::TypeStep)); // 牌の番号を設定
	} else {
		sound::Play(sound::IDs::sndCuohu);
	}
}

void GameTableScreen::CallTsumoAgari() { // ツモアガリ
	ui::UIEvent->set(0xffffffff);
}
void GameTableScreen::CallKyuushuKyuuhai() { // 九種九牌
	ui::UIEvent->set(0xfffffffe);
}


}
