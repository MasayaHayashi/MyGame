//������������������������������������������������������������������������������������������������������
// SceneSelect.cpp
// Author    : MasayaHayashi
//������������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Select/SceneSelect.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../Ball/BallObj.h"
#include "../../Skydome/Skydome.h"
#include "../../MyDelete/MyDelete.h"
#include "../../Board/Board.h"
#include "../../BackGround/BackGround.h"
#include "../../SelectManager/SelectManager.h"

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneSelect::SceneSelect()
{
	lightPtr.reset(	NEW Light() );
	cameraPtr.reset(NEW Camera());

	for (UINT playerIndex = 1; playerIndex < SelectManager::MaxPlayer + 1; playerIndex++)
	{
		playeresPtr.push_back(static_cast<std::unique_ptr<Player>> (NEW Player(D3DXVECTOR3( -2.0f * playerIndex, 0.0f, 0.0f), playeresPtr.size())) );
	}

	boardObjectesPtr.push_back(std::unique_ptr<Board>( NEW BackGround()) );
	gameObjectesPtr.push_back(  std::unique_ptr<Pawn>( NEW Skydome())    );
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneSelect::~SceneSelect()
{

}

//����������������������������������
// ������
//����������������������������������
void SceneSelect::initialize()
{
	lightPtr->initialize();
	cameraPtr->initializeTitle();

	for (const auto &player : playeresPtr)
	{
		player->initialize();
	}

	for (const auto &gameObject : gameObjectesPtr)
	{
		gameObject->initialize();
	}

	for (const auto& board : boardObjectesPtr)
	{
		board->initialize();
	}
}

//����������������������������������
// �㏈��
//����������������������������������
void SceneSelect::finalize()
{
	
	for (const auto &player : playeresPtr)
	{
		player->finalize();
	}
	
}

//������������������������������������
// �X�V
//������������������������������������
void SceneSelect::update()
{
	for (const auto &gameObject : gameObjectesPtr)
	{
		gameObject->update();
	}

	for (const auto &player : playeresPtr)
	{
		player->update(cameraPtr->getFowerd());
	}

	for (const auto& board : boardObjectesPtr)
	{
		board->update();
	}

	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}
}

//��������������������������������������
// �`��
//��������������������������������������
void SceneSelect::draw()
{
	for (const auto &gameObject : gameObjectesPtr)
	{
		if (gameObject->isUsedShader())
		{
			gameObject->draw(cameraPtr->getMtxView(), cameraPtr->getProjectionMtx());
		}
		else
		{
			gameObject->draw();
		}
	}

	for (const auto &player : playeresPtr)
	{
		player->draw();
	}

	for (const auto& board : boardObjectesPtr)
	{
		board->draw();
	}

	cameraPtr->setCamera();
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void SceneSelect::initializeStatus()
{

}

//��������������������������������������
// ����֘A
//��������������������������������������
void SceneSelect::checkCollision()
{

}

//������������������������������������������������
// 3D����2D�ւ̕ϊ��v�Z�p
//������������������������������������������������
void SceneSelect::checkUnProject(INT Indx)
{
#if 0

	D3DXVECTOR3 StarObj = pGameObj[Star_OBJ][Indx]->getPosition();
	D3DXMATRIX  ViewMtx = pCamera->getMtxView();
	D3DXMATRIX  ProjectionMtx = pCamera->getProjectionMtx();

	D3DXVECTOR3 OutVec;
	D3DXVECTOR3 OutVec2;

	D3DXVec3TransformCoord(&OutVec, &StarObj, &ViewMtx);
	D3DXVec3TransformCoord(&OutVec2, &OutVec, &ProjectionMtx);

	OutVec2.y *= -1;

	OutVec2.x *= 0.5f;
	OutVec2.y *= 0.5f;

	OutVec2.x += 0.5f;
	OutVec2.y += 0.5f;

	OutVec2.x *= SCREEN_WIDTH;
	OutVec2.y *= SCREEN_HEIGHT;

	INT UseStarCnt = 0;

	for (INT StarCnt = 0; StarCnt < MAX_Star; StarCnt++)
	{
		// ��O����
		if (pStarObj[OBJ_2D_Star_Star][StarCnt]->getUsedFlg())
			continue;

		UseStarCnt ++;

		pStarObj[OBJ_2D_Star_Star][StarCnt]->setUsedFlg(true);
		pStarObj[OBJ_2D_Star_Star][StarCnt]->setPosition(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));
		pStarObj[OBJ_2D_Star_Star][StarCnt]->setStartCurvePos(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));

		if (UseStarCnt > ONE_USE_PARTICLLE)
			break;
	}

#endif
}

//����������������������������������������
// �X�R�A�Z�b�g
//����������������������������������������
void SceneSelect::setScore()
{

}