//����������������������������������������������������������������
// C_FADE.cpp
// �t�F�[�h�N���X
// Author : Masaya Hayashi
//����������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "FadeUI.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../Scene/Load/SceneLoad.h"

// ===== �萔�E�}�N����` =====
#define TEXTURE_NAME		"data/TEXTURE/test.png"
#define TEXTURE_NAME_STAR	"data/TEXTURE/Background.png"
#define FADE_RATE (0.02f)

//��������������������������������
// �R���X�g���N�^
//��������������������������������
FadeUI::FadeUI()
{
	VertexBoard.bFade = true;
	CurentFadeType	= FadeType::FadeNone;
	CurrentColor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//CurrentColor	= D3DXCOLOR(0.0f, 0.635f, 0.91f, 0.0f);
	//Color = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.0f);
	strcpy_s(szFileName, TEXTURE_NAME);
	VertexBoard.SizeBoard		 = D3DXVECTOR3(Application::ScreenWidth, Application::ScreenHeight, 0.0f);
	VertexBoard.PosBoard		 = D3DXVECTOR3(Application::ScreenWidth * 0.5f, Application::ScreenHeight * 0.5f, 0.0f);
	VertexBoard.ScaleBoard		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	VertexBoard.fRadAngleBoard	 = D3DXToRadian(0);
	VertexBoard.BoardType		= POLYGON_2D;
	bAlphaBlendBoard = false;
	uNumber			 = 0;

	TexPatternDivideX		= 1;
	TexPatternDivideY		= 1;
	TexUV_SizeX				= 1.0f / TexPatternDivideX;
	TexUV_SizeY				= 1.0f / TexPatternDivideY;
	NumAnimPattern			= TexPatternDivideX * TexPatternDivideY;
	CurrentAnimPattern		= 1;
	nIntervalChangePattern	= 7;

	bUsed = true;
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
void C_FADE::InitObject()
{
	strcpy_s(szFileName, TEXTURE_NAME);
//	Texture.pD3DTexture = NULL;
	//	CurentFadeType = FADE_IN;
	CurrentColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	C_RESOURCE_MANAGER::MakeVertexBoard(VertexBoard,szFileName);	// ���_��񐶐�
	C_RESOURCE_MANAGER::CreateTexture(Texture,szFileName);		// �e�N�X�`������

	Texture.pD3DTexture = NULL;
}

//����������������������������������������������
// �ԍ��㏈��
//����������������������������������������������
void C_FADE::UninitObject()
{
//	C_BOARD::UninitObject();
	
	// ���_�����
	C_RESOURCE_MANAGER::DestroyFadeVtx();
}

//����������������������������������������������
// �t�F�[�h�X�V
//����������������������������������������������
void C_FADE::UpdateObject()
{
	if (CurentFadeType != FADE_NONE)
	{// �t�F�[�h������
		if (FADE_OUT == CurentFadeType)
		{
			// �t�F�[�h�A�E�g����
			CurrentColor.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���

			if (CurrentColor.a >= 1.0f)
			{
				// �t�F�[�h�C�������ɐ؂�ւ�
				CurrentColor.a = 1.0f;

				C_SCENE_MANAGER *pSceneManager = GetSceneManager();

				// �V�[���㏈��
				pSceneManager->UninitScene();
			//	SAFE_DELETE(pCurrentScene);


				// ���[�h�J�n
				C_SCENE_LOAD *pSceneLoad = GetSceneLoad();
				pSceneLoad->EnableLoad();
				SetFade(FADE_IN);
			}

			// �F��ݒ�
			SetColor(CurrentColor);
		}
		else if (FADE_IN == CurentFadeType)
		{
			// �t�F�[�h�C������
			CurrentColor.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点�Ă���

			if (CurrentColor.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				CurrentColor.a = 0.0f;
				SetFade(FADE_NONE);
			}

			// �F��ݒ�
			SetColor(CurrentColor);
		}
	}
	//SetTexture();
	SetVtxBoard();
	SetColor(CurrentColor);
}

//����������������������������������������������
// �ԍ��`��
//����������������������������������������������
void C_FADE::DrawObject()
{
	C_BOARD::DrawObject();
}


//������������������������������������������������������������������������������
// �F��ݒ�
//������������������������������������������������������������������������������
void C_FADE::SetColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	VertexBoard.pD3DVtxBuffBoard->Unlock();
}

//������������������������������������������������������������������������������
// �t�F�[�h�̏�Ԑݒ�
//������������������������������������������������������������������������������
void C_FADE::SetFade(FADE_TYPE fade)
{
	CurentFadeType = fade;
}

//������������������������������������������������������������������������������
// �t�F�[�h�̏�Ԏ擾
//������������������������������������������������������������������������������
C_FADE::FADE_TYPE C_FADE::GetFadeState()
{
	return CurentFadeType;
}

//����������������������������������������������������������
// ���[�N�p�V�[���Z�b�g
//����������������������������������������������������������
void C_FADE::SetWorkScene(C_SCENE_MANAGER::SCENE_STATE SetScene)
{
	WorkScene = SetScene;
}