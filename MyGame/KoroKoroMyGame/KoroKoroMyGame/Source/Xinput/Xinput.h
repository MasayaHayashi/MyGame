//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xinput.h
// XINPUT入力
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef XINPUT_H
#define XINPUT_H

// ===== インクルード部 ======
#include "../Linklib.h"
#include <Windows.h>
#include <Xinput.h>
#include <memory>

// ===== クラス定義 =====
class Xinput final
{
public:
	void update();

	// ボタン状態取得
	static bool getButtonPress(WORD uButtonCode);
	static bool getButtonTriger(WORD uButtonCode);
	static bool getButtonRelease(WORD uButtonCode);
		 
	static bool getButton_A_Press();
	static bool getButton_A_Triger();
	static bool getButton_A_Release();
	 	 
	static bool getButton_B_Press();
	static bool getButton_B_Triger();
	static bool getButton_B_Release();
	 	 
	static bool getButton_X_Press();
	static bool getButton_X_Triger();
	static bool getButton_X_Release();
	 	 
	static bool getButton_Y_Press();
	static bool getButton_Y_Triger();
	static bool getButton_Y_Release();
	 	 
	static bool getButton_START_Press();
	static bool getButton_START_Triger();
	static bool getButton_START_Release();
	 	 
	static bool getButton_BACK_Press();
	static bool getButton_BACK_Triger();
	static bool getButton_BACK_Release();
	 	 
	static bool getButton_LB_Press();
	static bool getButton_LB_Triger();
	static bool getButton_LB_Release();
	 	 
	static bool getButton_RB_Press();
	static bool getButton_RB_Triger();
	static bool getButton_RB_Release();
	 
	static bool getButton_LeftThumb_Press();
	static bool getButton_LeftThumb_Triger();
	static bool getButton_LeftThumb_Release();
 	 
	static bool getButton_RightThumb_Press();
	static bool getButton_RightThumb_Triger();
	static bool getButton_RightThumb_Release();
 
	// 十字キー(DPad)ボタン判定
	static bool getButton_DpadUp_Press();
	static bool getButton_DpadUp_Triger();
	static bool getButton_DpadUp_Release();
	
	static bool getButton_DpadDown_Press();
	static bool getButton_DpadDown_Triger();
	static bool getButton_DpadDown_Release();

	static bool getButton_DpadLeft_Press();
	static bool getButton_DpadLeft_Triger();
	static bool getButton_DpadLeft_Release();
 	 
	static bool getButton_DpadRight_Press();
	static bool getButton_DpadRight_Triger();
	static bool getButton_DpadRight_Release();
 
	// デジタル入力値取得
	static BYTE  getLeftTrigger();	// 0～255 LT
	static BYTE  getRightTrigger();	// 0～255 RT
	static SHORT getThumbLX();		// -32768 ～ 32767　左スティック横軸
	static SHORT getThumbLY();		// -32768 ～ 32767  左スティック縦軸
	static SHORT getThumbRX();		// -32768 ～ 32767  右スティック横軸
	static SHORT getThumbRY();		// -32768 ～ 32767  左スティック縦軸
	 
	// 振動
	static void vibration_Left_Start(UINT nVibLevel);		// 左モーター振動
	static void vibration_Right_Start(UINT nVibLevel);		// 右モーター振動
	static void vibration_Left_Stop();
	static void vibration_Right_Stop();
	static void vibration_Start(UINT nVibLevel);						// 両方のモーター振動
	static void vibration_Stop();										// 両方停止
	static void vibration_AnyTime(UINT uVibTime, USHORT nVibLevel);	// 任意の時間振動

	static Xinput& getInstance();
	
protected:

private:
 	static XINPUT_STATE			state;
	static XINPUT_VIBRATION		vibration;
	static XINPUT_GAMEPAD		WorkStatePrev;		// 1フレーム前の状態退避用
	static XINPUT_GAMEPAD		WorkStateTrigger;		// ゲームパッドの状態を受け取るワーク
	static XINPUT_GAMEPAD		WorkStateRelease;		// ゲームパッドの状態を受け取るワーク
	static XINPUT_GAMEPAD		WorkStateRepeat;		// ゲームパッドの状態を受け取るワーク
	static bool bConnected;	  		// 接続中か
	static bool bCurrentViberation;	// 振動中か
	static INT  nVibCnt;			// 振動用カウンタ
	

	static constexpr INT MaxControllers = 4;
	static constexpr INT MaxVibration = 65535;

	static Xinput *pInstance;	// インスタンス
	Xinput();
	~Xinput();
};

#endif