//����������������������������������������������
// Star.cpp
// Author : MasayaHayashi
//����������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Star/Star.h"
#include "../Application/Application.h"

// ===== �萔�E�}�N����` =====

//������������������������������������
// �R���X�g���N�^
//������������������������������������
Star::Star()
{
	strcpy_s(fileName, TextureFilePass.c_str());
	vertexBoard.pos			= D3DXVECTOR3(Application::ScreenCenterX * 0.5f, 0.0f, 0.0f);
	vertexBoard.rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vertexBoard.size		= D3DXVECTOR3(921.0f, 177.0f, 0.0f);
	vertexBoard.radAngle	= 0.0f;
	posDestBoard			= D3DXVECTOR3(Application::ScreenCenterX * 0.5f, Application::ScreenCenterY * 0.25f, 0.0f);
	isAlphaBlend			= false;
	vertexBoard.boardType	= boardType::Polygon2d;

	texPatternDivideX		= 1;
	texPatternDivideY		= 1;
	texUV_SizeX				= 1.0f / texPatternDivideX;
	texUV_SizeY				= 1.0f / texPatternDivideY;
	animPattern				= texPatternDivideX * texPatternDivideY;
	intervalChangePattern	= 1;

	isUsed = true;
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
Star::~Star()
{

}

//����������������������������������
// ������
//����������������������������������
void Star::initialize()
{
	ResourceManager::makevertexBoard(vertexBoard, fileName);
	ResourceManager::createTexture(texture, fileName);

	setVtx();
	setTexture();
}

//����������������������������������
// �^�C�g���I������
//����������������������������������
void Star::finalize()
{
	Board::finalize();
}

//������������������������������������
// �X�V
//������������������������������������
void Star::update()
{
	setVtx();
	setTexture();
}

//��������������������������������������
// �`��
//��������������������������������������
void Star::draw()
{
	Board::draw();
}