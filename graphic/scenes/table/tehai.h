#pragma once

#include "../table.h"
#include <bitset>

namespace mihajong_graphic {

class GameTableScreen::TehaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
private:
	static const unsigned int HandLength = 13;
	static const unsigned int HandPosH = (TableSize - ShowTile::VertTileWidth * (HandLength - 1)) / 2;
	static const unsigned int HandPosV = DeckPosV - 144;
	static const int tileCursorOff = -3; // 手牌カーソル無効時
	int tileCursor; // 手牌カーソル
	std::bitset<NumOfTilesInHand> tileEnabled;
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // 手牌の再構築
	void Render();
	explicit TehaiReconst(GameTableScreen* parent);
	~TehaiReconst();
public:
	int getTileCursor() {return tileCursor;}
	void setTileCursor(int val = tileCursorOff) {tileCursor = val;}
	bool isCursorEnabled() {return tileCursor != tileCursorOff;}
	int incrTileCursor() {return ++tileCursor;}
	int decrTileCursor() {return --tileCursor;}
public:
	void enable() {tileEnabled.set();}
	void enable(unsigned tileID) {tileEnabled[tileID] = true;}
	void disable() {tileEnabled.reset();}
	void disable(unsigned tileID) {tileEnabled[tileID] = false;}
	bool isEnabled(unsigned tileID) {return tileEnabled[tileID];}
private: // 時間関係
	std::uint64_t currTime();
};

}
