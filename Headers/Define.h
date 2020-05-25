#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "SingletonDefine.h"
#include "KeyCodeDefine.h"
#include "FlagDefine.h"
#include "FilePath.h"

#ifdef _TOOL
constexpr int WINCX = 640;
constexpr int WINCY = 480;
#elif _CLIENT
constexpr int WINCX = 1280;
constexpr int WINCY = 960;
#endif

constexpr int TILEX = 40;
constexpr int TILEY = 40;

#ifdef _TOOL
constexpr int TILECX = 128;
constexpr int TILECY = 64;
#elif _CLIENT
constexpr int TILECX = 256;
constexpr int TILECY = 128;
#endif

constexpr WORD MIN_STR = 64;
constexpr WORD MID_STR = 128;
constexpr WORD MAX_STR = 256;

#include <string.h>
#define INIT_ARRAY(arr) memset(arr, 0, sizeof(arr));
#define INIT_STRUCT(st) memset(&st, 0, sizeof(st));

#endif // !__DEFINE_H__
