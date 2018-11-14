//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// keyboard.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Keyboard.h"

// ===== 静的メンバ =====
LPDIRECTINPUT8			Keyboard::pDInput;
LPDIRECTINPUTDEVICE8	Keyboard::pDIDevKeyboard;

BYTE	Keyboard::keyState[MaxKey];
BYTE	Keyboard::keyStateTrigger[MaxKey];
BYTE	Keyboard::keyStateRelease[MaxKey];
BYTE	Keyboard::keyStateRepeat[MaxKey];
INT		Keyboard::keyStateRepeatCnt[MaxKey];

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

	if (!pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			 IID_IDirectInput8, (void**)&pDInput, nullptr);
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
	hr = pDInput->CreateDevice(GUID_SysKeyboard, &pDIDevKeyboard, nullptr);
	if (FAILED(hr) || pDIDevKeyboard == nullptr)
	{
		MessageBox(hWnd, "キーボードがありません", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	pDIDevKeyboard->Acquire();

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
	BYTE aKeyState[2500];

	// デバイスからデータを取得
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
		// キーボードへのアクセス権を取得
		pDIDevKeyboard->Acquire();
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
const bool Keyboard::getTrigger(int nKey)
{
	return (keyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
const bool Keyboard::getRepeat(int nKey)
{
	return (keyStateRepeat[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
const bool Keyboard::getRelease(int nKey)
{
	return (keyStateRelease[nKey] & 0x80) ? true : false;
}