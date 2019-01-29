//��������������������������������������������������
// MyAudiere.h
// Author : Masaya Hayashi
//��������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MY_AUDIERE_H
#define MY_AUDIERE_H

// ===== �C���N���[�h�� =====
#include "audiere.h"
#include "d3dx9.h"
#include "../Linklib.h"
#include <unordered_map>

// ===== �\���̒�` =====
typedef struct
{
	CHAR *filePassPtr;
} AUDIO_DATA;

// ===== �񋓑̒�` =====
enum class BGM_TYPE
{
	BGM_TITLE = 0,
	BGM_EDIT,
	BGM_MAIN,
	MAX_BGM_TYPE,
};
enum class SE_TYPE
{
	SE_HOKOU = 0,				// ���s
	JUMP,
	MAX_SE_TYPE,
};

// ===== �N���X��` =====
class MyAudiere final
{
public:
	MyAudiere();
	~MyAudiere();

	// �Q�b�^�[
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