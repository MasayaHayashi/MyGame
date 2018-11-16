//����������������������������������������������������������������
// FadeUI.cpp
// �t�F�[�h�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "FadeUI.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../Scene/Load/SceneLoad.h"

// ===== �萔�E�}�N����` =====
#define TEXTURE_NAME		"Data/Texture/fade.png"
#define TEXTURE_NAME_STAR	"data/TEXTURE/Background.png"
#define FADE_RATE (0.02f)

//��������������������������������
// �R���X�g���N�^
//��������������������������������
FadeUI::FadeUI()
{
	vertexBoard.fade = true;
	CurentFadeType	= FadeType::FadeNone;
	Currentcolor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//Currentcolor	= D3DXCOLOR(0.0f, 0.635f, 0.91f, 0.0f);
	//color = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.0f);
	strcpy_s(fileName, &TextureName.front());
	vertexBoard.size		 = D3DXVECTOR3(static_cast<FLOAT>( Application::ScreenWidth), static_cast<FLOAT>(Application::ScreenHeight), 0.0f);
	vertexBoard.pos			 = D3DXVECTOR3(static_cast<FLOAT>( Application::ScreenWidth) * 0.5f, static_cast<FLOAT>(Application::ScreenHeight) * 0.5f, 0.0f);
	vertexBoard.scale		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.radAngle	 = D3DXToRadian(0);
	vertexBoard.boardType		= boardType::Polygon2d;
	isAlphaBlend = false;
	idNumber			 = 0;

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	currentAnimPattern		= 1;
	intervalChangePattern	= 7;

	isUsed = true;
}

//����������������������������������������������
// �f�X�g���N�^
//����������������������������������������������
FadeUI::~FadeUI()
{

}

//����������������������������������������������
// �ԍ�������
//����������������������������������������������
void FadeUI::initialize()
{
	strcpy_s(fileName, TEXTURE_NAME);
//	Texture.pD3DTexture = nullptr;
	//	CurentFadeType = FADE_IN;
	Currentcolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	ResourceManager::makevertexBoard(vertexBoard,fileName);	// ���_��񐶐�
	ResourceManager::createTexture(texture,fileName);		// �e�N�X�`������

	texture.pD3DTexture = nullptr;
}

//����������������������������������������������
// �ԍ��㏈��
//����������������������������������������������
void FadeUI::finalize()
{
//	Board::finalizeObject();
	
	// ���_�����
	ResourceManager::destroyFadeVtx();
}

//����������������������������������������������
// �t�F�[�h�X�V
//����������������������������������������������
void FadeUI::update()
{
	if (CurentFadeType != FadeUI::FadeType::FadeNone)
	{// �t�F�[�h������
		if (FadeUI::FadeType::FadeOut == CurentFadeType)
		{
			// �t�F�[�h�A�E�g����
			Currentcolor.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���

			if (Currentcolor.a >= 1.0f)
			{
				// �t�F�[�h�C�������ɐ؂�ւ�
				Currentcolor.a = 1.0f;

				// �V�[���㏈��
				SceneManager::finalize();
			//	SAFE_DELETE(pCurrentScene);

				SceneLoad::enable();
				setFade(FadeUI::FadeType::FadeIn);
			}

			// �F��ݒ�
			setcolor(Currentcolor);
		}
		else if (FadeUI::FadeType::FadeIn == CurentFadeType)
		{
			// �t�F�[�h�C������
			Currentcolor.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点�Ă���

			if (Currentcolor.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				Currentcolor.a = 0.0f;
				setFade(FadeUI::FadeType::FadeNone);
			}

			// �F��ݒ�
			setcolor(Currentcolor);
		}
	}
	setTexture();
	setVtx();
	setcolor(Currentcolor);
}

//����������������������������������������������
// �ԍ��`��
//����������������������������������������������
void FadeUI::draw()
{
	Board::draw();
}

//������������������������������������������������������������������������������
// �F��ݒ�
//������������������������������������������������������������������������������
void FadeUI::setcolor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	vertexBoard.pD3DVtxBuff->Unlock();
}

//������������������������������������������������������������������������������
// �t�F�[�h�̏�Ԑݒ�
//������������������������������������������������������������������������������
void FadeUI::setFade(FadeType fade)
{
	CurentFadeType = fade;
}

//������������������������������������������������������������������������������
// �t�F�[�h�̏�Ԏ擾
//������������������������������������������������������������������������������
FadeUI::FadeType FadeUI::getFadeState()
{
	return CurentFadeType;
}

/*
//����������������������������������������������������������
// ���[�N�p�V�[���Z�b�g
//����������������������������������������������������������
void FadeUI::setWorkScene(C_SCENE_MANAGER::SCENE_STATE setScene)
{
	WorkScene = setScene;
}
*/