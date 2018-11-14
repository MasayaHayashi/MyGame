//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Keyboard.h
// キーボードクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef KEYBOARD_H
#define KEYBOARD_H

// ===== インクルード部 =====
#include "../Linklib.h"
#include "d3dx9.h"
#include <dinput.h>
#include <atlbase.h>

// ===== クラス定義 =====
class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	HRESULT initialize(HINSTANCE hInst, HWND hWnd);
	static void update();
	static void finalize();

	HRESULT initializeKeyboard(HINSTANCE hInst, HWND hWnd);
	static HRESULT updateKeyboard();


	static bool getPress(INT nKey);
	static const bool getTrigger(INT nKey);
	static const bool getRepeat(INT nKey);
	static const bool getRelease(INT nKey);

private:
	static constexpr INT MaxKey = 256;

//	static CComPtr<IDirectInput8>		pDInput;
//	static CComPtr<IDirectInputDevice8>	pDIDevKeyboard;
	static BYTE		keyState[MaxKey];
	static BYTE		keyStateTrigger[MaxKey];
	static BYTE		keyStateRelease[MaxKey];
	static BYTE		keyStateRepeat[MaxKey];
	static INT		keyStateRepeatCnt[MaxKey];


	static LPDIRECTINPUT8			pDInput;					// IDirectInput8インターフェースへのポインタ
	static LPDIRECTINPUTDEVICE8	pDIDevKeyboard;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)

	/*
	BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの押下状態を保持するワーク
	BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー状態を保持するワーク
	BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース状態を保持するワーク
	BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート状態を保持するワーク
	int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ
	*/
};

#endif