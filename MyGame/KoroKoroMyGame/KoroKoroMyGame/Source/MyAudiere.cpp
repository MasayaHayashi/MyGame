//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_Audio.h
// オーディオ関連
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "MyAudiere.h"
#include <string>
#include <array>

// ===== 静的メンバ変数 =====
std::unordered_map <UINT, audiere::OutputStreamPtr> MyAudiere::bgm;
std::unordered_map <UINT, audiere::SoundEffectPtr>  MyAudiere::se;

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
MyAudiere::MyAudiere()
{
	audioDeviceObj = audiere::OpenDevice();

	// 例外処理
	if (!audioDeviceObj)
		return;

	// BGMパス設定
	std::array<AUDIO_DATA, MaxSound> bgmFilePass =
	{
		"data/BGM/Title.wav"  ,
		"data/BGM/Edit.wav "  ,
		"data/BGM/Main.wav "  ,
	};

	// BGM初期化
	for (INT i = 0; i < MaxSound; i++)
	{
		if (!bgmFilePass[i].filePassPtr)
			break;
		
		std::pair<UINT, audiere::OutputStreamPtr> setPair = std::make_pair(i, audiere::OpenSound(audioDeviceObj.get(), bgmFilePass[i].filePassPtr));
		bgm.insert(setPair);
	}

	// SEパス設定
	std::array<AUDIO_DATA, MaxSound> seFilePass =
	{
		"data/SE/Star.wav"		,	// スターアイテム
		"data/SE/Jump.wav"		,	// ジャンプ
		"data/SE/StageClear.wav",	// ステージクリア
		"data/SE/landing.wav"	,	// 着地
		"data/SE/Hokou.wav"		,	// 歩行
	};

	// SE初期化
	for (INT i = 0; i < MaxSound; i++)
	{
		if (!seFilePass[i].filePassPtr)
			break;

		std::pair<UINT, audiere::SoundEffectPtr> setPair = std::make_pair(i, audiere::OpenSoundEffect(audioDeviceObj.get(), seFilePass[i].filePassPtr, audiere::SoundEffectType::MULTIPLE));
		se.insert(setPair);
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
MyAudiere::~MyAudiere()
{
	bgm.clear();
	se.clear();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// BGM取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
audiere::OutputStreamPtr MyAudiere::getBgm(UINT index)
{
	return bgm[index];
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SE取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
audiere::SoundEffectPtr MyAudiere::getSe(UINT index)
{
	return se[index];
}
