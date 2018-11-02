//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Keyboard.h
// キーボードクラス
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef KEYBOARD_H
#define KEYBOARD_H

// ===== インクルード部 =====
#include "d3dx9.h"

// ===== クラス定義 =====
class Keyboard
{
public:
	Keyboard();
	~Keyboard();


	HRESULT initialize(HINSTANCE hInst, HWND hWnd);
	void update();
	const void draw();
	void finalize();


	static bool getKeyboardPress(INT nKey);
	static bool getKeyboardTrigger(INT nKey);
	static bool getKeyboardRepeat(INT nKey);
	static bool getKeyboardRelease(INT nKey);

private:

};

#endif