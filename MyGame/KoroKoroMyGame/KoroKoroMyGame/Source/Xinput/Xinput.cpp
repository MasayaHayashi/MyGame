//������������������������������������������������������������������������������������
// Xinput.cpp
// Author : Masaya Hayashi
// ChangeLog
//������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "Xinput.h"
#include "../DirectX3D/DirectX3D.h"

// ===== �O���[�o���ϐ��錾 =====
Xinput* Xinput::pInstance;
XINPUT_STATE		Xinput::state;
XINPUT_VIBRATION	Xinput::vibration;
XINPUT_GAMEPAD		Xinput::WorkStatePrev;
XINPUT_GAMEPAD		Xinput::WorkStateTrigger;
XINPUT_GAMEPAD		Xinput::WorkStateRelease;
XINPUT_GAMEPAD		Xinput::WorkStateRepeat;
bool				Xinput::bConnected;
bool				Xinput::bCurrentViberation;
INT					Xinput::nVibCnt;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Xinput::Xinput()
{
	// �ڑ�
	XInputGetState(0, &state);

	// �e�평����
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	bCurrentViberation = false;



	DWORD dwResult;

	dwResult = XInputGetState(0, &state);

	if (dwResult == ERROR_SUCCESS)
	{
		bConnected = true;
	}
	else
	{
		bConnected = false;
	}

	// �e�평����
	bCurrentViberation = false;
	nVibCnt = 0;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
Xinput::~Xinput()
{
	XInputEnable(false);	// Xinput�g�p�I��
}

//��������������������������������������
// �X�V
//��������������������������������������
void Xinput::update()
{
	// ��O����
	if (!bConnected)
	{
		return;
	}

	// 1�t���[���O�̏�Ԃ�ޔ�
	WorkStatePrev = state.Gamepad;

	// ���͏�ԍX�V
	XInputGetState(0, &state);

#define XINP_PRESS	(state.Gamepad.wButtons)
#define XINP_PREV	(WorkStatePrev.wButtons)

	WorkStateTrigger.wButtons = (XINP_PRESS ^ XINP_PREV) & XINP_PRESS;
	WorkStateRelease.wButtons = (XINP_PRESS ^ XINP_PREV) & ~XINP_PRESS;
	/*
	if (aKeyState[nCnKey])
	{
		if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
		{
			g_aKeyStateRepeatCnt[nCnKey] ++;
			if (g_aKeyStateRepeatCnt[nCnKey] == 1)
			{// �L�[�������n�߂��ŏ��̃t���[��
				g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
			}
			else if (g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
			{// �J�E���^���ő�l�𒴂����烊�Z�b�g
				g_aKeyStateRepeat[nCnKey] = 0;
				g_aKeyStateRepeatCnt[nCnKey] = 0;
			}
			else
			{
				g_aKeyStateRepeat[nCnKey] = 0;
			}
		}
	}
	*/
	
	nVibCnt --;

	if (nVibCnt <= 0)
	{
		vibration_Stop();
	}

	// �U������ 
	if (vibration.wLeftMotorSpeed != 0 && vibration.wRightMotorSpeed != 0)
	{
		bCurrentViberation = true;
	}
	else
	{
		bCurrentViberation = false;
	}
}

//����������������������������������������������������
// ���͔���i�v���X�j
//����������������������������������������������������
bool Xinput::getButtonPress(WORD uButtonCode)
{
	// ��O����
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & uButtonCode) ? true : false;
}

//����������������������������������������������������
// ���͔���(�g���K�[)
//����������������������������������������������������
bool Xinput::getButtonTriger(WORD uButtonCode)
{
	// ��O����
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & uButtonCode) ? true : false;
}

//����������������������������������������������������
// ���͔���(�����[�X)
//����������������������������������������������������
bool Xinput::getButtonRelease(WORD uButtonCode)
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & uButtonCode) ? true : false;
}




//����������������������������������������������������
// A�{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_A_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? true : false;
}

//����������������������������������������������������
// A�{�^�����͔��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_A_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_A) ? true : false;
}

//����������������������������������������������������
// A�{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_A_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & XINPUT_GAMEPAD_A) ? true : false;
}

//����������������������������������������������������
// B�{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_B_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? true : false;
}

//����������������������������������������������������
// B�{�^�����͔���(�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_B_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_B) ? true : false;
}

//����������������������������������������������������
// B�{�^�����͔���(�����[�X)
//����������������������������������������������������
bool Xinput::getButton_B_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_B) ? true : false;
}

//����������������������������������������������������
// X�{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_X_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? true : false;
}

//����������������������������������������������������
// X�{�^�����͔��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_X_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_X) ? true : false;
}

//����������������������������������������������������
// X�{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_X_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_X) ? true : false;
}

//����������������������������������������������������
// Y�{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_Y_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
}

//����������������������������������������������������
// Y�{�^�����͔��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_Y_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
}

//����������������������������������������������������
// Y�{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_Y_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
}


//����������������������������������������������������
// �X�^�[�g�{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_START_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? true : false;
}

//����������������������������������������������������
// �X�^�[�g�{�^�����͔��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_START_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_START) ? true : false;
}

//����������������������������������������������������
// �X�^�[�g�{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_START_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_START) ? true : false;
}

//����������������������������������������������������
// �o�b�N(�Z���N�g)�{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_BACK_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
}

//����������������������������������������������������
// �o�b�N(�Z���N�g)�{�^�����͔��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_BACK_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
}

//����������������������������������������������������
// �o�b�N(�Z���N�g)�{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_BACK_Release()
{	
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
}

//����������������������������������������������������
// LB�{�^��(L1) �������ݔ���
//����������������������������������������������������
bool Xinput::getButton_LB_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
}

//����������������������������������������������������
// LB�{�^��(L1) �������ݔ��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_LB_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
}

//����������������������������������������������������
// LB�{�^��(L1) �������ݔ��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_LB_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
}

//����������������������������������������������������
// RB�{�^��(R1) �������ݔ���
//����������������������������������������������������
bool Xinput::getButton_RB_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
}

//����������������������������������������������������
// RB�{�^��(R1) �������ݔ��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_RB_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
}

//����������������������������������������������������
// RB�{�^��(R1) �������ݔ��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_RB_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
}

//����������������������������������������������������
// ���X�e�B�b�N�������ݔ���
//����������������������������������������������������
bool Xinput::getButton_LeftThumb_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//����������������������������������������������������
// ���X�e�B�b�N�������ݔ��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_LeftThumb_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//����������������������������������������������������
// ���X�e�B�b�N�������ݔ��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_LeftThumb_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//����������������������������������������������������
// �E�X�e�B�b�N�������ݔ���
//����������������������������������������������������
bool Xinput::getButton_RightThumb_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//����������������������������������������������������
// �E�X�e�B�b�N�������ݔ��� (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_RightThumb_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//����������������������������������������������������
// �E�X�e�B�b�N�������ݔ��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_RightThumb_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//����������������������������������������������������
// �\���L�[����͔���
//����������������������������������������������������
bool Xinput::getButton_DpadUp_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
}

//����������������������������������������������������
// �\���L�[����͔���  (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_DpadUp_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
}

//����������������������������������������������������
// �\���L�[����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_DpadUp_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
}


//����������������������������������������������������
// �\���L�[�����͔���
//����������������������������������������������������
bool Xinput::getButton_DpadDown_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
}


//����������������������������������������������������
// �\���L�[�����͔���  (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_DpadDown_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
}

//����������������������������������������������������
// �\���L�[�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_DpadDown_Release ()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
}

//����������������������������������������������������
// �\���L�[�����̓{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_DpadLeft_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
}


//����������������������������������������������������
// �\���L�[�����̓{�^�����͔���  (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_DpadLeft_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
}


//����������������������������������������������������
// �\���L�[�����̓{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_DpadLeft_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
}

//����������������������������������������������������
// �\���L�[�E���̓{�^�����͔���
//����������������������������������������������������
bool Xinput::getButton_DpadRight_Press()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
}

//����������������������������������������������������
// �\���L�[�E���̓{�^�����͔���  (�g���K�[)
//����������������������������������������������������
bool Xinput::getButton_DpadRight_Triger()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
}

//����������������������������������������������������
// �\���L�[�E���̓{�^�����͔��� (�����[�X)
//����������������������������������������������������
bool Xinput::getButton_DpadRight_Release()
{
	// ���ڑ��Ȃ�false��
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
}

//��������������������������������������������������
// LT�̒l�擾
// �߂�l�F0�`255 
//��������������������������������������������������
BYTE Xinput::getLeftTrigger()
{

	XInputGetState(0, &state);

	// ��O����
	if (!bConnected)
	{
		return 0;
	}

	return state.Gamepad.bLeftTrigger;
}

//��������������������������������������������������
// RT�̒l�擾
// �߂�l�F0�`255 
//��������������������������������������������������
BYTE Xinput::getRightTrigger()
{
	XInputGetState(0, &state);
	
	// ��O����
	if (!bConnected)
	{
		return 0;
	}

	return state.Gamepad.bRightTrigger;
}

//��������������������������������������������������
// ���X�e�B�b�N�����̒l�擾
// �߂�l�F-32768 �` 32767
//��������������������������������������������������
SHORT Xinput::getThumbLX()
{
	XInputGetState(0, &state);

	// ���ڑ��Ȃ�l��0��
	if (!bConnected)
	{
		return 0;
	}

	// �f�b�h�]�[���ݒ� (���ʈȉ��Ȃ�l��0��)
	if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		 state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbLX = 0;
	}

	return state.Gamepad.sThumbLX;
}

//��������������������������������������������������
// ���X�e�B�b�N�c���̒l�擾
// �߂�l�F-32768 �` 32767
//��������������������������������������������������
SHORT Xinput::getThumbLY()
{
	XInputGetState(0, &state);

	// ���ڑ��Ȃ�l��0��
	if (!bConnected)
		return 0;

	// �f�b�h�]�[���ݒ� (���ʈȉ��Ȃ�l��0��)
	if ((state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbLY = 0;
	}

	return state.Gamepad.sThumbLY;
}

//��������������������������������������������������
// �E�X�e�B�b�N�����̒l�擾
// �߂�l�F-32768 �` 32767
//��������������������������������������������������
SHORT Xinput::getThumbRX()
{
	XInputGetState(0, &state);

	// ���ڑ��Ȃ�l��0��
	if (!bConnected)
		return 0;

	// �f�b�h�]�[���ݒ� (���ʈȉ��Ȃ�l��0��)
	if ((state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbRX = 0;
	}

	return state.Gamepad.sThumbRX;
}

//��������������������������������������������������
// �E�X�e�B�b�N�����̒l�擾
// �߂�l�F-32768 �` 32767
//��������������������������������������������������
SHORT Xinput::getThumbRY()
{
	XInputGetState(0, &state);

	// ���ڑ��Ȃ�l��0��
	if (!bConnected)
		return 0;

	// �f�b�h�]�[���ݒ� (���ʈȉ��Ȃ�l��0��)
	if ((state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbRY = 0;
	}

	return state.Gamepad.sThumbRY;
}

//��������������������������������������������������������������������������
// �����[�^�[�U���J�n
// nVibLevel : 0�`65535 ���[�^�[���x
//��������������������������������������������������������������������������
void Xinput::vibration_Left_Start(UINT nVibLevel)
{
	// ��O����
	if (0 > nVibLevel)
	{
		nVibLevel = 0;
		bCurrentViberation = false;
		return;
	}
	else if (MaxVibration < nVibLevel)
	{
		nVibLevel = MaxVibration;
	}

	// �t���O�X�V
	bCurrentViberation = true;

	// ���[�^�[�l���Z�b�g
	vibration.wLeftMotorSpeed = nVibLevel;
	XInputSetState(0, &vibration);

}

//��������������������������������������������������������������������������
// �E���[�^�[�U���J�n
// nVibLevel : 0�`65535 ���[�^�[���x
//��������������������������������������������������������������������������
void Xinput::vibration_Right_Start(UINT nVibLevel)
{
	// ��O����
	if (0 > nVibLevel)
	{
		nVibLevel = 0;
		bCurrentViberation = false;
		return;
	}
	else if (MaxVibration < nVibLevel)
	{
		nVibLevel = MaxVibration;
	}

	// �t���O�X�V
	bCurrentViberation = true;

	// ���[�^�[�l���Z�b�g
	vibration.wRightMotorSpeed = nVibLevel;
	XInputSetState(0, &vibration);
}

//��������������������������������������������������������������������������
// �����[�^�[�U����~
//��������������������������������������������������������������������������
void Xinput::vibration_Left_Stop()
{
	vibration.wLeftMotorSpeed = 0;

	XInputSetState(0, &vibration);
}

//��������������������������������������������������������������������������
// �E���[�^�[�U����~
//��������������������������������������������������������������������������
void Xinput::vibration_Right_Stop()
{
	vibration.wRightMotorSpeed = 0;

	XInputSetState(0, &vibration);
}

//��������������������������������������������������������������������������
// �����̃��[�^�[�U���J�n
// nVibLevel : 0�`65535 ���[�^�[���x
//��������������������������������������������������������������������������
void Xinput::vibration_Start(UINT nVibLevel)
{

	vibration_Left_Start(nVibLevel);
	vibration_Right_Start(nVibLevel);
}

//��������������������������������������������������������������������������
// �����̃��[�^�[�U����~
//��������������������������������������������������������������������������
void Xinput::vibration_Stop()
{

	vibration_Left_Stop();
	vibration_Right_Stop();
}

//��������������������������������������������������������������������������
// �C�ӂ̎��Ԃ�U�������� (�����̃��[�^�[)
// �����F�U�����ԁA�U���̋���(0�`65535)
//��������������������������������������������������������������������������
void Xinput::vibration_AnyTime(UINT uVibTime,USHORT nVibLevel)
{

	vibration_Left_Start(nVibLevel);
	vibration_Right_Start(nVibLevel);

	nVibCnt = uVibTime;	// �U�����ԃZ�b�g
}