#include "random.h"

std::seed_seq RndNum::seed;
std::mt19937 RndNum::engine;

__declspec(dllexport) void RndNum::init() {
	std::random_device rnd;
	std::vector<std::uint32_t> v(624);
	std::generate(v.begin(), v.end(), std::ref(rnd));
	seed = std::seed_seq(v.begin(), v.end());
	engine = std::mt19937(seed);
}

/* 一様分布整乱数 */
__declspec(dllexport) unsigned int RndNum::rnd(unsigned int k) {
	std::uniform_int_distribution<int> distrib(0, k-1);
	return distrib(engine);
}

/* 一様分布浮動小数点乱数 */
double RndNum::rnd() {
	std::uniform_real_distribution<double> distrib(0.0, 1.0); // 半開区間 [0,1)
	return distrib(engine);
}
__declspec(dllexport) void RndNum::frnd(double* const ans) {
	std::uniform_real_distribution<double> distrib(0.0, 1.0); // 半開区間 [0,1)
	*ans = distrib(engine);
}

/* サイコロ */
__declspec(dllexport) unsigned int RndNum::dice() {
	std::uniform_int_distribution<int> distrib(1, 6);
	return distrib(engine);
}
