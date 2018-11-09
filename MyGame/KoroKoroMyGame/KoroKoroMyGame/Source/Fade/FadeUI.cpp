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
//#define TEXTURE_NAME		"data/TEXTURE/test.png"
#define TEXTURE_NAME_STAR	"data/TEXTURE/Background.png"
#define FADE_RATE (0.02f)

//��������������������������������
// �R���X�g���N�^
//��������������������������������
FadeUI::FadeUI()
{
	vertexBoard.fade = true;
	CurentFadeType	= FadeType::FadeNone;
	CurrentColor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//CurrentColor	= D3DXCOLOR(0.0f, 0.635f, 0.91f, 0.0f);
	//Color = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.0f);
	strcpy_s(fileName, &TextureName.front());
	vertexBoard.size		 = D3DXVECTOR3(Application::ScreenWidth, Application::ScreenHeight, 0.0f);
	vertexBoard.pos		 = D3DXVECTOR3(Application::ScreenWidth * 0.5f, Application::ScreenHeight * 0.5f, 0.0f);
	vertexBoard.scaleBoard		 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.radAngleBoard	 = D3DXToRadian(0);
	vertexBoard.boardType		= POLYGON_2D;
	alphaBlendBoard = false;
	number			 = 0;

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	currentAnimPattern		= 1;
	intervalChangePattern	= 7;

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
void C_FADE::initialize()
{
	strcpy_s(szFileName, TEXTURE_NAME);
//	Texture.pD3DTexture = nullptr;
	//	CurentFadeType = FADE_IN;
	CurrentColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

	ResourceManager::makevertexBoard(vertexBoard,szFileName);	// ���_��񐶐�
	ResourceManager::CreateTexture(Texture,szFileName);		// �e�N�X�`������

	Texture.pD3DTexture = nullptr;
}

//����������������������������������������������
// �ԍ��㏈��
//����������������������������������������������
void C_FADE::finalizeObject()
{
//	C_BOARD::finalizeObject();
	
	// ���_�����
	ResourceManager::destroyFadeVtx();
}

//����������������������������������������������
// �t�F�[�h�X�V
//����������������������������������������������
void C_FADE::updateObject()
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

				C_SCENE_MANAGER *pSceneManager = getSceneManager();

				// �V�[���㏈��
				pSceneManager->finalizeScene();
			//	SAFE_DELETE(pcurrentScene);


				// ���[�h�J�n
				C_SCENE_LOAD *pSceneLoad = getSceneLoad();
				pSceneLoad->EnableLoad();
				setFade(FADE_IN);
			}

			// �F��ݒ�
			setColor(CurrentColor);
		}
		else if (FADE_IN == CurentFadeType)
		{
			// �t�F�[�h�C������
			CurrentColor.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点�Ă���

			if (CurrentColor.a <= 0.0f)
			{
				// �t�F�[�h�����I��
				CurrentColor.a = 0.0f;
				setFade(FADE_NONE);
			}

			// �F��ݒ�
			setColor(CurrentColor);
		}
	}
	//setTexture();
	setvtxBoard();
	setColor(CurrentColor);
}

//����������������������������������������������
// �ԍ��`��
//����������������������������������������������
void C_FADE::drawObject()
{
	C_BOARD::drawObject();
}


//������������������������������������������������������������������������������
// �F��ݒ�
//������������������������������������������������������������������������������
void C_FADE::setColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	vertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	vertexBoard.pD3DVtxBuffBoard->Unlock();
}

//������������������������������������������������������������������������������
// �t�F�[�h�̏�Ԑݒ�
//������������������������������������������������������������������������������
void C_FADE::setFade(FADE_TYPE fade)
{
	CurentFadeType = fade;
}

//������������������������������������������������������������������������������
// �t�F�[�h�̏�Ԏ擾
//������������������������������������������������������������������������������
C_FADE::FADE_TYPE C_FADE::getFadeState()
{
	return CurentFadeType;
}

//����������������������������������������������������������
// ���[�N�p�V�[���Z�b�g
//����������������������������������������������������������
void C_FADE::setWorkScene(C_SCENE_MANAGER::SCENE_STATE setScene)
{
	WorkScene = setScene;
}