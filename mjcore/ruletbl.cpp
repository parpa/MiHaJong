#include "ruletbl.h"

struct GameTable;
extern GameTable GameStat;
inline bool chkGameType(const GameTable* const gameStat, gameTypeID gameType);

char RuleData::ruleConf[RULESIZE/RULE_IN_LINE][RULE_IN_LINE + 1];
RULETBL RuleData::Rules;
std::array<std::string, RULESIZE> RuleData::nametbl;
CSVReader::CsvVecVec RuleData::confdat;
INIParser::IniMapMap RuleData::confdict;
std::map<std::string, unsigned int> RuleData::inverse_nametbl;
const char RuleData::digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void RuleData::configinit_csv() { // コンフィグ用CSVを読み込む
	DWORD size = 0; const uint8_t* csv = NULL;
	LoadFileInResource(IDR_CSV_TABL1, CSV_TABLE, size, csv);
	char *csvdat = new char[size + 4]; memset(csvdat, 0, size+4); memcpy_s(csvdat, size+4, csv, size);
	CSVReader::parsecsv(confdat, csvdat);
	delete[] csvdat;

	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		nametbl[std::atoi((*k)[0].c_str())] = (*k)[8]; // 順方向
		inverse_nametbl[std::string((*k)[8])] = std::atoi((*k)[0].c_str()); // 逆方向
	}
}

void RuleData::configinit_ini() { // コンフィグ文字列変換用INIを読み込む
	DWORD size = 0; const uint8_t* ini = NULL;
	LoadFileInResource(IDR_INI_FIL1, INI_FILE, size, ini);
	char *inidat = new char[size + 4]; memset(inidat, 0, size+4); memcpy_s(inidat, size+4, ini, size);
	INIParser::parseini(confdict, inidat);
	delete[] inidat;
}

__declspec(dllexport) void RuleData::configinit() { // コンフィグ用CSVを読み込む
	configinit_csv(); configinit_ini();
}

void RuleData::parseRule() { // ルール設定を数値に変換
	debug("ルール設定を連想配列に変換します。");
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == std::string::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // HSP→C++ ルール設定転送
	debug("HSP→C++ ルール転送");
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info("設定がロードされました。");
}

__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // C++→HSP ルール設定転送
	debug("C++→HSP ルール転送");
	for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

std::string RuleData::chkRule(std::string RuleTag) { // ルール設定タグを取得する
	return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ルール設定
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
bool RuleData::chkRuleApplied(std::string RuleTag) { // ルール設定
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
uint8_t RuleData::getRule(int RuleID) { // ルール設定を取得する[OBSOLETE]
	return Rules[nametbl[RuleID]];
}
__declspec(dllexport) int getRule(int RuleID) { // ルール設定を取得する[OBSOLETE]
	return (int)RuleData::getRule(RuleID);
}

__declspec(dllexport) void RuleData::getRuleName(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if ((chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) ||
			(chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))))
			strcpy_s(txt, bufsize, ((*k)[9]).c_str()); return;
	}
	strcpy_s(txt, bufsize, "");
}

__declspec(dllexport) void RuleData::getRuleDescription(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) {
			strcpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) {
			if (chkGameType(&GameStat, SanmaS)) strcpy_s(txt, bufsize, "数牌三麻では設定できません");
			else if (chkGameType(&GameStat, SanmaX)) strcpy_s(txt, bufsize, "三人打ちでは設定できません");
			else if (chkGameType(&GameStat, Yonma)) strcpy_s(txt, bufsize, "四人打ちでは設定できません");
			else strcpy_s(txt, bufsize, "");
			return;
		}
	}
	strcpy_s(txt, bufsize, "");
}

std::string RuleData::getRuleItemTag(std::function<bool(const CSVReader::RECORD&)> RuleF, int index) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (RuleF(*k)) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) {
			if ((*k).size() > (unsigned int)(11 + index))
				return std::string((*k)[11 + index]);
			else return std::string("");
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) {
			return std::string("N/A");
		}
	}
	return std::string("");
}
std::string RuleData::getRuleItemTag(int RuleID, int index) {
	return getRuleItemTag(
		[RuleID](const CSVReader::RECORD& k){return std::atoi(k[0].c_str()) != RuleID;},
		index);
}
std::string RuleData::getRuleItemTag(std::string RuleTag, int index) {
	return getRuleItemTag(
		[RuleTag](const CSVReader::RECORD& k){return k[8] != RuleTag;},
		index);
}

__declspec(dllexport) void RuleData::getRuleTxt(char* const txt, int bufsize, int RuleID, int index) {
	const std::string tag = getRuleItemTag(RuleID, index);
	if ((confdict.find("dictionary") != confdict.end()) &&
		(confdict["dictionary"].find(tag) != confdict["dictionary"].end()))
		strcpy_s(txt, bufsize, confdict["dictionary"][tag].c_str());
	else strcpy_s(txt, bufsize, tag.c_str());
}

__declspec(dllexport) int RuleData::loadConfigFile(const char* const filename) {
	errno_t err; FILE* conffile;
	if (err = fopen_s(&conffile, filename, "r")) { // オープンし、失敗したら
		std::ostringstream o;
		o << "設定ファイルのオープンに失敗しました。エラーコード [" << err << "]";
		error(o.str().c_str());
		fclose(conffile); // ファイルを閉じる
		return -1;
	} else { // 正しくオープンされたら
		fseek(conffile, 0, SEEK_END); long filesize = ftell(conffile); rewind(conffile); // ファイルサイズを取得
		long bufsize = (filesize | (sizeof(int) - 1)) + 1;
		char* const filedat = new char[bufsize]; // バッファを確保
		memset(filedat, 0, bufsize); // バッファをゼロクリア
		fread_s(filedat, bufsize, sizeof(char), filesize, conffile); // 読み込み
		{
			INIParser::IniMapMap config_ini; // INIパース結果を格納する「マップのマップ」
			INIParser::parseini(config_ini, filedat); // INIをパースする
			for (int i = 0; i < (RULESIZE/RULE_IN_LINE); i++) { // 初期化
				memset(ruleConf[i], 0, RULE_IN_LINE + 1);
				memset(ruleConf[i], '-', RULE_IN_LINE);
			}
			auto config_rules = config_ini["rules"];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rulesセクションについて
				if (inverse_nametbl.find(k->first) != inverse_nametbl.end()) { // キーがあったら
					unsigned int ruleid = inverse_nametbl[k->first]; // 番号に変換
				} else { // なかったら
					std::ostringstream o; o << "キー [" << k->first << "] は無視されます";
					warn(o.str().c_str());
				}
			}
		}
		delete[] filedat; // バッファを解放
		fclose(conffile); // ファイルを閉じる
		return 0;
	}
}
