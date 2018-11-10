//����������������������������������������������
// Keyboard.h
// �L�[�{�[�h�N���X
// Author : MasayaHayashi
//����������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef KEYBOARD_H
#define KEYBOARD_H

#define DIRECTINPUT_VERSION (0x0800)	// �x���΍�

// ===== �C���N���[�h�� =====
#include "../Linklib.h"
#include "d3dx9.h"
#include <dinput.h>
#include <atlbase.h>

// ===== �N���X��` =====
class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	HRESULT initialize(HINSTANCE hInst, HWND hWnd);
	void update();
	void draw();
	void finalize();

	HRESULT initializeKeyboard(HINSTANCE hInst, HWND hWnd);
	HRESULT updateKeyboard();


	static const bool getPress(INT nKey);
	static const bool getTrigger(INT nKey);
	static const bool getRepeat(INT nKey);
	static const bool getRelease(INT nKey);

private:
	static constexpr INT MaxKey = 256;

	CComPtr<IDirectInput8>			pDInput		   = nullptr;
	CComPtr<IDirectInputDevice8>	pDIDevKeyboard = nullptr;
	static BYTE						keyState[MaxKey];
	static BYTE						keyStateTrigger[MaxKey];
	static BYTE						keyStateRelease[MaxKey];
	static BYTE						keyStateRepeat[MaxKey];
	static INT						keyStateRepeatCnt[MaxKey];
};

#endif