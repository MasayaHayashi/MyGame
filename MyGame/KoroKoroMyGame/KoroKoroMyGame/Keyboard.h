//����������������������������������������������
// Keyboard.h
// �L�[�{�[�h�N���X
// Author : MasayaHayashi
//����������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef KEYBOARD_H
#define KEYBOARD_H

// ===== �C���N���[�h�� =====
#include "d3dx9.h"

// ===== �N���X��` =====
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