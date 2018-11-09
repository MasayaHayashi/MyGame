//������������������������������������������
// keyboard.cpp
// Author : MasayaHayashi
//������������������������������������������

// ===== �C���N���[�h�� =====
#include "Keyboard.h"

// ===== �ÓI�����o =====
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
	hr = pDIDevKeyboard->setDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = pDIDevKeyboard->setCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
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
	BYTE keyState[MaxKey];

	// �f�o�C�X����f�[�^���擾
	hr = pDIDevKeyboard->getDeviceState(sizeof(keyState), keyState);
	if (SUCCEEDED(hr))
	{
		for (int nCntKey = 0; nCntKey < MaxKey; nCntKey++)
		{
			keyStateTrigger[nCntKey] = (keyState[nCntKey] ^ keyState[nCntKey]) & keyState[nCntKey];
			keyStateRelease[nCntKey] = (keyState[nCntKey] ^ keyState[nCntKey]) & keyState[nCntKey];
			keyStateRepeat[nCntKey]	 = keyStateTrigger[nCntKey];

			if (keyState[nCntKey])
			{
				keyStateRepeatCnt[nCntKey]++;
				if (keyStateRepeatCnt[nCntKey] >= 20)
				{
					keyStateRepeat[nCntKey] = keyState[nCntKey];
				}
			}
			else
			{
				keyStateRepeatCnt[nCntKey] = 0;
				keyStateRepeat[nCntKey] = 0;
			}

			keyState[nCntKey] = keyState[nCntKey];
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
const bool Keyboard::getPress(INT nKey)
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