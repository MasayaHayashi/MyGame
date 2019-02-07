//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ItemBase.cpp
// アイテム基底クラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "ItemBase.h"
#include "../ResoueceManager/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../SceneManager/SceneManager.h"
#include "../GameManager/GameManager.h"
#include "../MyVector3/MyVector3.h"
#include "../KeyBoard/Keyboard.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ItemBase::ItemBase()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
// modelPass:	 モデルパス入力
// texturePass : テクスチャパス入力
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ItemBase::ItemBase(std::string modelPass,std::string texturePass, size_t setNumber,GameObjectType setGameObj,bool setFieldModel)
{
	tagName						= modelPass;
	idNumber					= setNumber;
	modelPasses[tagName]		= passName + modelPass;
	texturePasses[tagName]		= passName + texturePass;
	myGameObjType				= setGameObj;
	isFieldObject				= setFieldModel;
	
	myTransform.isUsed = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ItemBase::~ItemBase()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ItemBase::initialize()
{
	// ファイルパス設定
	strcpy_s(fileName, modelPasses[tagName].c_str());
	strcpy_s(texFileName, texturePasses[tagName].c_str());

	// モデル生成
	ResourceManager::makeModel(meshDataObj, fileName, meshType);
	ResourceManager::createTexture(textureData, texFileName);

	myTransform.collisionBox = meshDataObj.collitionBox;

	Collision::registerList(&myTransform, tagName);

	// 位置、移動量、拡大率初期化
	myTransform.scale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	myTransform.rotDeg	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	myTransform.pos		= D3DXVECTOR3(0.0f, -meshDataObj.collitionBox.y * 2, 0.0f);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ItemBase::finalize()
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
void ItemBase::update()
{
	// コライダー更新
//	pCollider->UpdateCollider(mtxWorldPawn, FIELD_BOX_COLOR );
	updateExportData();

	if (!myTransform.isUsed)
	{
		return;
	}

	if (GameManager::getGameType() != GameManager::GameType::Playing)
	{
		return;
	}

	if (myGameObjType == GameObjectType::NormalBlockObj)
	{
		myTransform.rotDeg.y += 0.1f;

		D3DXMATRIX matrix;
		D3DXMatrixRotationY(&matrix, D3DXToRadian(myTransform.rotDeg.y));
		D3DXMatrixMultiply(&worldMtx, &worldMtx, &matrix);
	}

	if (Collision::getTransform(tagName, idNumber)->isHitAABB)
	{
 		GameManager::changeGameType(GameManager::GameType::Miss);
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブロック描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ItemBase::draw()
{
	if (!myTransform.isUsed)
	{
		return;
	}

	Pawn::draw();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void ItemBase::draw(D3DXMATRIX mtxView, D3DXMATRIX mtxProj)
{
	// 例外処理
	if (!myTransform.isUsed)
	{
		return;
	}

	// 描画
	Pawn::draw(mtxView,mtxProj);
}