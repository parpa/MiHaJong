#include "../catalog.h"

extern const std::array<char[4], 6> parsedat_trichrome3;

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_triplet_2() {
	/* 奇数対々和 */
	if (RuleData::getRule("odd_toitoi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"奇数対々和", yaku::yakuCalculator::Yaku::yval_4han,
			"対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 1; i < TILE_SUIT_HONORS; i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SIZE_OF_MELD_BUFFER);
			}
		));
	/* 偶数対々和 */
	if (RuleData::getRule("even_toitoi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"偶数対々和", yaku::yakuCalculator::Yaku::yval_5han,
			"対々和", "断幺九",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				int count = 0;
				for (int i = 2; i < TILE_SUIT_HONORS; i += 2)
					if (analysis->DuiziCount[i] >= 1) ++count;
				return (count == SIZE_OF_MELD_BUFFER);
			}
		));

	// ---------------------------------------------------------------------

	/* 中原逐鹿 */
	if (RuleData::getRule("chuugen_chikuroku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中原逐鹿", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・發", "役牌・中", "絶一門",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[CircleSix] >= 1) );
			}
		));
	/* 金亀孵蛋 */
	if (RuleData::getRule("turtle_egg") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"金亀孵蛋", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));
	/* 旭日東昇 */
	if (RuleData::getRule("kyokujitsu_toushou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"旭日東昇", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) );
			}
		));
	/* 日落西山 */
	if (RuleData::getRule("riirao_shiishan") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"日落西山", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1) );
			}
		));
	/* 揺望北斗 */
	if (RuleData::getRule("youbou_hokuto") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"揺望北斗", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) );
			}
		));
	/* 北斗七星 */
	if (RuleData::getRule("big_dipper") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北斗七星", yaku::yakuCalculator::Yaku::yval_yakuman,
			"ビックボーナス", "三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) );
			}
		));
	/* 西廂待月 */
	if (RuleData::getRule("seisou_taigetsu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西廂待月", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1) &&
					(analysis->TsumoHai->tile == CircleOne) );
			}
		));
	/* 南無八幡 */
	if (RuleData::getRule("namu_hachiman") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南無八幡", yaku::yakuCalculator::Yaku::yval_yakuman,
			"役牌・白", "南白つきもの",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1) );
			}
		));
	/* 西湖十景 */
	if (RuleData::getRule("seiko_jikkei") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西湖十景", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1 && analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1 && analysis->KeziCount[CircleEight] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1 && analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1 && analysis->KeziCount[CircleSix] >= 1);
			}
		));
	/* 火山爆発 */
	if (RuleData::getRule("volcanic_erruption") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"火山爆発", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・發", "役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[CharacterNine] >= 1 ||
					analysis->KeziCount[CircleNine] >= 1 || analysis->KeziCount[BambooNine] >= 1);
			}
		));
	/* 丹鳳朝陽 */
	if (RuleData::getRule("tanfon_chouyou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"丹鳳朝陽", yaku::yakuCalculator::Yaku::yval_yakuman,
			"役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) );
			}
		));

	// ---------------------------------------------------------------------

	auto haouben =
		[](const MENTSU_ANALYSIS* const analysis, tileCode tc, int step) -> bool {
			if ((analysis->KangziCount[tc] >= 1) &&
				(analysis->KeziCount[tc + step] >= 1) &&
				(analysis->KangziCount[tc + step] == 0) &&
				(analysis->MianziDat[0].tile == (tileCode)(tc + step * 2)) )
				return true;
			if ((analysis->KangziCount[tc + step * 2] >= 1) &&
				(analysis->KeziCount[tc + step] >= 1) &&
				(analysis->KangziCount[tc + step] == 0) &&
				(analysis->MianziDat[0].tile == tc) )
				return true;
			return false;
		};

	/* 覇王鞭 */
	if (RuleData::getRule("haouben") == 1)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"覇王鞭", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 3; k++)
						yakuFlag = yakuFlag || haouben(analysis, (tileCode)(i + k), 3);
				return yakuFlag;
			}
		));
	else if (RuleData::getRule("haouben") == 2)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"覇王鞭", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int k = 1; k <= 3; k++)
					yakuFlag = yakuFlag || haouben(analysis, (tileCode)(TILE_SUIT_BAMBOOS + k), 3);
				return yakuFlag;
			}
		));
	/* 劉関張 */
	if (RuleData::getRule("ryuukanchan") == 2)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"劉関張", yaku::yakuCalculator::Yaku::yval_6han,
			[haouben](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				for (int i = 0; i < TILE_SUIT_HONORS; i += TILE_SUIT_STEP)
					for (int k = 1; k <= 7; k++)
						yakuFlag = yakuFlag || haouben(analysis, (tileCode)(i + k), 1);
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 鵲橋会 */
	if (RuleData::getRule("queqiaohui") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"鵲橋会", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->KeziCount[BambooOne] >= 1) {
					if ((analysis->KeziCount[CharacterSeven] >= 1) &&
						(analysis->KeziCount[CircleSeven] >= 1)) return true;
					else if ((analysis->KeziCount[CharacterSeven] >= 1) &&
						(analysis->KeziCount[BambooSeven] >= 1)) return true;
					else if ((analysis->KeziCount[CircleSeven] >= 1) &&
						(analysis->KeziCount[BambooSeven] >= 1)) return true;
					else return false;
				}
				else return false;
			}
		));
	/* 一竿風月 */
	if (RuleData::getRule("yigan_fengyue") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一竿風月", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					((analysis->KeziCount[EastWind] >= 1) || (analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) || (analysis->KeziCount[NorthWind] >= 1) ));
			}
		));
	/* 南の朱雀 */
	if (RuleData::getRule("vermilion_bird") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南の朱雀", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "混一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[SouthWind] >= 1) &&
					(analysis->KangziCount[BambooOne] >= 1) &&
					((analysis->KeziCount[BambooFive] >= 1 && analysis->KeziCount[BambooSeven] >= 1) ||
					(analysis->KeziCount[BambooFive] >= 1 && analysis->KeziCount[BambooNine] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1 && analysis->KeziCount[BambooNine] >= 1)) &&
					(analysis->MianziDat[0].tile == RedDragon));
			}
		));
	/* 西の白虎 */
	if (RuleData::getRule("white_tiger") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西の白虎", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[WestWind] >= 1) &&
					(analysis->KangziCount[CharacterThree] >= 1) &&
					(analysis->KangziCount[CircleThree] >= 1) &&
					(analysis->KangziCount[BambooThree] >= 1) &&
					(analysis->MianziDat[0].tile == WhiteDragon));
			}
		));
	/* 三杯美酒敬梅花 */
	if (RuleData::getRule("sanbei_meijiu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"三杯美酒敬梅花", yaku::yakuCalculator::Yaku::yval_yakuman,
			"役牌・中",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CircleFive] >= 1) &&
					((analysis->KeziCount[CharacterNine] >= 1) ||
					(analysis->KeziCount[CircleNine] >= 1) ||
					(analysis->KeziCount[BambooNine] >= 1) ));
			}
		));
	/* 東風圧倒西風 */
	if (RuleData::getRule("dongfeng_yadao") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東風圧倒西風", yaku::yakuCalculator::Yaku::HANFUNC(
			[](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(analysis->KangziCount[EastWind] >= 1) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_3han :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_2han);
			}),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (((analysis->KangziCount[EastWind] == 0) && // 東刻子・西対子
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->MianziDat[0].tile == WestWind)) ||
					((analysis->KangziCount[EastWind] >= 1) && // 東槓子・西刻子
					(analysis->KangziCount[WestWind] == 0) &&
					(analysis->KeziCount[WestWind] >= 1)));
			}
		));
	/* 太公釣金亀 */
	if (RuleData::getRule("taikou_turtle") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"太公釣金亀", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooTwo] >= 1) &&
					(analysis->TsumoHai->tile == BambooThree) &&
					(analysis->Machi == yaku::yakuCalculator::machiTanki));
			}
		));
	/* 青函トンネル */
	if (RuleData::getRule("seikan_tunnel") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"青函トンネル", yaku::yakuCalculator::Yaku::yval_yakuman,
			"役牌・發", "東發つきもの",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KangziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[EastWind] >= 1) &&
					(analysis->AnKeziCount[EastWind] == 0) &&
					(analysis->KeziCount[CircleOne] >= 1));
			}
		));
	/* 宝紅開花 */
	if (RuleData::getRule("baohongkaihua") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"宝紅開花", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・白", "役牌・中", "混老頭", "三色小同刻", "白鳥",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CharacterOne] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1) &&
					(analysis->DuiziCount[BambooOne] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 東發つきもの */
	if (RuleData::getRule("east_green") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東發つきもの", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1));
			}
		));
	/* 南白つきもの */
	if (RuleData::getRule("south_white") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"南白つきもの", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1));
			}
		));
	/* 中西君 */
	if (RuleData::getRule("west_red") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中西君", (RuleData::getRule("west_red") == 2) ?
			yaku::yakuCalculator::Yaku::yval_3han : yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 白鳥 */
	if (RuleData::getRule("hakucho") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白鳥", yaku::yakuCalculator::Yaku::yval_5han,
			"対々和", "役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[BambooOne] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->TotalKezi == SIZE_OF_MELD_BUFFER - 1));
			}
		));
	/* 火山爆発(五索と七索) */
	if (RuleData::getRule("volcanic_toitoi") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"火山爆発 ", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・中", "役牌・發", "混一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[BambooThree] >= 1) &&
					(analysis->DuiziCount[BambooFive] >= 1) &&
					(analysis->DuiziCount[BambooSeven] >= 1) &&
					(analysis->DuiziCount[GreenDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* 白一色の定義処理はmisc.cppに移動しました */

	/* 雪の銀閣 */
	if (RuleData::getRule("snow_ginkaku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"雪の銀閣", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・白", "混一色", "断紅和", "混断紅",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* 北白川 */
	if (RuleData::getRule("kitashirakawa") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"北白川", yaku::yakuCalculator::Yaku::yval_yakuman,
			"役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[NorthWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[BambooSix] >= 1));
			}
		));
	/* 素数役満の定義処理はmisc.cppに移動しました */

	/* アル中 */
	if (RuleData::getRule("alcoholism") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"アル中", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・中", "三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CharacterTwo] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[BambooTwo] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* 救急車 */
	if (RuleData::getRule("kyukyusha") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"救急車", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "役牌・白", "三色同刻", "混老頭",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* 盗難車 */
	if (RuleData::getRule("tounansha") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"盗難車", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三風刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[WestWind] >= 1));
			}
		));
	/* 青春に悔いなし */
	if (RuleData::getRule("seishun_kuinashi") != 0) {
		auto seishunnikuinashi =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1));
			};
		if (RuleData::getRule("seishun_kuinashi") == 1) {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"青春に悔いなし", yaku::yakuCalculator::Yaku::yval_1han_menzen,
				seishunnikuinashi
			));
		} else {
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"青春に悔いなし", yaku::yakuCalculator::Yaku::yval_2han_menzen,
				"青春に悔いあり",
				seishunnikuinashi
			));
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"青春に悔いあり", yaku::yakuCalculator::Yaku::yval_1han,
				seishunnikuinashi
			));
		}
	}
	/* スーパーマン */
	if (RuleData::getRule("superman") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"スーパーマン", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if ((analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->KeziCount[CharacterEight] >= 1)) return true;
				else if ((analysis->MianziDat[0].tile == CharacterEight) &&
					(analysis->KeziCount[CharacterFour] >= 1) &&
					(analysis->TsumoHai->tile == CharacterFour)) return true;
				else if ((analysis->MianziDat[0].tile == CharacterFour) &&
					(analysis->KeziCount[CharacterEight] >= 1) &&
					(analysis->TsumoHai->tile == CharacterEight)) return true;
				else return false;
			}
		));
	/* 東花園 */
	if (RuleData::getRule("higashi_hanazono_triplets") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東花園", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					((analysis->KeziCount[CharacterEight] >= 1) || (analysis->KeziCount[CircleEight] >= 1) ||
					(analysis->KeziCount[BambooEight] >= 1)) &&
					((analysis->KeziCount[CharacterSeven] >= 1) || (analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1)));
			}
		));
	/* 琳両数 */
	if (RuleData::getRule("rinryanshii") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"琳両数", yaku::yakuCalculator::Yaku::yval_6han,
			/* 対々和が必ず複合 */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				char pattern[] = "121314151617181923242526272829343536373839454647484956575859676869787989";
				tileCode tCode[TILE_SUIT_HONORS / TILE_SUIT_STEP * 2];
				bool yakuFlag = false;
				for (int i = 0; i < 36; i++) {
					int num1 = (int)(pattern[i * 2] - '0');
					int num2 = (int)(pattern[i * 2 + 1] - '0');
					for (int k = 0; k < TILE_SUIT_HONORS / TILE_SUIT_STEP * 2; k += 2) {
						tCode[k] = (tileCode)(k / 2 * TILE_SUIT_HONORS + num1);
						tCode[k + 1] = (tileCode)(k / 2 * TILE_SUIT_HONORS + num2);
					}
					if (yaku::countingFacility::countSpecMentz(
						analysis->MianziDat, tCode, TILE_SUIT_HONORS / TILE_SUIT_STEP * 2, NULL, 0, false))
						yakuFlag = true;
				}
				return yakuFlag;
			}
		));
	/* ３分間待ってやる */
	auto anysuit3 =
		[](const MENTSU_ANALYSIS* const analysis, int val1, int val2, int val3, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (int suit1 = 0; suit1 < TILE_SUIT_HONORS; suit1+= TILE_SUIT_STEP)
				for (int suit2 = 0; suit2 < TILE_SUIT_HONORS; suit2 += TILE_SUIT_STEP)
					for (int suit3 = 0; suit3 < TILE_SUIT_HONORS; suit3 += TILE_SUIT_STEP)
						if (((*count)[suit1 + val1] >= 1) && ((*count)[suit2 + val2] >= 1) &&
							((*count)[suit3 + val3] >= 1)) yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::getRule("colonel_muska") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ムスカ大佐", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 6, 4, 3, true) && (analysis->Machi == yaku::yakuCalculator::machiKanchan);
			}
		));
	/* 都の西北 */
	if (RuleData::getRule("miyakonoseihoku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"都の西北", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和",
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 5, false) &&
					(analysis->DuiziCount[WestWind] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1);
			}
		));
	/* 東京二十三区 */
	if (RuleData::getRule("tokyo") != 0) {
#include "isshoku.h" /* やぶれかぶれ。行儀の悪いinclude */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東京二十三区",  yaku::yakuCalculator::Yaku::HANFUNC(
			[isshoku](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(isshoku(analysis, false)) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"対々和",
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 2, 3, 9, true) &&
					(analysis->KeziCount[EastWind] >= 1);
			}
		));
	}
	/* 初音ミク */
	auto anysuit2 =
		[](const MENTSU_ANALYSIS* const analysis, int val1, int val2, bool noDui) -> bool {
			const Int8ByTile* count = noDui ? &analysis->KeziCount : &analysis->DuiziCount;
			bool yakuFlag = false;
			for (int suit1 = 0; suit1 < TILE_SUIT_HONORS; suit1+= TILE_SUIT_STEP)
				for (int suit2 = 0; suit2 < TILE_SUIT_HONORS; suit2 += TILE_SUIT_STEP)
					if (((*count)[suit1 + val1] >= 1) && ((*count)[suit2 + val2] >= 1))
						yakuFlag = true;
			return yakuFlag;
		};
	if (RuleData::getRule("hatsune_miku") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"初音ミク", yaku::yakuCalculator::Yaku::yval_2han,
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 3, 9, true) && (analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* 白花 */
	if (RuleData::getRule("baihua") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"白花", yaku::yakuCalculator::Yaku::yval_1han,
			/* 別途白で1飜 */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleFive] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));
	/* 撃鼓愕曹 */
	if (RuleData::getRule("jigu_ecao") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"撃鼓愕曹", yaku::yakuCalculator::Yaku::yval_2han,
			"役牌・白",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[CircleTwo] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1);
			}
		));

	// ---------------------------------------------------------------------

	/* ビックボーナス */
	if (RuleData::getRule("777") != 0) {
		auto yaku777 =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterSeven] >= 1) &&
					(analysis->KeziCount[CircleSeven] >= 1) &&
					(analysis->KeziCount[BambooSeven] >= 1);
			};
		if (RuleData::getRule("777") == 2)
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ビックボーナス", yaku::yakuCalculator::Yaku::yval_yakuman,
				"三色同刻",
				yaku777
			));
		else
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				"ビックボーナス", yaku::yakuCalculator::Yaku::yval_2han,
				yaku777
			));
	}
	/* クリスマス */
	if (RuleData::getRule("christmas") != 0) {
		auto chris =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				for (int i = 0; i < 6; i++)
					if ((analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 0] + 1] >= 1) &&
						(analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 0] + 2] >= 1) &&
						(analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 1] + 2] >= 1) &&
						(analysis->KeziCount[(&parsedat_trichrome3[0][0])[i * 4 + 1] + 5] >= 1))
						return true;
				return false;
			};
		auto chrisday = 
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 12) && (nowTime.wDay == 25);
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"クリスマス", yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_yakuman);
			}),
			"対々和",
			chris
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純正クリスマス", yaku::yakuCalculator::Yaku::HANFUNC(
			[chrisday](const MENTSU_ANALYSIS* const analysis) {
				return yaku::yakuCalculator::Yaku::YAKU_HAN(
					(chrisday()) ?
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_triple_yakuman :
					yaku::yakuCalculator::Yaku::YAKU_HAN::HAN::yv_double_yakuman);
			}),
			"対々和", "クリスマス",
			[chris](const MENTSU_ANALYSIS* const analysis) -> bool {
				return chris(analysis) &&
					(analysis->MianziDat[0].tile == BambooSeven);
			}
		));
	}
	/* 一富士二鷹三茄子 */
	if (RuleData::getRule("newyear_dream") != 0) {
		auto isnewyeardays = 
			[]() -> bool {
				SYSTEMTIME nowTime; GetLocalTime(&nowTime);
				return (nowTime.wMonth == 1) && (nowTime.wDay <= 2);
			};
		auto newyrdrm =
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if (analysis->PlayerStat->MeldPointer >= 3)
					return ((analysis->PlayerStat->Meld[1].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[1].tile == BambooThree) &&
						(analysis->PlayerStat->Meld[2].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[2].tile == BambooOne) &&
						(analysis->PlayerStat->Meld[3].mstat > meldTripletConcealed) &&
						(analysis->PlayerStat->Meld[3].tile == CircleEight));
				else return false;
			};
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"一富士二鷹三茄子", yaku::yakuCalculator::Yaku::yval_yakuman,
			newyrdrm
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純正一富士二鷹三茄子", yaku::yakuCalculator::Yaku::yval_double_yakuman,
			"一富士二鷹三茄子",
			[newyrdrm, isnewyeardays](const MENTSU_ANALYSIS* const analysis) -> bool {
				return newyrdrm(analysis) && isnewyeardays();
			}
		));
	}
	/* 武蔵丸 */
	if (RuleData::getRule("musashimaru") != 0) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"武蔵丸", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 6, 3, 4, true) &&
					(analysis->KeziCount[CircleOne] >= 1);
			}
		));
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純正武蔵丸", yaku::yakuCalculator::Yaku::yval_yakuman,
			"武蔵丸",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CircleSix] >= 1) &&
					(analysis->KeziCount[CircleThree] >= 1) &&
					(analysis->KeziCount[CircleFour] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->MianziDat[0].tile / TILE_SUIT_STEP == TILE_SUIT_CIRCLES / TILE_SUIT_STEP);
			}
		));
	}

	// ---------------------------------------------------------------------

	/* 阪神 */
	if (RuleData::getRule("hanshin") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"阪神", yaku::yakuCalculator::Yaku::yval_1han,
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 8, 4, true);
			}
		));
	/* 神戸 */
	if (RuleData::getRule("kobe") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"神戸", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterFive] >= 1) ||
					(analysis->KeziCount[CircleFive] >= 1) ||
					(analysis->KeziCount[BambooFive] >= 1)) &&
					(analysis->KeziCount[NorthWind] >= 1);
			}
		));
	/* サッポロ一番 */
	if (RuleData::getRule("sapporo1") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"サッポロ一番", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit3](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit3(analysis, 3, 8, 6, true) &&
					(analysis->GameStat->GameRound == 0) && // 東一局であって
					(analysis->GameStat->LoopRound == 0) && // 返り東場でもなくて
					(analysis->GameStat->Honba == 0); // 平場の時
			}
		));
	/* ポカリスエット */
	if (RuleData::getRule("pcrswt") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ポカリスエット", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleTwo] >= 1) ||
					(analysis->KeziCount[CircleFour] >= 1) ||
					(analysis->KeziCount[CircleSix] >= 1) ||
					(analysis->KeziCount[CircleEight] >= 1)) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* アクエリアス */
	if (RuleData::getRule("aqrs") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"アクエリアス", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) ||
					(analysis->KeziCount[CircleThree] >= 1) ||
					(analysis->KeziCount[CircleFive] >= 1) ||
					(analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[CircleNine] >= 1)) &&
					(analysis->KeziCount[GreenDragon] >= 1);
			}
		));
	/* ＳＭＡＰ */
	if (RuleData::getRule("smap") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"ＳＭＡＰ", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterFour] +
					analysis->KeziCount[CircleFour] +
					analysis->KeziCount[BambooFour] -
					analysis->AnKeziCount[CharacterFour] -
					analysis->AnKeziCount[CircleFour] -
					analysis->AnKeziCount[BambooFour] >= 1));
			}
		));
	/* モンスターハンター */
	if (RuleData::getRule("monster_hunter") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"モンスターハンター", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) ||
					(analysis->KeziCount[NorthWind] >= 1)) &&
					(analysis->GameStat->TurnRound <= 6);
			}
		));
	/* 旭川ラーメン */
	if (RuleData::getRule("asahikawa_raamen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"旭川ラーメン", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[NorthWind] >= 1));
			}
		));

	// ---------------------------------------------------------------------

	/* 東方妖々夢 */
	if (RuleData::getRule("perfect_cherry_blossom") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東方妖々夢", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三色同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterOne] >= 1) &&
					(analysis->KeziCount[CircleOne] >= 1) &&
					(analysis->KeziCount[BambooOne] >= 1);
			}
		));
	/* 東方紅楼夢 */
	if (RuleData::getRule("touhou_kouroumu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東方紅楼夢", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->KeziCount[CharacterSix] >= 1) &&
					(analysis->TsumoHai->tile == EastWind);
			}
		));
	/* Windows95 */
	if (RuleData::getRule("windows95") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows95", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三色同刻", "対々和", "琳両数",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterFive] +
					analysis->DuiziCount[CircleFive] +
					analysis->DuiziCount[BambooFive] >= 2);
			}
		));
	/* Windows98 */
	if (RuleData::getRule("windows98") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows98", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三色同刻", "対々和", "琳両数",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[CharacterEight] +
					analysis->DuiziCount[CircleEight] +
					analysis->DuiziCount[BambooEight] >= 2);
			}
		));
	/* Windows3.1 */
	if (RuleData::getRule("windows31") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows3.1", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "琳両数",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return (analysis->DuiziCount[CharacterThree] +
					analysis->DuiziCount[CircleThree] +
					analysis->DuiziCount[BambooThree] +
					analysis->DuiziCount[CharacterOne] +
					analysis->DuiziCount[CircleOne] +
					analysis->DuiziCount[BambooOne] == SIZE_OF_MELD_BUFFER);
			}
		));
	/* Windows7 */
	if (RuleData::getRule("windows7") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"Windows7", yaku::yakuCalculator::Yaku::yval_1han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag = false;
				if ((analysis->TsumoHai->tile == CharacterSeven) &&
					((analysis->KeziCount[CircleSeven] >= 1) ||
					(analysis->KeziCount[BambooSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai->tile == CircleSeven) &&
					((analysis->KeziCount[BambooSeven] >= 1) ||
					(analysis->KeziCount[CharacterSeven] >= 1))) yakuFlag = true;
				else if ((analysis->TsumoHai->tile == BambooSeven) &&
					((analysis->KeziCount[CharacterSeven] >= 1) ||
					(analysis->KeziCount[CircleSeven] >= 1))) yakuFlag = true;
				return yakuFlag;
			}
		));
	/* 東方風神録 */
	if (RuleData::getRule("mountain_of_faith") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"東方風神録", yaku::yakuCalculator::Yaku::yval_yakuman,
			[anysuit2](const MENTSU_ANALYSIS* const analysis) -> bool {
				return anysuit2(analysis, 6, 7, true) &&
					((analysis->KeziCount[EastWind] >= 1) ||
					(analysis->KeziCount[SouthWind] >= 1) ||
					(analysis->KeziCount[WestWind] >= 1) ||
					(analysis->KeziCount[NorthWind] >= 1));
			}
		));
	/* 博麗霊夢 */
	if (RuleData::getRule("hakurei_reimu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"博麗霊夢", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "絶一門",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[CharacterEight] >= 1) &&
					(analysis->DuiziCount[CharacterNine] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[EastWind] >= 1) &&
					(analysis->DuiziCount[CircleOne] >= 1));
			}
		));
	/* 霧雨魔理沙 */
	if (RuleData::getRule("kirisame_marisa") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"霧雨魔理沙", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "絶一門",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1) &&
					(analysis->DuiziCount[CharacterThree] >= 1));
			}
		));
	/* 十六夜咲夜 */
	if (RuleData::getRule("izayoi_sakuya") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"十六夜咲夜", yaku::yakuCalculator::Yaku::yval_yakuman,
			"対々和", "混一色",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->KeziCount[BambooEight] >= 1) &&
					(analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->Machi == yaku::yakuCalculator::machiShanpon));
			}
		));
	/* 西南戦争 */
	if (RuleData::getRule("seinansensou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"西南戦争", yaku::yakuCalculator::Yaku::yval_2han,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[BambooThree] >= 1));
			}
		));
	/* 純正西南戦争 */
	if (RuleData::getRule("junsei_seinansensou") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"純正西南戦争", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三色小同刻",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->DuiziCount[CharacterNine] >= 1) &&
					(analysis->DuiziCount[CircleNine] >= 1) &&
					(analysis->DuiziCount[BambooNine] >= 1));
			}
		));
	/* 九州新幹線 */
	if (RuleData::getRule("kyushu_shinkansen") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"九州新幹線", yaku::yakuCalculator::Yaku::yval_yakuman,
			"三色同刻", "五門斎", "混老頭",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[CircleNine] >= 1) &&
					(analysis->KeziCount[BambooNine] >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] + analysis->DuiziCount[GreenDragon] +
					analysis->DuiziCount[RedDragon] >= 1));
			}
		));
	/* 阿拉 */
	if (RuleData::getRule("ara") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"阿拉", yaku::yakuCalculator::Yaku::yval_yakuman,
			"小三元", "対々和", "混老頭",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[CharacterNine] >= 1) &&
					(analysis->KeziCount[SouthWind] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[WhiteDragon] >= 1));
			}
		));
	/* 快馬加鞭 */
	if (RuleData::getRule("kaibakaben") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"快馬加鞭", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"大三元", "字一色", "対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[WestWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* 酔江月 */
	if (RuleData::getRule("suikougetsu") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"酔江月", yaku::yakuCalculator::Yaku::yval_triple_yakuman,
			"大三元", "字一色", "対々和",
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->KeziCount[EastWind] >= 1) &&
					(analysis->KeziCount[WhiteDragon] >= 1) &&
					(analysis->KeziCount[GreenDragon] >= 1) &&
					(analysis->KeziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[NorthWind] >= 1));
			}
		));
	/* 中目黒 */
	if (RuleData::getRule("nakameguro") != 0)
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			"中目黒", yaku::yakuCalculator::Yaku::yval_yakuman,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->DuiziCount[RedDragon] >= 1) &&
					(analysis->DuiziCount[CircleTwo] >= 1) &&
					(analysis->DuiziCount[CircleFour] >= 1) &&
					(analysis->DuiziCount[CircleEight] >= 1) &&
					(analysis->DuiziCount[EastWind] + analysis->DuiziCount[SouthWind] +
					analysis->DuiziCount[WestWind] + analysis->DuiziCount[NorthWind]>= 1));
			}
		));
}
