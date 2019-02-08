//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xinput.cpp
// Author : Masaya Hayashi
// ChangeLog
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Xinput.h"
#include "../DirectX3D/DirectX3D.h"

// ===== グローバル変数宣言 =====
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

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Xinput::Xinput()
{
	// 接続
	XInputGetState(0, &state);

	// 各種初期化
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

	// 各種初期化
	bCurrentViberation = false;
	nVibCnt = 0;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Xinput::~Xinput()
{
	XInputEnable(false);	// Xinput使用終了
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::update()
{
	// 例外処理
	if (!bConnected)
	{
		return;
	}

	// 1フレーム前の状態を退避
	WorkStatePrev = state.Gamepad;

	// 入力状態更新
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
			{// キーを押し始めた最初のフレーム
				g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
			}
			else if (g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
			{// カウンタが最大値を超えたらリセット
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

	// 振動判定 
	if (vibration.wLeftMotorSpeed != 0 && vibration.wRightMotorSpeed != 0)
	{
		bCurrentViberation = true;
	}
	else
	{
		bCurrentViberation = false;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 入力判定（プレス）
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButtonPress(WORD uButtonCode)
{
	// 例外処理
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & uButtonCode) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 入力判定(トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButtonTriger(WORD uButtonCode)
{
	// 例外処理
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & uButtonCode) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 入力判定(リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButtonRelease(WORD uButtonCode)
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & uButtonCode) ? true : false;
}




//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Aボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_A_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Aボタン入力判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_A_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_A) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Aボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_A_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & XINPUT_GAMEPAD_A) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Bボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_B_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Bボタン入力判定(トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_B_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_B) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Bボタン入力判定(リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_B_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_B) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_X_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xボタン入力判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_X_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_X) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_X_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_X) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Yボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_Y_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Yボタン入力判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_Y_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Yボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_Y_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_Y) ? true : false;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スタートボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_START_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スタートボタン入力判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_START_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_START) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// スタートボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_START_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_START) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// バック(セレクト)ボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_BACK_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// バック(セレクト)ボタン入力判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_BACK_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// バック(セレクト)ボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_BACK_Release()
{	
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_BACK) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// LBボタン(L1) 押し込み判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_LB_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// LBボタン(L1) 押し込み判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_LB_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// LBボタン(L1) 押し込み判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_LB_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// RBボタン(R1) 押し込み判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_RB_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// RBボタン(R1) 押し込み判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_RB_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// RBボタン(R1) 押し込み判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_RB_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左スティック押し込み判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_LeftThumb_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左スティック押し込み判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_LeftThumb_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左スティック押し込み判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_LeftThumb_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右スティック押し込み判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_RightThumb_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右スティック押し込み判定 (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_RightThumb_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右スティック押し込み判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_RightThumb_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return (WorkStateRelease.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー上入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadUp_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー上入力判定  (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadUp_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー上入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadUp_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? true : false;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー下入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadDown_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー下入力判定  (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadDown_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー下入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadDown_Release ()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー左入力ボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadLeft_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー左入力ボタン入力判定  (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadLeft_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー左入力ボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadLeft_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー右入力ボタン入力判定
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadRight_Press()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー右入力ボタン入力判定  (トリガー)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadRight_Triger()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateTrigger.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 十字キー右入力ボタン入力判定 (リリース)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Xinput::getButton_DpadRight_Release()
{
	// 未接続ならfalseに
	if (!bConnected)
		return false;

	return  (WorkStateRelease.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? true : false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// LTの値取得
// 戻り値：0～255 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BYTE Xinput::getLeftTrigger()
{

	XInputGetState(0, &state);

	// 例外処理
	if (!bConnected)
	{
		return 0;
	}

	return state.Gamepad.bLeftTrigger;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// RTの値取得
// 戻り値：0～255 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BYTE Xinput::getRightTrigger()
{
	XInputGetState(0, &state);
	
	// 例外処理
	if (!bConnected)
	{
		return 0;
	}

	return state.Gamepad.bRightTrigger;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左スティック横軸の値取得
// 戻り値：-32768 ～ 32767
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SHORT Xinput::getThumbLX()
{
	XInputGetState(0, &state);

	// 未接続なら値を0に
	if (!bConnected)
	{
		return 0;
	}

	// デッドゾーン設定 (一定量以下なら値を0に)
	if ((state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		 state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbLX = 0;
	}

	return state.Gamepad.sThumbLX;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左スティック縦軸の値取得
// 戻り値：-32768 ～ 32767
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SHORT Xinput::getThumbLY()
{
	XInputGetState(0, &state);

	// 未接続なら値を0に
	if (!bConnected)
		return 0;

	// デッドゾーン設定 (一定量以下なら値を0に)
	if ((state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbLY = 0;
	}

	return state.Gamepad.sThumbLY;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右スティック横軸の値取得
// 戻り値：-32768 ～ 32767
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SHORT Xinput::getThumbRX()
{
	XInputGetState(0, &state);

	// 未接続なら値を0に
	if (!bConnected)
		return 0;

	// デッドゾーン設定 (一定量以下なら値を0に)
	if ((state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbRX = 0;
	}

	return state.Gamepad.sThumbRX;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右スティック横軸の値取得
// 戻り値：-32768 ～ 32767
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SHORT Xinput::getThumbRY()
{
	XInputGetState(0, &state);

	// 未接続なら値を0に
	if (!bConnected)
		return 0;

	// デッドゾーン設定 (一定量以下なら値を0に)
	if ((state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		state.Gamepad.sThumbRY = 0;
	}

	return state.Gamepad.sThumbRY;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左モーター振動開始
// nVibLevel : 0～65535 モーター強度
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_Left_Start(UINT nVibLevel)
{
	// 例外処理
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

	// フラグ更新
	bCurrentViberation = true;

	// モーター値をセット
	vibration.wLeftMotorSpeed = nVibLevel;
	XInputSetState(0, &vibration);

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右モーター振動開始
// nVibLevel : 0～65535 モーター強度
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_Right_Start(UINT nVibLevel)
{
	// 例外処理
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

	// フラグ更新
	bCurrentViberation = true;

	// モーター値をセット
	vibration.wRightMotorSpeed = nVibLevel;
	XInputSetState(0, &vibration);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 左モーター振動停止
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_Left_Stop()
{
	vibration.wLeftMotorSpeed = 0;

	XInputSetState(0, &vibration);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 右モーター振動停止
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_Right_Stop()
{
	vibration.wRightMotorSpeed = 0;

	XInputSetState(0, &vibration);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 両方のモーター振動開始
// nVibLevel : 0～65535 モーター強度
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_Start(UINT nVibLevel)
{

	vibration_Left_Start(nVibLevel);
	vibration_Right_Start(nVibLevel);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 両方のモーター振動停止
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_Stop()
{

	vibration_Left_Stop();
	vibration_Right_Stop();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 任意の時間を振動させる (両方のモーター)
// 引数：振動時間、振動の強さ(0～65535)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Xinput::vibration_AnyTime(UINT uVibTime,USHORT nVibLevel)
{

	vibration_Left_Start(nVibLevel);
	vibration_Right_Start(nVibLevel);

	nVibCnt = uVibTime;	// 振動時間セット
}