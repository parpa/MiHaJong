#include "result.h"
#include "../graphic/graphic.h"
#include "../sound/sound.h"
#include "sound.h"

void gameResult(const GameTable* gameStat, int origTurn, int origHonba) {
	sound::util::bgmstop();
	mihajong_graphic::Transit(mihajong_graphic::sceneResult);
	/* TODO: 終了時の処理 */
	/* TODO: 入力待ち
	buttonPressed = 0
	repeat
		redrscreen
		await 1000
		if (buttonPressed) {break}
	loop
	*/
	Sleep(INFINITE);
}
