#pragma once

#include "../table.h"

namespace mihajong_graphic {

class GameTableScreen::NakihaiReconst {
private:
	ShowTile* TileTexture;
	GameTableScreen* caller;
private:
	static const unsigned int MeldPosH = TableSize - 31;
	static const unsigned int MeldPosV = DeckPosV - 166;
	static const unsigned int MPosVVert = TableSize - MeldPosV;
	static const unsigned int MPosVHorU = TableSize - MeldPosV - ShowTile::VertTileWidth + 6;
	static const unsigned int MPosVHorL = TableSize - MeldPosV + 6;
	static inline const unsigned int MPosHVertR(unsigned p) {return MeldPosH - ShowTile::VertTileWidth * p;}
	static inline const unsigned int MPosHVertL(unsigned p) {return MeldPosH - ShowTile::VertTileWidth * (p - 1) - ShowTile::HoriTileHeight - 5;}
	static inline const unsigned int MPosHHor(unsigned p) {return MeldPosH - ShowTile::VertTileWidth * (p - 1) - ShowTile::HoriTileHeight + 2;}
	std::tuple<std::function<unsigned (unsigned)>, std::function<int (unsigned)>, std::function<int (unsigned)>, TileDirection, TileDirection, TileDirection>
		playerPosition(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID,
		unsigned h1, unsigned h2, unsigned h3, unsigned h4, unsigned v1, unsigned v2, unsigned v3, unsigned v4,
		bool r1, bool r2, bool r3, bool r4);
	void NakihaiAnkan(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanKamicha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanToimen(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void MinkanShimocha(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
	void NakihaiSelRoutine(const GameTable* gameStat, PlayerID targetPlayer, signed PositionOffset, unsigned IDOffset, unsigned meldID);
public:
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer); // �����v�̍č\�z
	void Render();
	explicit NakihaiReconst(GameTableScreen* parent);
	~NakihaiReconst();
};

}
