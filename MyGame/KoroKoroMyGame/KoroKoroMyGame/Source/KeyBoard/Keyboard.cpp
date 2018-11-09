//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// keyboard.cpp
// Author : MasayaHayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Keyboard.h"

// ===== 静的メンバ =====
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
	hr = pDIDevKeyboard->setDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = pDIDevKeyboard->setCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
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
	BYTE keyState[MaxKey];

	// デバイスからデータを取得
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
const bool Keyboard::getPress(INT nKey)
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