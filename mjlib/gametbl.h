#ifndef GAMETBL_H
#define GAMETBL_H

#include <array>
#include <cstdint>
#include <exception>
#include <stdexcept>
#include "tilecode.h"

// �V���[���΍�
// �ꉞ21�s�v�c�܂ŕ\���\�c�c

#define DIGIT_GROUPS 8

class LargeNum {
private:
	std::array<int32_t, DIGIT_GROUPS> digitGroup;
	void fix() { // ���K�`�ɒ���
		for (int i = 0; i < (DIGIT_GROUPS - 1); i++) {
			if ((this->digitGroup[i] > 99999999)||(this->digitGroup[i] < -99999999)) {
				this->digitGroup[i + 1] += this->digitGroup[i] / 100000000;
				this->digitGroup[i] %= 100000000;
			}
		}
	}
public:
	LargeNum() { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
		this->digitGroup.fill(0);
	}
	LargeNum(int val) { // �}21�s�v�c�܂ŕ\���\�Ȑ��̃N���X
		this->digitGroup.fill(0);
		this->digitGroup[0] = (val % 100000000);
		this->digitGroup[1] = (val / 100000000);
	}
	/* �������牉�Z�q���I�[�o�[���[�h */
	const LargeNum operator+(const LargeNum& addend) {
		LargeNum ans;
		for (int i = 0; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i] + addend.digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator+(const int32_t addend) {
		LargeNum ans;
		ans.digitGroup[0] = digitGroup[0] + addend;
		for (int i = 1; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i];
		ans.fix();
		return ans;
	}
	LargeNum& operator+=(const LargeNum& addend) {
		for (int i = 0; i < DIGIT_GROUPS; i++)
			digitGroup[i] += addend.digitGroup[i];
		fix();
		return *this;
	}
	LargeNum& operator+=(const int32_t addend) {
		digitGroup[0] += addend;
		fix();
		return *this;
	}
	const LargeNum operator-(const LargeNum& subtrahend) {
		LargeNum ans;
		for (int i = 0; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i] - subtrahend.digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator-(const int32_t subtrahend) {
		LargeNum ans;
		ans.digitGroup[0] = digitGroup[0] - subtrahend;
		for (int i = 1; i < DIGIT_GROUPS; i++)
			ans.digitGroup[i] = digitGroup[i];
		ans.fix();
		return ans;
	}
	const LargeNum operator*(const int32_t multiplier) { // �߂�ǂ������̂�32bit�����{�����c�c
		LargeNum ans = LargeNum();
		for (int i = 0; i < DIGIT_GROUPS; i++) {
			int64_t tmpdigit = digitGroup[i] * multiplier;
			ans.digitGroup[i] = (int32_t)(tmpdigit % 100000000L);
			if ((i == (DIGIT_GROUPS - 1))
				&& ((tmpdigit > INT_MAX) || (tmpdigit < INT_MIN)))
				throw std::overflow_error("�I�[�o�[�t���[���܂���");
			ans.digitGroup[i + 1] = (int32_t)(tmpdigit / 100000000L);
		}
		ans.fix();
		return ans;
	}
	const int32_t& operator[] (const int i) const { // const �z��A�N�Z�X�c�c
		return digitGroup[i];
	}
	int32_t& operator[] (const int i) { // non-const �z��A�N�Z�X�c�c
		return digitGroup[i];
	}
};

#define PLAYERS 4
#define NUM_OF_TILES_IN_HAND 14
#define SIZE_OF_DISCARD_BUFFER 33

typedef int8_t PLAYER_ID; // �v���C���[�ԍ�
typedef std::array<int, PLAYERS> INT_EACH_PLAYER;

enum handTilePage { tlCode, redTile };

typedef std::array<tileCode, NUM_OF_TILES_IN_HAND> HAND_TILES;
typedef std::array<HAND_TILES, PLAYERS> HAND_EACH_PLAYER;

#define SUTEHAI_TYPE_STEP 200
enum discardStat {
	discardNormal,
	discardTaken,
	discardRiichi,
	discardRiichiTaken,
};
struct discardTile {
	tileCode tcode;
	discardStat dstat;
};
enum discardTilePage { dTileCode, dRedTile, dThrough };

typedef std::array<discardTile, SIZE_OF_DISCARD_BUFFER> DISCARD_BUF;
typedef std::array<DISCARD_BUF, PLAYERS> DISCARD_EACH_PLAYER;

#define SIZE_OF_MELD_BUFFER 5
#define MELD_TYPE_STEP 1000
enum meldTilePage { mTileCode, mRedTile };
enum meldStat {
	meldSequenceConcealed,      // ��̓��̏��q
	meldSequenceExposedLower,   // �����������`�[
	meldSequenceExposedMiddle,  // �ƒ����`�[
	meldSequenceExposedUpper,   // �傫�������`�[
	meldTripletConcealed,       // �Í�
	meldTripletExposedLeft,     // ��Ƃ���|��
	meldTripletExposedCenter,   // �Ζʂ���|��
	meldTripletExposedRight,    // ���Ƃ���|��
	meldQuadConcealed,          // �Þ�
	meldQuadExposedLeft,        // ��Ƃ��疾��
	meldQuadExposedCenter,      // �Ζʂ��疾��
	meldQuadExposedRight,       // ���Ƃ��疾��
	meldQuadAddedLeft,          // ��Ƃ���|���̌�J��
	meldQuadAddedCenter,        // �Ζʂ���|���̌�J��
	meldQuadAddedRight          // ���Ƃ���|���̌�J��
};
struct meldCode {
	tileCode tcode;
	meldStat mstat;
};
typedef std::array<meldCode, SIZE_OF_MELD_BUFFER> MELD_BUF;
typedef std::array<MELD_BUF, PLAYERS> MELD_EACH_PLAYER;

struct RichiStat { // �����t���O���i�[
	bool RichiFlag;
	bool IppatsuFlag;
	bool DoubleFlag;
	bool OpenFlag;
};

struct PlayerTable { // �v���C���[�̏�Ԃ��i�[
	LargeNum PlayerScore;
	int playerChip; // �`�b�v�̎��x
	bool SumaroFlag; // �l�n�H���փt���O
	bool YakitoriFlag; // �Ă����t���O
	std::array<HAND_TILES, 2> Hand; // ��v�̔z��
	std::array<DISCARD_BUF, 3> Discard; // �̔v�̔z��
	uint8_t DiscardPointer;
	std::array<MELD_BUF, 2> Meld; // ���ʎq���i�[
	uint8_t MeldPointer;
	bool MenzenFlag; // ��O�t���O
	int MenzenFlagAb;
	int8_t HandStat; // ��v�̏�ԁi���Ă�E������E������j
	int8_t NumberOfQuads; // �Ȏq�̐��i�l�ȗ��ǁA�O�Ȏq�A�l�Ȏq�Ȃǂ̔���Ɏg���j
	RichiStat RichiFlag; // ���[�`���Ă��邩�ǂ���
};

class GameTable { // ��̏����i�[���邽�߂̃N���X
public:
	std::array<PlayerTable, PLAYERS> Player;
	PLAYER_ID PlayerID;
	int GameLength;
	int GameRound;
	int LoopRound;
	int Honba;
	int TurnRound;
	int Deposit;
	int AgariChain;
	int LastAgariPlayer;
	int RichiFlag;
	int OpenRichiWait;
	int FirstDrawFlag;
	int DoujunFuriten;
	int AgariHouki;
	int FlowerFlag;
	int NorthFlag;
	int KangFlag;
	int KangNum;
	int RichiCounter;
	int WaremePlayer;
	int DoukasenPlayer;
	int Dice1;
	int Dice2;
	int Dice1Direction;
	int Dice2Direction;
	int PaoFlag;
	int Deck;
	int DeadTiles;
	int ExtraRinshan;
	int ShibariFlag;
	int DoraFlag;
	int TilePointer;
	int DoraPointer;
	int RinshanPointer;
	int TianHuFlag;
	int PreviousMeld;
	int ConnectionLost;
	int CurrentPlayer;
	int DeclarationFlag;
	int TsumoAgariFlag;
	int CurrentDiscard;
	// Constructor
	GameTable() {
	}
};

#endif