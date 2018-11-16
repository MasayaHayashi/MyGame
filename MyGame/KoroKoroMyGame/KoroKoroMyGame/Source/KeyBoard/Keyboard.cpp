//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// keyboard.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Keyboard.h"

// ===== 静的メンバ =====
LPDIRECTINPUT8			Keyboard::dInputPtr;				// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	Keyboard::devKeyboardPtr;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					Keyboard::keyState[MaxKey];					// キーボードの押下状態を保持するワーク
BYTE					Keyboard::keyStateTrigger[MaxKey];			// キーボードのトリガー状態を保持するワーク
BYTE					Keyboard::keyStateRelease[MaxKey];			// キーボードのリリース状態を保持するワーク
BYTE					Keyboard::keyStateRepeat[MaxKey];				// キーボードのリピート状態を保持するワーク
INT						Keyboard::keyStateRepeatCnt[MaxKey];			// キーボードのリピートカウンタ

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝
Keyboard::Keyboard()
{
	
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝
Keyboard::~Keyboard()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Keyboard::initialize(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if (!dInputPtr)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&dInputPtr, nullptr);
	}

	// キーボードの初期化
	initializeKeyboard(hInst, hWnd);

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Keyboard初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Keyboard::initializeKeyboard(HINSTANCE hInst, HWND hWnd)
{

	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = dInputPtr->CreateDevice(GUID_SysKeyboard, &devKeyboardPtr, nullptr);
	if (FAILED(hr) || devKeyboardPtr == nullptr)
	{
		MessageBox(hWnd, "キーボードがありません", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = devKeyboardPtr->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = devKeyboardPtr->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	devKeyboardPtr->Acquire();

	return S_OK;
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 入力処理の更新処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Keyboard::update()
{
	updateKeyboard();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// キーボードの更新処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT Keyboard::updateKeyboard()
{
	HRESULT hr;
	BYTE aKeyState[MaxKey];

	// デバイスからデータを取得
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
		// キーボードへのアクセス権を取得
		devKeyboardPtr->Acquire();
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void Keyboard::finalize()
{

}


//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool Keyboard::getPress(INT nKey)
{
	return (keyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool Keyboard::getTrigger(INT nKey)
{
	return (keyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool Keyboard::getRepeat(INT nKey)
{
	return (keyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool Keyboard::getRelease(INT nKey)
{
	return (keyStateRelease[nKey] & 0x80) ? true : false;

}