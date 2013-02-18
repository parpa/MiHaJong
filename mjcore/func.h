#pragma once

#include <cstdint>
#ifdef MJCORE_EXPORTS
#include <type_traits>
#endif
#include "mjexport.h"
#include "gametbl.h"

// -------------------------------------------------------------------------

using mihajong_structs::InfoByPlayer;
using mihajong_structs::PlayerID;
using mihajong_structs::TileCode;

enum seatAbsolute : uint8_t { sEast, sSouth, sWest, sNorth };
enum seatRelative : uint8_t { sSelf, sRight, sOpposite, sLeft };
typedef uint8_t PLAYER_RANK;
template struct InfoByPlayer<PLAYER_RANK>;
typedef InfoByPlayer<PLAYER_RANK> PlayerRankList;
#ifdef MJCORE_EXPORTS
static_assert(std::is_pod<PlayerRankList>::value, "PlayerRankList is not POD");
#endif

// -------------------------------------------------------------------------

#ifdef MJCORE_EXPORTS
extern "C" inline uint8_t diceSum(const GameTable* const gameStat);

seatAbsolute inline playerwind(const GameTable* const gameStat, PlayerID player, int currentRound);
__declspec(dllexport) inline int playerwind(int player, int currentRound);

seatRelative inline playerRelative(PlayerID targetPlayer, PlayerID basePlayer);
__declspec(dllexport) inline int playerRelative(int targetPlayer, int basePlayer);

PlayerID inline RelativePositionOf(PlayerID targetPlayer, seatRelative relative);
__declspec(dllexport) inline int RelativePositionOf(int targetPlayer, int relative);

__declspec(dllexport) inline int roundLoopRate();

__declspec(dllexport) inline int tilesLeft(const GameTable* const gameStat);

PlayerRankList calcRank(const GameTable* const gameStat);
__declspec(dllexport) void calcRank(int* Rank, const GameTable* const gameStat);

bool isPao(const GameTable* const gameStat, PlayerID agariPlayer, PlayerID paoPlayer);
__declspec(dllexport) int isPao(const GameTable* const gameStat, int agariPlayer, int paoPlayer);

bool isPaoAgari(const GameTable* const gameStat, PlayerID agariPlayer);
__declspec(dllexport) int isPaoAgari(const GameTable* const gameStat, int agariPlayer);

bool isGotPao(const GameTable* const gameStat, PlayerID paoPlayer);
__declspec(dllexport) int isGotPao(const GameTable* const gameStat, int paoPlayer);

PlayerID getPaoPlayer(const GameTable* const gameStat, PlayerID agariPlayer);
__declspec(dllexport) int getPaoPlayer(const GameTable* const gameStat, int agariPlayer);

__declspec(dllexport) int RonPlayers(const GameTable* const gameStat);

CodeConv::tstring inline windName(seatAbsolute wind);
__declspec(dllexport) void windName(LPTSTR str, int bufsz, int wind);

CodeConv::tstring inline roundName(int roundNum, const GameTable* const gameStat);
__declspec(dllexport) void roundName(LPTSTR str, int bufsz, int roundNum);

CodeConv::tstring inline TileName(TileCode tile);
__declspec(dllexport) void TileName(LPTSTR str, int bufsz, int tile);

TileCode Wind2Tile(uint8_t wind);

int BasePoint();
__declspec(dllexport) int BasePointHSP();

bool isAboveBase(const GameTable* const gameStat, PlayerID player);
__declspec(dllexport) int isAboveBase(const GameTable* const gameStat, int player);

CodeConv::tstring intstr(int val);

namespace confpath {
	bool isVista();

	std::string confPath();
	__declspec(dllexport) void confPath(char* path, int bufsz);
}

bool isRichiReqSatisfied (const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isRichiReqSatisfied (const GameTable* const gameStat, int targetPlayer);

bool isDobon (const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isDobon (const GameTable* const gameStat, int targetPlayer);

bool isTeppen (const GameTable* const gameStat, PlayerID targetPlayer);
__declspec(dllexport) int isTeppen (const GameTable* const gameStat, int targetPlayer);

inline bool exist (LPCSTR filename) {
	return (GetFileAttributesA(filename) != -1);
}
#endif

MJCORE void cleanup();
