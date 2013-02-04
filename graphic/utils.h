#pragma once

#include "exports.h"
#include "../mjcore/func.h"
#include "../mjcore/tileutil.h"
#include "../mjcore/shanten.h"

namespace mihajong_graphic {
namespace utils {

typedef seatAbsolute (*playerwind_type)(const GameTable*, PLAYER_ID, int);
typedef seatRelative (*playerRelative_type)(PLAYER_ID, PLAYER_ID);
typedef PLAYER_ID (*RelativePositionOf_type)(PLAYER_ID, seatRelative);
typedef int (*tilesLeft_type)(const GameTable*);
typedef PlayerRankList (*calcRank_type)(const GameTable*);
typedef MachihaiInfo (*chkFuriten_type)(const GameTable*, PLAYER_ID);
typedef bool (*isTenpai_type)(const GameTable*, PLAYER_ID);
typedef bool (*isRichiReqSatisfied_type)(const GameTable*, PLAYER_ID);
typedef SHANTEN (*calcShanten_type)(const GameTable*, PLAYER_ID, ShantenAnalyzer::shantenType);
typedef bool (*chkdaopaiability_type)(const GameTable*, PLAYER_ID);
typedef bool (*chkShisanBuDa_type)(const GameTable*, PLAYER_ID);
typedef bool (*chkShisiBuDa_type)(const GameTable*, PLAYER_ID);

#ifdef GRAPHIC_EXPORTS
extern playerwind_type playerwind;
extern playerRelative_type playerRelative;
extern RelativePositionOf_type RelativePositionOf;
extern tilesLeft_type tilesLeft;
extern calcRank_type calcRank;
extern chkFuriten_type chkFuriten;
extern isTenpai_type isTenpai;
extern isRichiReqSatisfied_type isRichiReqSatisfied;
extern calcShanten_type calcShanten;
extern chkdaopaiability_type chkdaopaiability;
extern chkShisanBuDa_type chkShisanBuDa;
extern chkShisiBuDa_type chkShisiBuDa;
#endif

EXPORT void setfunc(
	playerwind_type ptr_playerwind,
	playerRelative_type ptr_playerRelative,
	RelativePositionOf_type ptr_RelativePositionOf,
	tilesLeft_type ptr_tilesLeft,
	calcRank_type ptr_calcRank,
	chkFuriten_type ptr_chkFuriten,
	isTenpai_type ptr_isTenpai,
	isRichiReqSatisfied_type ptr_isRichiReqSatisfied,
	calcShanten_type ptr_calcShanten,
	chkdaopaiability_type ptr_chkdaopaiability,
	chkShisanBuDa_type ptr_chkShisanBuDa,
	chkShisiBuDa_type ptr_chkShisiBuDa);

}
}
