//������������������������������������������������������������������������������������������������������
// C_SceneTitle.cpp
// Author    : MasayaHayashi
//������������������������������������������������������������������������������������������������������

// ===== �C���N���[�h�� =====
#include "../Main/SceneMain.h"
#include "../../Light/Light.h"
#include "../../Camera/Camera.h"
#include "../../Player/Player.h"
#include "../../Skydome/Skydome.h"
#include "../../MainField/MainField.h"
#include "../../Collision/Collision.h"
#include "../../Ball/BallObj.h"
#include "../../MyDelete/MyDelete.h"
#include "../../StarParticle/StarParticle.h"
#include "../../MainObject/MainObject.h"
#include "../../Countdown/Countdown.h"
#include "../../Goal/Goal.h"
#include "../../StageClearUI/StageClearUI.h"
#include "../../MissUI/MissUI.h"
#include "../../Audio/MyAudiere.h"
#include "../../GameManager/GameManager.h"
#include "../../SpikeBlock/SpikeBlock.h"
#include "../../HeartObj/HeartObj.h"
#include "../../StarItem/StarItem.h"
#include <fstream>

/*
#include "C_Board.h"
#include "C_Pause.h"
#include "C_Player.h"
#include "C_Enemy.h"
#include "C_Light.h"
#include "C_Camera.h"
#include "C_Skydome.h"
#include "C_velocityBlock.h"
#include "C_GoalObj.h"
#include "C_Pause.h"
#include "C_ItemStar.h"
#include "input.h"
#include "C_Ready.h"
#include "C_StarStar.h"
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
UINT	SceneMain::prevScore	= 0;

//������������������������������������
// �R���X�g���N�^
//������������������������������������
SceneMain::SceneMain()
{
	prevScore = 0;
	lightPtr.reset( NEW Light());
	cameraPtr.reset(NEW Camera());

	playeresPtr.push_back(static_cast<std::unique_ptr<Player>>(NEW Player(D3DXVECTOR3(-2.0f, 0.0f, 0.0f), playeresPtr.size() )));

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		creteGameObj(gameObjTypeIndex);
	}

	skyDomePtr.push_back(		std::unique_ptr<Skydome>(	NEW Skydome())	);
	
	for (int index = 0; index < StarParticle::MaxParticle; index++)
	{
		boardObjectesPtr.push_back(std::unique_ptr<Board>(  NEW StarParticle(index) ));
	}

	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW StageClearUI()  ));
	boardObjectesPtr.push_back( std::unique_ptr<Board>(		NEW MissUI()	    ));
	boardObjectesPtr.push_back(std::unique_ptr<Board>(		NEW Countdown()));

	collisionPtr.reset( NEW Collision() );

	for (auto& player : playeresPtr)
	{
		collisionPtr->registerPlayer(player.get());
	}

	for (size_t gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType++)
	{
		for (auto gameObj : gameObjPtr[gameObjType])
		{
			if (gameObj->getGameObjectType() == GameObjectType::NormalBlockObj || 
				gameObj->getGameObjectType() == GameObjectType::SpikeObj)
			{
				collisionPtr->registerBlock(*gameObj);
			}
		}
	}

	for (size_t gameObjType = 0; gameObjType < GameObjectBase::MaxGameObjType; gameObjType ++)
	{
		for (auto gameObj : gameObjPtr[gameObjType])
		{
			if (gameObj->getIsFieldObject())
			{
				collisionPtr->registerField(gameObj);
			}
		}
	}
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
	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->initialize();
		}
	}

	loadStageData(1);
	
	lightPtr->initialize();

	playeresPtr.front()->initialize();
	
	cameraPtr->initializeMain(playeresPtr.back().get());

	for (const auto &skyDome : skyDomePtr)
	{
		skyDome->initialize();
	}

	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->initialize();
	}

	MyAudiere::getBgm(0)->setRepeat(true);
	MyAudiere::getBgm(0)->setVolume(0.1f);
	MyAudiere::getBgm(0)->play();
}

//����������������������������������
// �㏈��
//����������������������������������
void SceneMain::finalize()
{
	for (const auto &player : playeresPtr)
	{
		player->finalize();
	}

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->finalize();
		}
	}

	collisionPtr->finalize("Player");
}

//������������������������������������
// �X�V
//������������������������������������
void SceneMain::update()
{
	if (Keyboard::getTrigger(DIK_2))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneTitle);
	}

	for (const auto &skyDome : skyDomePtr)
	{
		skyDome->update();
	}

	for (const auto &player : playeresPtr)
	{
		player->update(cameraPtr->getFowerd());
	}
	
	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->update();
	}

	for (UINT gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->update();
		}
	}

	cameraPtr->update(*playeresPtr.front().get(), boardObjectesPtr.back()->getCurrentAnim());
	
	if (Keyboard::getPress(DIK_1))
	{
		SceneManager::setNextScene(SceneManager::SceneState::SceneMain);
	}

	collisionPtr->update();

	if (GameManager::isGameType(GameManager::GameType::Miss))
	{
		restartCnt ++;

		if (restartCnt > RestartFream)
		{
			initializeStatus();
			GameManager::changeGameType(GameManager::GameType::Ready);
			restartCnt = 0;
		}
	}
}

//��������������������������������������
// �`��
//��������������������������������������
void SceneMain::draw()
{
	for (const auto &gameObject : skyDomePtr)
	{
		gameObject->draw();
	}

	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->draw();
	}

	playeresPtr.front()->draw();

	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			(*itr)->draw();
		}
	}

	cameraPtr->setCamera();
}

//��������������������������������������
// �X�e�[�^�X������
//��������������������������������������
void SceneMain::initializeStatus()
{
	// BGM�Đ�
//	PlaySound(SOUND_BGM_MAIN);

	// �v���C���[�X�e�[�^�X������
	playeresPtr.front()->initializeStatus();

	for (const auto &boardObject : boardObjectesPtr)
	{
		boardObject->initializeStatus();
	}

	collisionPtr->initilize("Player", 0);
	collisionPtr->initilize();

	cameraPtr->initializeStatus();

	for (const auto &gameObj : gameObjPtr)
	{
		for (const auto &list : gameObj)
		{
			if (list->getUsedFlg() && 
				list->getGameObjectType() == GameObjectType::SpikeObj)
			{
				list->initializeStatus();
			}
		}
	}

}

//��������������������������������������
// �J�����擾
//��������������������������������������
Camera* SceneMain::getCamera()
{
	return cameraPtr.get();
}

//������������������������������������������������
// 3D����2D�ւ̕ϊ��v�Z�p
//������������������������������������������������
void SceneMain::checkUnProject(INT Indx)
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
void SceneMain::setScore()
{

}

//����������������������������������������
// �f�[�^�ǂݍ���
//����������������������������������������
void SceneMain::loadStageData(size_t stageNumber)
{
	std::ifstream loadtFile;

	std::string stageString = std::to_string(stageNumber);
	loadtFile.open(StageFilePass + stageString + ".bin", std::ios::binary);

	if (!loadtFile)
	{
		MessageBox(nullptr, TEXT("Error"), TEXT("�X�e�[�W�f�[�^�ǂݍ��݃G���["), MB_ICONERROR);
		return;
	}

	for (size_t gameObjTypeIndex = 0; gameObjTypeIndex < GameObjectBase::MaxGameObjType; gameObjTypeIndex++)
	{
		for (auto itr = gameObjPtr[gameObjTypeIndex].begin(); itr != gameObjPtr[gameObjTypeIndex].end(); itr++)
		{
			loadtFile.read((CHAR*)(&exportWorkData), sizeof(ExportData));

			if (exportWorkData.isUsed)
			{
				selectGameObjIndex++;
			}

			(*itr)->reflectionExportData(exportWorkData);
		}
	}
}

//����������������������������������������
// �Q�[���I�u�W�F�N�g����
//����������������������������������������
void SceneMain::creteGameObj(size_t objType)
{
	switch (objType)
	{
	case static_cast<INT>(GameObjectType::NormalBlockObj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[0].push_back(NEW MainObject("Spikes.x","IceStone.png",objIndex,GameObjectType::NormalBlockObj,false));
		}
		break;
	case static_cast<INT>(GameObjectType::MoveBlockOBj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[1].push_back(NEW MainObject("flatAndHill.x","double_1.png", objIndex, GameObjectType::MoveBlockOBj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::FieldObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[2].push_back(NEW MainObject("flat.x","double_1.png", objIndex, GameObjectType::FieldObj,true));
		}
		break;
	case static_cast<INT>((GameObjectType::GoalObj)) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[3].push_back(NEW Goal("heart.x", "heart.png", objIndex));
		}
		break; 
	case static_cast<INT>(GameObjectType::SpikeObj) :
		for (INT objIndex = 0; objIndex < MaxGameObj; objIndex++)
		{
			gameObjPtr[4].push_back(NEW SpikeBlock("block_spike1.x", "cube_metal_unity.tga", objIndex, GameObjectType::SpikeObj, false));
		}
		break;
	default:
		break;
	}
}

void SceneMain::createItem(size_t index)
{

}