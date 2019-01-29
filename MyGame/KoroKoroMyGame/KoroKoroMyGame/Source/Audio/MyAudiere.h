//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// MyAudiere.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef MY_AUDIERE_H
#define MY_AUDIERE_H

// ===== インクルード部 =====
#include "audiere.h"
#include "d3dx9.h"
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
	SE_HOKOU = 0,				// 歩行
	JUMP,
	MAX_SE_TYPE,
};

// ===== クラス定義 =====
class MyAudiere final
{
public:
	MyAudiere();
	~MyAudiere();

	// ゲッター
	static audiere::OutputStreamPtr  getBgm(UINT);
	static audiere::SoundEffectPtr   getSe(UINT);

private:
	static constexpr INT MaxSound = 128;

	audiere::AudioDevicePtr	audioDeviceObj;

	static std::unordered_map <UINT, audiere::OutputStreamPtr>	bgm;
	static std::unordered_map <UINT, audiere::SoundEffectPtr>	se;
	
	UINT	bgmKey;
	UINT	seKey;
};


#endif