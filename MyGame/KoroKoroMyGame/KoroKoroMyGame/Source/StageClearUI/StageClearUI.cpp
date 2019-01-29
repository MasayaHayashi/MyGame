//����������������������������������������������
// StageClearUI.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "../StageClearUI/StageClearUI.h"
#include "../Application/Application.h"
#include "../Collision/Collision.h"
#include "../DirectX3D/DirectX3D.h"
#include "../GameManager/GameManager.h"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
StageClearUI::StageClearUI()
{
	strcpy_s(fileName, TextureFilePass.c_str());

	color					= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	vertexBoard.pos			= D3DXVECTOR3(Application::ScreenCenterX - 100.0f, Application::ScreenCenterY, 0.0f);
	vertexBoard.rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(1.0f, TextureSizeY, 0.0f);
	isAlphaBlend			= false;
	vertexBoard.boardType	= boardType::Polygon2d;
	vertexBoard.radAngle	= D3DXToRadian(0);

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	intervalChangePattern	= 1;

	isUsed					= true;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
StageClearUI::~StageClearUI()
{

}

//����������������������������������
// ������
//����������������������������������
void StageClearUI::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void StageClearUI::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void StageClearUI::update()
{
	if (GameManager::isGameType(GameManager::GameType::Goal))
	{
		vertexBoard.pos.x += 2.0f;

		if (vertexBoard.pos.x > Application::ScreenCenterX)
		{
			vertexBoard.pos.x = Application::ScreenCenterX;
		}

		vertexBoard.size.x += 10.0f;

		if (vertexBoard.size.x > TextureSizeX)
		{
			vertexBoard.size.x = TextureSizeX;
		}

		drawFlg = true;
	}

	setTexture();
	setVtx();
}

//��������������������������������������
// �`��
//��������������������������������������
void StageClearUI::draw()
{
	if (!drawFlg)
	{
		return;
	}
	Board::draw();
}