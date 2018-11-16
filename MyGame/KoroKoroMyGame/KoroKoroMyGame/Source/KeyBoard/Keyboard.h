//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Keyboard.h
// キーボードクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef KEYBOARD_H
#define KEYBOARD_H

#define DIRECTINPUT_VERSION (0x0800)

// ===== インクルード部 =====
#include "../Linklib.h"
#include "dinput.h"
#include "d3dx9.h"
#include <atlbase.h>

// ===== クラス定義 =====
class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	static HRESULT initialize(HINSTANCE hInst, HWND hWnd);
	static void update();
	static void finalize();

	static HRESULT initializeKeyboard(HINSTANCE hInst, HWND hWnd);
	static HRESULT updateKeyboard();


	static bool getPress(INT nKey);
	static bool getTrigger(INT nKey);
	static bool getRepeat(INT nKey);
	static bool getRelease(INT nKey);

private:
	static constexpr INT MaxKey = 256;

	static LPDIRECTINPUT8			dInputPtr;
	static LPDIRECTINPUTDEVICE8		devKeyboardPtr;			
	static BYTE						keyState[MaxKey];
	static BYTE						keyStateTrigger[MaxKey];
	static BYTE						keyStateRelease[MaxKey];
	static BYTE						keyStateRepeat[MaxKey];
	static INT						keyStateRepeatCnt[MaxKey];


};

#endif