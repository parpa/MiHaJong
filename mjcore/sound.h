#pragma once

#include "../sound/sound.h"

namespace sound {
namespace util {

void bgmload(unsigned ID, const char* filename, bool looped); /* BGMÇÝÝ */
void soundload(unsigned ID, const char* filename, bool looped); /* TEhÇÝÝ */
void bgmstop(); /* BGMâ~ */
void bgmplay(unsigned ID); /* aflÄ¶ */
void setvolume(); /* ¹ÊÝèð½f */

}
}
