//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MyAudiere.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MY_AUDIERE_H
#define MY_AUDIERE_H

// ===== インクルード部 =====
#include "audiere.h"
#include "../DirectX3D.h"
#include "../Linklib.h"
#include <unordered_map>

// ===== 構造体定義 =====
typedef struct
{
	CHAR *filePassPtr;
} AUDIO_DATA;

// ===== 列挙体定義 =====
enum class BGM_TYPE
{
	BGM_TITLE = 0,
	BGM_EDIT,
	BGM_MAIN,
	MAX_BGM_TYPE,
};
enum class SE_TYPE
{
	SE_STAR = 0,			// スターアイテム
	SE_JUMP,  				// ジャンプ
	SE_STAGE_CLEAR,			// ステージクリア
	SE_LANDING,				// 着地
	SE_HOKOU,				// 歩行
	MAX_SE_TYPE,
};

// ===== クラス定義 =====
class MyAudiere
{
public:
	MyAudiere();		// コンストラクタ
	~MyAudiere();		// デストラクタ

	// ゲッター
	static audiere::OutputStreamPtr  getBgm(UINT);
	static audiere::SoundEffectPtr   getSe(UINT);

private:
	static const INT MaxSound = 128;

	audiere::AudioDevicePtr	audioDeviceObj;		// デバイス

	static std::unordered_map <UINT, audiere::OutputStreamPtr>	bgm;
	static std::unordered_map <UINT, audiere::SoundEffectPtr>	se;
	
	UINT	bgmKey;
	UINT	seKey;
};


#endif