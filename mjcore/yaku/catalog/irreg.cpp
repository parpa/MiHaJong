#include "../catalog.h"

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_irregular()
{
	/* チートイ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"七対子", (getRule(RULE_SEVEN_PAIRS) == 1) ? /* 1翻50符のルールと2翻25符のルールがある。符はここでは設定できないです…… */
		yaku::yakuCalculator::Yaku::yval_1han_menzen : yaku::yakuCalculator::Yaku::yval_2han_menzen,
		[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[shantenPairs] == -1)&&(analysis->shanten[shantenRegular] >= 0));
		}
	));
	/* 国士 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		"国士無双", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
		"十三不塔",
		[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
			return (analysis->shanten[shantenOrphans] == -1);
		}
	));
	if (getRule(RULE_DOUBLE_YAKUMAN) == 0) /* 国士13面待ち（ダブル役満） */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"国士無双十三面", yaku::yakuCalculator::Yaku::yval_double_yakuman_menzen,
			"国士無双", "十三不塔",
			[](const GameTable* const gameStat, const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[shantenOrphans] == -1) && // 国士無双になっていて、
					(/* フリテンかどうか判定する */0) && // フリテンではなくて、
					((/* 13面待ちか判定する */0)||(gameStat->TianHuFlag))); // 13面待ちか天和になっている
			}
		));
	/* 七星無靠 */
	if (getRule(RULE_STELLAR_UUSHII) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"七星無靠", (getRule(RULE_STELLAR_UUSHII) == 2) ?
			yaku::yakuCalculator::Yaku::yval_baiman_menzen : yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			"全不靠",
			[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenStellar] == -1);
			}
		));
	/* 南北戦争 */
	if (getRule(RULE_CIVIL_WAR) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南北戦争", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenCivilWar] == -1);
			}
		));
	/* 惑星直列 */
	if (getRule(RULE_SYZYGY) != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"惑星直列", yaku::yakuCalculator::Yaku::yval_yakuman_menzen,
			[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenSyzygy] == -1);
			}
		));
	/* 全不靠 */
	if (getRule(RULE_QUANBUKAO) != 0) {
		const yaku::yakuCalculator::Yaku::MenzenHan* qbk_han = NULL;
		switch (getRule(RULE_QUANBUKAO)) {
		case 1: case 2:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_3han_menzen;
			break;
		case 3:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_4han_menzen;
			break;
		case 4:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_mangan_menzen;
			break;
		case 5:
			qbk_han = &yaku::yakuCalculator::Yaku::yval_yakuman_menzen;
			break;
		}
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"全不靠", *qbk_han,
			[](const GameTable* const, const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->shanten[shantenQuanbukao] == -1);
			}
		));
	}
}
