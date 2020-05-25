// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <io.h>
#include <process.h>

// C++
#include <string>
#include <fstream>

// STL
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

// DirectX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

// User Defines
#include "Extern.h"
#include "Define.h"
#include "Struct.h"
#include "Enum.h"
#include "FilePath.h"
#include "Function.h"
#include "Functor.h"
#include "SingletonDefine.h"
#include "KeyCodeDefine.h"
#include "FlagDefine.h"

// FMOD
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

// Manager
#include "DeviceManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "FrameManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "ScrollManager.h"
#include "SceneManager.h"
#include "AStarManager.h"

// vld
#ifdef _DEBUG
#include "vld.h"
#pragma comment(lib, "vld.lib")
#endif