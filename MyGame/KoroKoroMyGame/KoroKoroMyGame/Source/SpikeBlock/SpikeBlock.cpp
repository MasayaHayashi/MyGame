//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// SpikeBlock.cpp
// ブロッククラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "SpikeBlock.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include "../KeyBoard/Keyboard.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SpikeBlock::SpikeBlock()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
// modelPass:	 モデルパス入力
// texturePass : テクスチャパス入力
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SpikeBlock::SpikeBlock(std::string modelPass, std::string texturePass, size_t setNumber, GameObjectType setGameObj, bool setFieldModel)
{
	tagName = modelPass;
	idNumber = setNumber;
	modelPasses[tagName] = passName + modelPass;
	texturePasses[tagName] = passName + texturePass;
	myGameObjType = setGameObj;
	isFieldObject = setFieldModel;

	isUsed = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
SpikeBlock::~SpikeBlock()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SpikeBlock::initialize()
{
	// ファイルパス設定
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, TextureFilePass.c_str());

	// モデル生成
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.collisionBox = meshDataObj.collitionBox;

	Collision::registerList(&myTransform, tagName);

	myTransform.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos = D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);

	setDefaultValue();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SpikeBlock::finalize()
{
	ResourceManager::destroyAllMesh();

	modelPasses.clear();
	texturePasses.clear();
	ResourceManager::destroyAllTexture();

	Collision::release(tagName);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SpikeBlock::update()
{
	updateExportData();

	if (!isUsed)
	{
		return;
	}

	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	if (MyVector3::getLength(Collision::getTransform("Player", 0)->pos - myTransform.pos) < 15.0f)
	{
		if (myGameObjType == GameObjectType::SpikeObj)
		{
			if (myTransform.pos.y < -7.0f)
			{
				myTransform.pos.y += 0.2f;
			}
		}
	}

	if (Collision::getTransform(tagName, idNumber)->isHitAABB)
	{
		GameManager::changeGameType(GameManager::GameType::Miss);
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SpikeBlock::draw()
{
	if (!isUsed)
	{
		return;
	}

	Pawn::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SpikeBlock::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// 例外処理
	if (!isUsed)
	{
		return;
	}

	// 描画
	Pawn::draw(mtxView, mtxProj);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void SpikeBlock::initializeStatus()
{
	Pawn::initializeStatus();
}