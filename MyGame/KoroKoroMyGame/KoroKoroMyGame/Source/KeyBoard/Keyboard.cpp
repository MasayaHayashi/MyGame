//������������������������������������������
// keyboard.cpp
// Author : MasayaHayashi
//������������������������������������������

// ===== �C���N���[�h�� =====
#include "Keyboard.h"

// ===== �ÓI�����o =====
LPDIRECTINPUT8			Keyboard::dInputPtr;				// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	Keyboard::devKeyboardPtr;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					Keyboard::keyState[MaxKey];					// �L�[�{�[�h�̉�����Ԃ�ێ����郏�[�N
BYTE					Keyboard::keyStateTrigger[MaxKey];			// �L�[�{�[�h�̃g���K�[��Ԃ�ێ����郏�[�N
BYTE					Keyboard::keyStateRelease[MaxKey];			// �L�[�{�[�h�̃����[�X��Ԃ�ێ����郏�[�N
BYTE					Keyboard::keyStateRepeat[MaxKey];				// �L�[�{�[�h�̃��s�[�g��Ԃ�ێ����郏�[�N
INT						Keyboard::keyStateRepeatCnt[MaxKey];			// �L�[�{�[�h�̃��s�[�g�J�E���^

//��������������������������
// �R���X�g���N�^
//��������������������������
Keyboard::Keyboard()
{
	
}

//��������������������������
// �f�X�g���N�^
//��������������������������
Keyboard::~Keyboard()
{

}

//��������������������������������������������������
// ������
//��������������������������������������������������
HRESULT Keyboard::initialize(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if (!dInputPtr)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&dInputPtr, nullptr);
	}

	// �L�[�{�[�h�̏�����
	initializeKeyboard(hInst, hWnd);

	return S_OK;
}

//����������������������������������������������������
// Keyboard������
//����������������������������������������������������
HRESULT Keyboard::initializeKeyboard(HINSTANCE hInst, HWND hWnd)
{

	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = dInputPtr->CreateDevice(GUID_SysKeyboard, &devKeyboardPtr, nullptr);
	if (FAILED(hr) || devKeyboardPtr == nullptr)
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = devKeyboardPtr->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = devKeyboardPtr->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	devKeyboardPtr->Acquire();

	return S_OK;
}


//����������������������������������
// ���͏����̍X�V����
//����������������������������������
void Keyboard::update()
{
	updateKeyboard();
}

//��������������������������������������������
// �L�[�{�[�h�̍X�V����
//��������������������������������������������
HRESULT Keyboard::updateKeyboard()
{
	HRESULT hr;
	BYTE aKeyState[MaxKey];

	// �f�o�C�X����f�[�^���擾
	hr = devKeyboardPtr->GetDeviceState(sizeof(aKeyState), aKeyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < MaxKey; nCntKey++)
		{
			keyStateTrigger[nCntKey] = (keyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			keyStateRelease[nCntKey] = (keyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			keyStateRepeat[nCntKey] = keyStateTrigger[nCntKey];

			if (aKeyState[nCntKey])
			{
				keyStateRepeatCnt[nCntKey]++;
				if (keyStateRepeatCnt[nCntKey] >= 20)
				{
					keyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
			}
			else
			{
				keyStateRepeatCnt[nCntKey] = 0;
				keyStateRepeat[nCntKey] = 0;
			}

			keyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		devKeyboardPtr->Acquire();
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void Keyboard::finalize()
{

}


//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool Keyboard::getPress(INT nKey)
{
	return (keyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool Keyboard::getTrigger(INT nKey)
{
	return (keyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool Keyboard::getRepeat(INT nKey)
{
	return (keyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool Keyboard::getRelease(INT nKey)
{
	return (keyStateRelease[nKey] & 0x80) ? true : false;

}