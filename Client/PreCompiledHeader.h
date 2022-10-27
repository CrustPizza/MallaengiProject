#pragma once
// �� ��� ������ Client Project�� Pre-Compiled Header�Դϴ�.

// Include Engine Static Library

//Sound
#include "../Library/SoundManager.h"

#pragma comment(lib, "..\\..\\Output\\Engine.lib")
#pragma comment(lib, "..\\..\\Output\\Library.lib")

// Include Engine Pre-Compiled Header
#include "..\\Engine\\pch.h"

// C++
#include <functional>

// In-Game
#include "Game.h"

// Ÿ�� ������ ��Ʈ���ϴ� Define
#define TILE_WIDTH 80
#define TILE_HEIGHT 77

// ī�޶� �� ������ ����ؼ� UI���� ��ġ�� �����մϴ�.
#define CAMERA_ZOOM 0.7f

#define _AFXDLL
#undef _WINDOWS_
#include <afx.h>

#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif