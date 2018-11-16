//������������������������������������������������������������������������������������������������������
// C_SceneTitle.cpp
// Author    : MasayaHayashi
//������������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Main/SceneMain.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"

/*
#include "C_Board.h"
#include "C_Pause.h"
#include "C_Player.h"
#include "C_Enemy.h"
#include "C_Light.h"
#include "C_Camera.h"
#include "C_Skydome.h"
#include "C_MoveBlock.h"
#include "C_GoalObj.h"
#include "C_Pause.h"
#include "C_ItemStar.h"
#include "input.h"
#include "C_Ready.h"
#include "C_StarParticle.h"
#include "C_MissUI.h"
#include "C_GoalUI.h"
#include "C_StageLoader.h"
#include "C_BgEffect.h"
#include "C_ScoreUi.h"
#include "C_StarUI.h"
#include "C_Goal.h"
#include "C_TutorialUI.h"
#include "C_HeartBoard.h"
#include "C_NextUI.h"
#include "C_Audio.h"

#if _DEBUG
	#include "debugproc.h"
#endif
*/

// ===== �ÓI�����o�ϐ� =====
UINT	SceneMain::currentStage = 0;
UINT	SceneMain::prevScore = 0;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneMain::SceneMain()
{
	prevScore = 0;
	lightPtr.reset(NEW Light);
	cameraPtr.reset(NEW Camera);
	playerPtr.reset(NEW Player);
}

//������������������������������������
// �f�X�g���N�^
//������������������������������������
SceneMain::~SceneMain()
{
	
}

//����������������������������������
// ������
//����������������������������������
void SceneMain::initialize()
{
	lightPtr->initialize();
	cameraPtr->initializeMain(playerPtr.get());

	/*
	// �v���C���[������
	pPlayer = NEW Player;
	pPlayer->initialize();

	// ���C�g������
	pLight = NEW Light;
	pLight->initializeLight();

	// �J����������
	pCamera = NEW C_CAMERA;
	pCamera->initializeCamera(pPlayer);

	// �X�J�C�h�[��������
	pSkydome = NEW C_SKYDOME;
	pSkydome->initialize();

	// �p�[�e�B�N��������
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
		{
			switch (TypeCnt)
			{
				case OBJ_BILLBOARD_STAR_PARTICLE:
					pParticleObj[TypeCnt][ObjCnt] = NEW C_STAR_PARTICLE();
					break;
				case OBJ_2D_STAR_PARTICLE:
					pParticleObj[TypeCnt][ObjCnt] = NEW C_PARTICLE_BASE(ObjCnt);
					break;
				default:
					break;
			}
			pParticleObj[TypeCnt][ObjCnt]->initialize();
		}

	// UI������
	for (INT UiTypeCnt = 0; UiTypeCnt < MAX_UI_OBJ_TYPE; UiTypeCnt++)
	{
		switch (UiTypeCnt)
		{
		case OBJ_READY :
			pUIObj[UiTypeCnt] = NEW C_READY();
			break;
		case OBJ_MISS :
			pUIObj[UiTypeCnt] = NEW C_MISS_UI();
			break;
		case OBJ_BG:
			pUIObj[UiTypeCnt] = NEW C_BG_EFFECT();
			break;
		case OBJ_STAR:
			pUIObj[UiTypeCnt] = NEW C_STAR_UI();
			break;
		case OBJ_TUTORIAL:
			pUIObj[UiTypeCnt] = NEW C_TUTORIAL_UI();
			break;
		case OBJ_NEXT:
			pUIObj[UiTypeCnt] = NEW C_NEXT_UI();
			break;
		default:
			break;
		}
		pUIObj[UiTypeCnt]->initialize();
	}

	// �Q�[���I�u�W�F�N�g������
	for (INT BlockType = 0; BlockType < MAX_GAME_OBJ_TYPE; BlockType++)
		for (INT i = 0; i < MAX_GAME_OBJ; i++)
		{
			switch (BlockType)
			{
			case NORMAL_BLOCK_OBJ:
				pGameObj[BlockType][i] = NEW C_BLOCK(i);
				break;
			case MOVE_BLOCK_OBJ:
				pGameObj[BlockType][i] = NEW C_MOVE_BLOCK(i);
				break;
			case STAR_OBJ:
				pGameObj[BlockType][i] = NEW C_ITEM_STAR();
				break;
			case GOAL_OBJ:
				pGameObj[BlockType][i] = NEW C_GOAL_OBJ();
				break;				
			default:
				break;
			}
			pGameObj[BlockType][i]->initialize();
		}

	// �Փ˗p�N���X����
	pCollision = NEW C_COLLISION;

	// �|�[�Y�p�I�u�W�F�N�g������
	pPause = NEW C_PAUSE;
	pPause->initialize();

	// �X�e�[�W�ǂݍ��ݏ�����
	pStageLoader = NEW C_STAGE_LOADER;
	pStageLoader->LoadStage(pGameObj, getCurrentStageNum());

	// �X�R�A������
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		pUIScore[i] = NEW C_SCORE_UI(i);
		pUIScore[i]->initialize();
	}

	// �S�[��������
	pGoal = NEW C_GOAL;
	pGoal->initialize();

	// �n�[�g������
	pHeart = NEW C_HEART_BOARD;
	pHeart->initialize();

	// �Q�[����ԏ�����
	uGameState  = GAME_TUTORIAL;

	nRestartCnt = RESTART_CNT;

	/*
	// BGM�Đ�
	auto Bgm = C_AUDIO::getBgm(AUDIO::BGM_MAIN);

	
	float vol = 0.3f;
	Bgm->setVolume(vol);

	Bgm->play();
	*/
}

//����������������������������������
// �㏈��
//����������������������������������
void SceneMain::finalize()
{
	/*
	// BGM��~

	// �J�����㏈��
	pCamera->finalizeCamera();
	SAFE_DELETE(pCamera);

	// ���C�g�㏈��
	pLight->finalizeLight();
	SAFE_DELETE(pLight);

	// �v���C���[�㏈��
	pPlayer->finalizeObject();
	SAFE_DELETE(pPlayer);

	// �X�J�C�h�[���㏈��
	pSkydome->finalizeObject();
	SAFE_DELETE(pSkydome);

	// �Փ˗p�㏈��
	SAFE_DELETE(pCollision);

	// �|�[�Y�㏈��
	pPause->finalizeObject();
	SAFE_DELETE(pPause);

	// �p�[�e�B�N���㏈��
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
		{
			pParticleObj[TypeCnt][ObjCnt]->finalizeObject();
			SAFE_DELETE(pParticleObj[TypeCnt][ObjCnt]);
		}


	// �Q�[���I�u�W�F�N�g�㏈��
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
		{
			pGameObj[i][j]->finalizeObject();
			SAFE_DELETE(pGameObj[i][j]);
		}

	// UI�㏈��
	for (INT TypeCnt = 0; TypeCnt < MAX_UI_OBJ_TYPE; TypeCnt++)
	{
		pUIObj[TypeCnt]->finalizeObject();
		SAFE_DELETE(pUIObj[TypeCnt]);
	}

	// �X�R�A�㏈��
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		pUIScore[i]->finalizeObject();
		SAFE_DELETE(pUIScore[i]);
	}

	// �X�e�[�W���[�_�[�㏈��
	pStageLoader->finalizeObject();
	SAFE_DELETE(pStageLoader);

	// �S�[���㏈��
	pGoal->finalizeObject();
	SAFE_DELETE(pGoal);

	// �n�[�g�㏈��
	pHeart->finalizeObject();
	SAFE_DELETE(pHeart);
	*/
}

//������������������������������������
// �X�V
//������������������������������������
void SceneMain::update()
{



#if 0
	if (currentGameState == SceneMain::GameState::Tutorial)
	{
		cameraPtr->setState(Camera::MoceType::Tutorial);
	}


	// �S�[������ڍs����
	if (uGameState == GAME_GOAL)
	{
		bool bAllUse = true;
		for (INT i = 0; i < MAX_PARTICLE; i++)
		{
			if (pParticleObj[OBJ_2D_STAR_PARTICLE][i]->getUsedFlg())
			{
				bAllUse = false;
				break;
			}
		}
		if (bAllUse)
			getSceneManager()->setSceneChange(C_SCENE_MANAGER::SCENE_MAIN);
	}

	// �n�[�g�X�V
	pHeart->updateObject(pPlayer->getOffset());
	
	// �|�[�Y��������
	if (getKeyboardTrigger(DIK_P))
	{
		if (uGameState == GAME_NORMAL)
			uGameState = GAME_PAUSE;
		if (uGameState == GAME_PAUSE)
			uGameState = GAME_NORMAL;
	}

	// �ăX�^�[�g
	if (uGameState == GAME_MISS)
	{
		nRestartCnt--;
		if (nRestartCnt < 0)
			initializeStatus();
	}

	// UI�X�V
	for (INT UiTypeCnt = 0; UiTypeCnt < MAX_UI_OBJ_TYPE; UiTypeCnt++)
		pUIObj[UiTypeCnt]->updateObject();

	// �X�R�A�X�V
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
		pUIScore[i]->updateObject();
	
	// UI�X�V
	if (uGameState == GAME_MISS)
	{
		pUIObj[OBJ_MISS]->setUsedFlg(true);
		pUIObj[OBJ_BG]->setUsedFlg(true);
	}

	// �v���C���[�X�V
	pPlayer->updatePlayer_GameMain(pCamera->getCameraFowerd());

	// �Q�[����ԍX�V
	if (!pUIObj[OBJ_READY]->getUsedFlg() && uGameState != GAME_MISS && uGameState != GAME_GOAL && uGameState != GAME_TUTORIAL)
		uGameState = GAME_NORMAL;

	// �|�[�Y�X�V
	if (uGameState == GAME_PAUSE)
	{
		pPause->updateObject();
		return;
	}
	if (uGameState == GAME_MISS)
		return;

	// ���S����
	if (pPlayer->getState() == PlayerState::TYPE_DEAD)
		uGameState = GAME_MISS;

	// �X�J�C�h�[���X�V
	pSkydome->updateObject();

	// �p�[�e�B�N���X�V����
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
			pParticleObj[TypeCnt][ObjCnt]->updateObject();

	// �Q�[���I�u�W�F�N�g�X�V
	for (INT i = 0; i < MAX_GAME_OBJ_TYPE; i++)
		for (INT j = 0; j < MAX_GAME_OBJ; j++)
			pGameObj[i][j]->updateObject();
	
	// �����蔻��`�F�b�N
	if (uGameState == GAME_NORMAL || uGameState == GAME_GOAL)
	{
		checkCollision();
	}

#if _DEBUG
	PrintDebugProc("�Q�[���X�e�[�g��%d\n", uGameState);
#endif

	// �X�R�A���Z
	for (INT i = 0; i < MAX_PARTICLE; i++)
	{
		if (!pParticleObj[OBJ_2D_STAR_PARTICLE][i]->getUsedFlg())
			continue;

		if (pParticleObj[OBJ_2D_STAR_PARTICLE][i]->getCurveCnt() >= 1.0f)
		{
			pPlayer->addScore();
			pParticleObj[OBJ_2D_STAR_PARTICLE][i]->setCnt(0.0f);
			pParticleObj[OBJ_2D_STAR_PARTICLE][i]->setUsedFlg(false);
		}
	}

	INT nScore = pPlayer->getScore();
	INT nDigit;

	// �X�R�A�Z�b�g
	for (INT i = 0; i < MAX_SCORE_DIGIT; i++)
	{
		nDigit = nScore % 10;
		pUIScore[i]->setCurrentAnimPattern(nDigit);
		nScore /= 10;
	}

	// �S�[���X�V
	pGoal->updateObject();

	// �J�����X�V
	pCamera->updateCamera_GameMain(pPlayer,pUIObj[OBJ_READY]);

#endif
}

//��������������������������������������
// �`��
//��������������������������������������
void SceneMain::draw()
{

	playerPtr->draw();

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
	for (INT TypeCnt = 0; TypeCnt < MAX_PARTICLE_OBJ_TYPE; TypeCnt++)
		for (INT ObjCnt = 0; ObjCnt < MAX_PARTICLE; ObjCnt++)
			pParticleObj[TypeCnt][ObjCnt]->drawObject();

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
void SceneMain::initializeStatus()
{
#if 0

	// BGM�Đ�
//	PlaySound(SOUND_BGM_MAIN);

	// �v���C���[�X�e�[�^�X������
	pPlayer->initializeStatus();

	// UI�g�p�t���O�X�V
	pUIObj[OBJ_MISS]->setUsedFlg(false);
	pUIObj[OBJ_BG]->setUsedFlg(false);

	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticleObj[0][i]->setUsedFlg(false);

	// ���̔z�u���Ď擾
	pStageLoader->LoadStage(pGameObj, getCurrentStageNum());

	nRestartCnt = RESTART_CNT;

	// �X�R�A���Z�b�g
	pPlayer->setScore(0);
	
	// �Q�[����ԏ�����
	uGameState = GAME_READY;

	// READY�A�C�R��������
	pUIObj[OBJ_READY]->setUsedFlg(true);
	pUIObj[OBJ_READY]->setCnt(MAX_READY_CNT);
	pUIObj[OBJ_READY]->setCurrentAnimPattern(4);

	// �p�[�e�B�N��������
	for (INT i = 0; i < MAX_PARTICLE; i++)
		pParticleObj[OBJ_2D_STAR_PARTICLE][i]->initializeStatus();

	// �X�e�[�^�X������
	pUIObj[OBJ_MISS]->initializeStatus();

#endif
}

//��������������������������������������
// �J�����擾
//��������������������������������������
Camera* SceneMain::getCamera()
{
	return cameraPtr.get();
}

//��������������������������������������
// ����֘A
//��������������������������������������
void SceneMain::checkCollision()
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
	D3DXVECTOR3 MoveVec = pPlayer->getMoveVec();
	D3DXVec3Normalize(&MoveVec, &MoveVec);

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
				uIsHitBlock = pCollision->CheckCollisionWall(pPlayer, nullptr, pGameObj[i][j], Cross, Normal, fLength, MoveVec);

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
				pPlayer->setStatus(PlayerState::TYPE_MOVE_HIT_WALL);
				pPlayer->setPosition(pPlayer->getPosition() - pPlayer->getMoveVec());
				PrintDebugProc("�Ă��Ƃ�������������������������������������������");
			}
		}

	}




	// ���Ƃ̔���
	if (pPlayer->getState() != (PlayerState::TYPE_MOVE_HIT_WALL))
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
				pPlayer->setStatus(PlayerState::TYPE_MOVE);
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
		if (!pGameObj[STAR_OBJ][i]->getUsedFlg())
			continue;

		// AABB�Փ˔���
		if (pCollision->IsHitAABBItem(pPlayer, pGameObj[STAR_OBJ][i]))
		{
			pGameObj[STAR_OBJ][i]->setUsedFlg(false);
			INT particleCnt = 0;

//			PlaySound(SOUND_SE_STAR);	// ���Đ�

			/*
			// �p�[�e�B�N������
			for (INT ParticleCnt = 0; ParticleCnt < MAX_PARTICLE; ParticleCnt++)
			{
				if (pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->getUsedFlg())
					continue;

				particleCnt++;
				pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->setUsedFlg(true);
				pParticleObj[OBJ_BILLBOARD_STAR_PARTICLE][ParticleCnt]->setPosition(pGameObj[STAR_OBJ][i]->getPosition());

				if (ONE_USE_PARTICLLE < particleCnt)
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
void SceneMain::checkUnProject(INT Indx)
{
#if 0

	D3DXVECTOR3 StarObj = pGameObj[STAR_OBJ][Indx]->getPosition();
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

	INT UseParticleCnt = 0;

	for (INT ParticleCnt = 0; ParticleCnt < MAX_PARTICLE; ParticleCnt++)
	{
		// ��O����
		if (pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->getUsedFlg())
			continue;

		UseParticleCnt ++;

		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->setUsedFlg(true);
		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->setPosition(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));
		pParticleObj[OBJ_2D_STAR_PARTICLE][ParticleCnt]->setStartCurvePos(D3DXVECTOR3(OutVec2.x, OutVec2.y, 0.0f));

		if (UseParticleCnt > ONE_USE_PARTICLLE)
			break;
	}

#endif
}

//����������������������������������������
// �X�R�A�Z�b�g
//����������������������������������������
void SceneMain::setScore()
{

}