#include "../catalog.h"

static const tileCode YaojiuShunCode[] = {
	CharacterOne, CharacterSeven, CircleOne, CircleSeven, BambooOne, BambooSeven,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon
};
static const tileCode OneCode[] = {CharacterOne, CircleOne, BambooOne,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};
static const tileCode SevenCode[] = {CharacterSeven, CircleSeven, BambooSeven};
static const tileCode NineCode[] = {CharacterNine, CircleNine, BambooNine,
	EastWind, SouthWind, WestWind, NorthWind, WhiteDragon, GreenDragon, RedDragon};

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_yaojiu()
{
	/* �^�����I */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"�f���", (RuleData::chkRule("kuitan") == "yes_but_unbound") ? /* ��O�Ȃ甛��𖞂����A�򂢒f�͔���𖞂����Ȃ����[�� */
		yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return *analysis->MenzenFlag ?
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null) :
					yaku::yakuCalculator::Yaku::YAKU_HAN(yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_null,
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_1han);
			}) :
			((RuleData::chkRule("kuitan") == "no") ? (yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han_menzen : // �򂢒f�Ȃ�
			(yaku::yakuCalculator::Yaku::HANFUNC)yaku::yakuCalculator::Yaku::yval_1han), // �򂢒f����
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == 0);
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == 0);
			else return false;
		}
	));
	/* �`�����^ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���S�ћ��", yaku::yakuCalculator::Yaku::yval_2han_kuisagari,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles(), 13, YaojiuShunCode, 13, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�ΑS�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"���S�ћ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, OneCode, 10, OneCode, 3, false) == SIZE_OF_MELD_BUFFER) ||
					(yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, NineCode, 10, SevenCode, 3, false) == SIZE_OF_MELD_BUFFER) );
				else return false;
			}
		));
	/* ���`���� */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���S�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
		"���S�ћ��",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentzWithDup(analysis->MianziDat, Honor_Major_Tiles(), 6, YaojiuShunCode, 6, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("henchantaiyao"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"���ΑS�ћ��", yaku::yakuCalculator::Yaku::yval_3han_kuisagari,
			"���S�ћ��", "�ΑS�ћ��",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return ( (yaku::countingFacility::countSpecMentz(analysis->MianziDat, OneCode, 3, OneCode, 3, false) == SIZE_OF_MELD_BUFFER) ||
					(yaku::countingFacility::countSpecMentz(analysis->MianziDat, NineCode, 3, SevenCode, 3, false) == SIZE_OF_MELD_BUFFER) );
				else return false;
			}
		));
	/* �z�����[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���V��", get_yaku_han("honroutou"),
		"���S�ћ��",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 13, nullptr, 0, false) == SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles(), 13) == NUM_OF_TILES_IN_HAND / 2);
			else return false;
		}
	));
	/* �`�����[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"���V��", yaku::yakuCalculator::Yaku::yval_yakuman,
		"���S�ћ��", "���S�ћ��", "���V��", "�΁X�a",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles(), 6, nullptr, 0, false) == SIZE_OF_MELD_BUFFER);
			else return false;
		}
	));
	/* �c�[�C�[�\�[ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"����F", yaku::yakuCalculator::Yaku::yval_yakuman,
		"���S�ћ��", "���S�ћ��", "���V��", "����F", "�l����", "�΁X�a", "����F", "�O�����Ύq", "�l�쎵�Ύq", "�g������",
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
				return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, false) == SIZE_OF_MELD_BUFFER);
			else if (analysis->shanten[ShantenAnalyzer::shantenPairs] == -1)
				return (yaku::countingFacility::countPairs(analysis->TileCount, Honor_Major_Tiles()+6, 7) == NUM_OF_TILES_IN_HAND / 2);
			else return false;
		}
	));
	if (RuleData::chkRuleApplied("four_honors"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"�l����", get_yaku_han("four_honors"),
			"���S�ћ��", "���S�ћ��", "���V��", "����F", "����F",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->shanten[ShantenAnalyzer::shantenRegular] == -1)
					return (yaku::countingFacility::countSpecMentz(analysis->MianziDat, Honor_Major_Tiles()+6, 7, nullptr, 0, true) == (SIZE_OF_MELD_BUFFER - 1));
				else return false;
			}
		));
}