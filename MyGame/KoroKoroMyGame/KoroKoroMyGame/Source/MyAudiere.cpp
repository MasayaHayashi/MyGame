//������������������������������������������������
// C_Audio.h
// �I�[�f�B�I�֘A
// Author : Masaya Hayashi
//������������������������������������������������

// ===== �C���N���[�h�� =====
#include "MyAudiere.h"
#include <string>
#include <array>

// ===== �ÓI�����o�ϐ� =====
std::unordered_map <UINT, audiere::OutputStreamPtr> MyAudiere::bgm;
std::unordered_map <UINT, audiere::SoundEffectPtr>  MyAudiere::se;

//��������������������������������
// �R���X�g���N�^
//��������������������������������
MyAudiere::MyAudiere()
{
	audioDeviceObj = audiere::OpenDevice();

	// ��O����
	if (!audioDeviceObj)
		return;

	// BGM�p�X�ݒ�
	std::array<AUDIO_DATA, MaxSound> bgmFilePass =
	{
		"data/BGM/Title.wav"  ,
		"data/BGM/Edit.wav "  ,
		"data/BGM/Main.wav "  ,
	};

	// BGM������
	for (INT i = 0; i < MaxSound; i++)
	{
		if (!bgmFilePass[i].filePassPtr)
			break;
		
		std::pair<UINT, audiere::OutputStreamPtr> setPair = std::make_pair(i, audiere::OpenSound(audioDeviceObj.get(), bgmFilePass[i].filePassPtr));
		bgm.insert(setPair);
	}

	// SE�p�X�ݒ�
	std::array<AUDIO_DATA, MaxSound> seFilePass =
	{
		"data/SE/Star.wav"		,	// �X�^�[�A�C�e��
		"data/SE/Jump.wav"		,	// �W�����v
		"data/SE/StageClear.wav",	// �X�e�[�W�N���A
		"data/SE/landing.wav"	,	// ���n
		"data/SE/Hokou.wav"		,	// ���s
	};

	// SE������
	for (INT i = 0; i < MaxSound; i++)
	{
		if (!seFilePass[i].filePassPtr)
			break;

		std::pair<UINT, audiere::SoundEffectPtr> setPair = std::make_pair(i, audiere::OpenSoundEffect(audioDeviceObj.get(), seFilePass[i].filePassPtr, audiere::SoundEffectType::MULTIPLE));
		se.insert(setPair);
	}
}

//��������������������������������
// �f�X�g���N�^
//��������������������������������
MyAudiere::~MyAudiere()
{
	bgm.clear();
	se.clear();
}

//��������������������������������
// BGM�擾
//��������������������������������
audiere::OutputStreamPtr MyAudiere::getBgm(UINT index)
{
	return bgm[index];
}

//��������������������������������
// SE�擾
//��������������������������������
audiere::SoundEffectPtr MyAudiere::getSe(UINT index)
{
	return se[index];
}
