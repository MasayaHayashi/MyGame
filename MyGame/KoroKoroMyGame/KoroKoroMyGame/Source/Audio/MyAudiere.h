//��������������������������������������������������
// MyAudiere.h
// Author : Masaya Hayashi
//��������������������������������������������������

// ===== ���d�C���N���[�h�h�~ =====
#ifndef MY_AUDIERE_H
#define MY_AUDIERE_H

// ===== �C���N���[�h�� =====
#include "audiere.h"
#include "../DirectX3D.h"
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
	SE_STAR = 0,			// �X�^�[�A�C�e��
	SE_JUMP,  				// �W�����v
	SE_STAGE_CLEAR,			// �X�e�[�W�N���A
	SE_LANDING,				// ���n
	SE_HOKOU,				// ���s
	MAX_SE_TYPE,
};

// ===== �N���X��` =====
class MyAudiere
{
public:
	MyAudiere();		// �R���X�g���N�^
	~MyAudiere();		// �f�X�g���N�^

	// �Q�b�^�[
	static audiere::OutputStreamPtr  getBgm(UINT);
	static audiere::SoundEffectPtr   getSe(UINT);

private:
	static const INT MaxSound = 128;

	audiere::AudioDevicePtr	audioDeviceObj;		// �f�o�C�X

	static std::unordered_map <UINT, audiere::OutputStreamPtr>	bgm;
	static std::unordered_map <UINT, audiere::SoundEffectPtr>	se;
	
	UINT	bgmKey;
	UINT	seKey;
};


#endif