//������������������������������������������
// keyboard.cpp
// Author : MasayaHayashi
//������������������������������������������

// ===== �C���N���[�h�� =====
#include "Keyboard.h"

// ===== �ÓI�����o =====
LPDIRECTINPUT8			Keyboard::pDInput;
LPDIRECTINPUTDEVICE8	Keyboard::pDIDevKeyboard;

BYTE	Keyboard::keyState[MaxKey];
BYTE	Keyboard::keyStateTrigger[MaxKey];
BYTE	Keyboard::keyStateRelease[MaxKey];
BYTE	Keyboard::keyStateRepeat[MaxKey];
INT		Keyboard::keyStateRepeatCnt[MaxKey];

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

	if (!pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			 IID_IDirectInput8, (void**)&pDInput, nullptr);
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
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, nullptr);
	if (FAILED(hr) || pDIDevKeyboard == nullptr)
	{
		MessageBox(hWnd, "�L�[�{�[�h������܂���", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	pDIDevKeyboard->Acquire();

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
	BYTE aKeyState[2500];

	// �f�o�C�X����f�[�^���擾
	hr = pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < 256; nCntKey++)
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
		pDIDevKeyboard->Acquire();
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
const bool Keyboard::getTrigger(int nKey)
{
	return (keyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
const bool Keyboard::getRepeat(int nKey)
{
	return (keyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
const bool Keyboard::getRelease(int nKey)
{
	return (keyStateRelease[nKey] & 0x80) ? true : false;
}