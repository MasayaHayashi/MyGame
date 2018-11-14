//����������������������������������������������
// Keyboard.h
// �L�[�{�[�h�N���X
// Author : MasayaHayashi
//����������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef KEYBOARD_H
#define KEYBOARD_H

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


	static LPDIRECTINPUT8			pDInput;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
	static LPDIRECTINPUTDEVICE8	pDIDevKeyboard;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)

	/*
	BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̉�����Ԃ�ێ����郏�[�N
	BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��Ԃ�ێ����郏�[�N
	BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��Ԃ�ێ����郏�[�N
	BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��Ԃ�ێ����郏�[�N
	int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^
	*/
};

#endif