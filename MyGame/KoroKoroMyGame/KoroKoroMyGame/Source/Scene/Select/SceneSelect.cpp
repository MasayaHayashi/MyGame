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

// ===== �ÓI�����o�ϐ� =====
UINT	SceneSelect::currentStage = 0;
UINT	SceneSelect::prevScore = 0;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneSelect::SceneSelect()
{
	prevScore = 0;
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

#if 0

	// �|�[�Y����
	if (uGameState == GAME_PAUSE)
	{
		// �|�[�Y�`��
		pPause->drawObject();
	}

	// �v���C���[�`��
	pPlayer->drawObject();

	// �Q�[���I�u�W�F�N�g�`��
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->drawObject(pCamera->getMtxView(),pCamera->getProjectionMtx());

	// �X�J�C�h�[���`��
	pSkydome->drawObject();

	// �p�[�e�B�N���`��
	for (INT TypeCnt = 0; TypeCnt < MAX_Star_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_Star; ObjCnt++)
			pStarObj[TypeCnt][ObjCnt]->drawObject();

	// �J�����Z�b�g
	pCamera->setCamera();

	// �X�R�A�`��
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
		pUIScore[i]->drawObject();

	// UI�`��
	for (INT ObjCnt = 0; ObjCnt < MAX_UI_OBJ_TYPE; ObjCnt++)
		pUIObj[ObjCnt]->drawObject();

	// �n�[�g�`��
	pHeart->drawObject();

	// �S�[���`��
	pGoal->drawObject();

#endif
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void SceneSelect::initializeStatus()
{
#if 0

	// BGM�Đ�
//	PlaySound(SOUND_BGM_MAIN);

	// �v���C���[�X�e�[�^�X������
	pPlayer->initializeStatus();

	// UI�g�p�t���O�X�V
	pUIObj[OBJ_MISS]->setUsedFlg(false);
	pUIObj[OBJ_BG]->setUsedFlg(false);

	for (INT i = 0; i < MAX_Star; i++)
		pStarObj[0][i]->setUsedFlg(false);

	// ���̔z�u���Ď擾
	pStageLoader->LoadStage(pGameObj, getCurrentStageNum());

	nReStartCnt = REStarT_CNT;

	// �X�R�A���Z�b�g
	pPlayer->setScore(0);
	
	// �Q�[����ԏ�����
	uGameState = GAME_READY;

	// READY�A�C�R��������
	pUIObj[OBJ_READY]->setUsedFlg(true);
	pUIObj[OBJ_READY]->setCnt(MAX_READY_CNT);
	pUIObj[OBJ_READY]->setCurrentAnimPattern(4);

	// �p�[�e�B�N��������
	for (INT i = 0; i < MAX_Star; i++)
		pStarObj[OBJ_2D_Star_Star][i]->initializeStatus();

	// �X�e�[�^�X������
	pUIObj[OBJ_MISS]->initializeStatus();

#endif
}

//��������������������������������������
// �J�����擾
//��������������������������������������
Camera* SceneSelect::getCamera()
{
	return cameraPtr.get();
}

//��������������������������������������
// ����֘A
//��������������������������������������
void SceneSelect::checkCollision()
{
#if 0

	// ��O����
	if (pPlayer->getState() == PlayerState::TYPE_JUMP_UP)
		return;

	// �u���b�N�Ƃ̔���
	uIsHitBlock = NONE;
	D3DXVECTOR3 Cross, Normal;

	INT nHitIndex = 0;
	D3DXVECTOR3 fLength = D3DXVECTOR3(0,0,0);




	// �ړ������ɕǂ����邩
	D3DXVECTOR3 velocityVec = pPlayer->getvelocityVec();
	D3DXVec3Normalize(&velocityVec, &velocityVec);

	// �u���b�N�Ƃ̔���
	uIsHitBlock = NONE;
	Cross = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	nHitIndex = 0;
	fLength = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



	if (pPlayer->getState() != PlayerState::TYPE_JUMP_DOWN)
	{
		for (INT i = 0; i < MAX_BLOCK_TYPE; i++)
		{
			for (INT j = 0; j < MAX_GAME_OBJ; j++)
			{
				// ��O����
				if (!pGameObj[i][j]->getUsedFlg())
					continue;

				// �v���C���[�ƃu���b�N�Ƃ̔���
				uIsHitBlock = pCollision->CheckCollisionWall(pPlayer, nullptr, pGameObj[i][j], Cross, Normal, fLength, velocityVec);

				nHitIndex = j;

				if (uIsHitBlock == RAY_TRUE)
					break;
			}
			if (uIsHitBlock == RAY_TRUE)
				break;
		}

		if (uIsHitBlock == RAY_TRUE)
		{
			if (fLength.x < 0.4f)
			{
				pPlayer->setStatus(PlayerState::TYPE_velocity_HIT_WALL);
				pPlayer->setPosition(pPlayer->getPosition() - pPlayer->getvelocityVec());
				PrintDebugProc("�Ă��Ƃ�������������������������������������������");
			}
		}

	}




	// ���Ƃ̔���
	if (pPlayer->getState() != (PlayerState::TYPE_velocity_HIT_WALL))
	{
		for (INT i = 0; i < MAX_BLOCK_TYPE; i++)
		{
			for (INT j = 0; j < MAX_GAME_OBJ; j++)
			{
				// ��O����
				if (!pGameObj[i][j]->getUsedFlg())
					continue;

				// �v���C���[�ƃu���b�N�Ƃ̔���
				uIsHitBlock = pCollision->CheckCollisionField(pPlayer, nullptr, pGameObj[i][j], Cross, Normal, fLength, D3DXVECTOR3(0.0f, -1.0f, 0.0f));
				nHitIndex = j;

				if (uIsHitBlock == RAY_TRUE)
					break;
			}
			if (uIsHitBlock == RAY_TRUE)
				break;
		}


		bool bSphereHit = false;

		FLOAT CheckY = pPlayer->getPosition().y - Cross.y;
		if (CheckY < 0.0f)
			CheckY *= -1;

		if (uIsHitBlock == RAY_TRUE)
		{
			if (fLength.x < 0.1f)
			{
				PrintDebugProc("�q�b�g�I�I�I�I�I�I�I�I�I�I�P");
				PrintDebugProc("hit%d", nHitIndex);
				pPlayer->setPosition(Cross - D3DXVECTOR3(0.0f, 0.01f, 0.0f));
				pPlayer->setStatus(PlayerState::TYPE_velocity);
			}

			if (fLength.x > 0.0f)
			{
				pPlayer->setStatus(PlayerState::TYPE_JUMP_DOWN);
			}
		}

		else if (uIsHitBlock == RAY_FALSE)
		{
			pPlayer->setStatus(PlayerState::TYPE_FALL);
			PrintDebugProc("�́[�Ђ��ƁI�I�I�I�I�I�I�I�I�I�P");

			PrintDebugProc("�́[�Ђ���");

		}
		else if (uIsHitBlock == NONE)
		{
			PrintDebugProc("�́[");
			pPlayer->setIsGround(false);
		}
	}
	PrintDebugProc("\nCross%f\n", Cross.x);
	PrintDebugProc("Cross%f\n", Cross.y);
	PrintDebugProc("Cross%f\n", Cross.z);






	// �A�C�e���Ƃ̔���
	for (INT i = 0; i < MAX_GAME_OBJ; i++)
	{
		// ��O����
		if (!pGameObj[Star_OBJ][i]->getUsedFlg())
			continue;

		// AABB�Փ˔���
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[Star_OBJ][i]))
		{
			pGameObj[Star_OBJ][i]->setUsedFlg(false);
			INT StarCnt = 0;

//			PlaySound(SOUND_SE_Star);	// ���Đ�

			/*
			// �p�[�e�B�N������
			for (INT StarCnt = 0; StarCnt < MAX_Star; StarCnt++)
			{
				if (pStarObj[OBJ_BILLBOARD_Star_Star][StarCnt]->getUsedFlg())
					continue;

				StarCnt++;
				pStarObj[OBJ_BILLBOARD_Star_Star][StarCnt]->setUsedFlg(true);
				pStarObj[OBJ_BILLBOARD_Star_Star][StarCnt]->setPosition(pGameObj[Star_OBJ][i]->getPosition());

				if (ONE_USE_PARTICLLE < StarCnt)
					break;
			}
			*/

			// 3D��2D���W�ϊ�
			checkUnProject(i);
		}
	}

	// �S�[������
	for (INT i = 0; i < MAX_GAME_OBJ; i++)
	{
		// ��O����
		if (!pGameObj[GOAL_OBJ][i]->getUsedFlg())
			continue;

		// AABB�Փ˔���
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[GOAL_OBJ][i]))
		{

			// �S�[���t���O�X�V
			if (uGameState != GAME_GOAL)
			{
//				PlaySound(SOUND_SE_STAGE_CLEAR);
				pCamera->setCameraState(MAIN_GOAL_FADE);
				uGameState = GAME_GOAL;
				AddStage();		// �X�e�[�W�����Z
		//		getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
			}
		}
	}

#endif
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