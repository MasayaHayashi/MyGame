//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Board.cpp
// 2Dポリゴン　ゲーム内のUIなど一枚の画像に使われるもの
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Board.h"
#include "../Application/Application.h"
#include "../SceneManager/SceneManager.h"
#include "../DirectX3D/DirectX3D.h"
#include "../Camera/camera.h"
#include "../Collision/Collision.h"
#include "../MyVector3/MyVector3.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Board::Board()
{
	pD3DVtxBuff	= nullptr;
	pD3DTexture	= nullptr;
	vertexBoard.fade	= false;
	pos					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale				= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	size				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posDestBoard		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tempQuaternion		= D3DXQUATERNION(0, 0, 0, 1);
	radAngle		= 0.0f;
	rotSpeed		= 0.0f;
	isAlphaBlend		= false;
	isvelocityUV			= false;
	isNarrow				= false;
	boardType			= boardType::Polygon2d;

	// テクスチャ関連
	texUV_SizeX			   = 0.0f;
	texUV_SizeY			   = 0.0f;
	velocityUV				   = D3DXVECTOR3 (0.0f,0.0f,0.0f);
	texPatternDivideX, texPatternDivideY = 0;
	animPattern		   = 0;
	intervalChangePattern = 0;
	animCnt			   = 0;
	currentAnimPattern	   = 1;
	color				   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Board::Board(size_t index)
{
	pD3DVtxBuff = nullptr;
	pD3DTexture = nullptr;
	vertexBoard.fade = false;
	pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	posDestBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tempQuaternion = D3DXQUATERNION(0, 0, 0, 1);
	radAngle = 0.0f;
	rotSpeed = 0.0f;
	isAlphaBlend = false;
	isvelocityUV = false;
	isNarrow = false;
	boardType = boardType::Polygon2d;

	// テクスチャ関連
	texUV_SizeX = 0.0f;
	texUV_SizeY = 0.0f;
	velocityUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	texPatternDivideX, texPatternDivideY = 0;
	animPattern = 0;
	intervalChangePattern = 0;
	animCnt = 0;
	currentAnimPattern = 1;
	color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	idNumber = index;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Board::~Board()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボード初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::initialize()
{
//	CreateTexture();	// テクスチャ生成
	isUsed = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボード後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::finalize()
{
	// 頂点情報解放
	ResourceManager::destroyVtx();
	ResourceManager::destroyAllTexture();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボード更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::update()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボード描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::draw()
{
	if (!isUsed)
	{
		return;
	}
	if (!vertexBoard.pD3DVtxBuff)
	{
		return;
	}

	// オブジェクト取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	D3DXMATRIX mtxScale, mtxTranslate, mtxRoll,mtxRot;

	// 描画
	switch (vertexBoard.boardType)
	{
	case  boardType::Polygon2d:

			// αテストを有効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, true);	// アルファテストオン
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);				// 5以下は描画しない
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// 頂点バッファをデバイスのデータストリームにバインド
			devicePtr->SetStreamSource(0, vertexBoard.pD3DVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			devicePtr->SetFVF(FVF_VERTEX_2D);

			devicePtr->SetTexture(0, texture.pD3DTexture);

			// アルファブレンド設定
			if (isAlphaBlend)
			{
				devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ポリゴンの描画
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, Polygon);

			// アルファブレンド設定を戻す
			devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// αテストを無効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case boardType::Billboard:

			// αテストを有効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ラインティングを無効にする
			devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);

			// ビューマトリックスを取得
			mtxTempView = Collision::getCameraTransform("Camera",0)->viewMtx;

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&worldMtx);

			// 逆行列をもとめる
			D3DXMatrixInverse(&worldMtx, nullptr, &mtxTempView);
			worldMtx._41 = 0.0f;
			worldMtx._42 = 0.0f;
			worldMtx._43 = 0.0f;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, vertexBoard.scale.x, vertexBoard.scale.y, vertexBoard.scale.z);
			D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, vertexBoard.pos.x, vertexBoard.pos.y, vertexBoard.pos.z);
			D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

			// ワールドマトリックスの設定
			devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

			// 頂点バッファをデバイスのデータストリームにバインド
			devicePtr->SetStreamSource(0, vertexBoard.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			devicePtr->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			devicePtr->SetTexture(0, texture.pD3DTexture);
	
			// アルファブレンド
			if (isAlphaBlend)
				devicePtr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// 半透明処理を行う
			devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

			// ポリゴンの描画
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, Polygon);

			// ラインティングを有効にする
			devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);

			// αテストを無効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		break;
	case boardType::Polygon3d:

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&worldMtx);

#if 0
		// 回転を反映
		D3DXQuaternionIdentity(&quat);

		vecUpObj = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// オブジェクトの上方向
		vecUpField = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// 地面の法線

														// オブジェクトの上方向と地面の法線の外積から回転軸を求める
		D3DXVec3Cross(&outVec, &vecUpObj, &vecUpField);

		// オブジェクトの上方向と地面の法線のなす角を求める
		fDotProduct = D3DXVec3Dot(&vecUpObj, &vecUpField);
		fRot = acosf(fDotProduct / (sqrtf(vecUpObj.x * vecUpObj.x + vecUpObj.y * vecUpObj.y + vecUpObj.z * vecUpObj.z)
			* sqrtf(vecUpField.x * vecUpField.x + vecUpField.y * vecUpField.y + vecUpField.z * vecUpField.z)));

		// 回転軸となす角からクォータニオンを求め、回転マトリックスを算出
		D3DXQuaternionRotationAxis(&quat, &outVec, fRot);
		D3DXMatrixRotationQuaternion(&mtxRot, &quat);
		D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);
#endif
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, pos.x,
			pos.y,
			pos.z);
		D3DXMatrixMultiply(&worldMtx,
			&worldMtx, &mtxTranslate);

		// ワールドマトリックスの設定
		devicePtr->SetTransform(D3DTS_WORLD, &worldMtx);

		// 頂点バッファをレンダリングパイプラインに設定
		devicePtr->SetStreamSource(0, vertexBoard.pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		devicePtr->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		devicePtr->SetTexture(0, texture.pD3DTexture);


		// ポリゴンの描画
		devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, Polygon);

		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 生成した頂点情報のセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setVtx()
{
	switch (boardType)
	{
	case boardType::Polygon2d:
		{
			// 頂点バッファの中身を埋める
			VERTEX_2D  *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定

			// 回転中心座標(ポリゴンの中心)
			D3DXVECTOR3 center = D3DXVECTOR3(vertexBoard.pos.x, vertexBoard.pos.y, 0.0f);

			// 左上
			pVtx[0].vtx = center;
			pVtx[0].vtx.x += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[0].vtx.y += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// 右上
			pVtx[1].vtx = center;
			pVtx[1].vtx.x += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[1].vtx.y += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (-vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// 左下
			pVtx[2].vtx = center;
			pVtx[2].vtx.x += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[2].vtx.y += (-vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// 右下
			pVtx[3].vtx = center;
			pVtx[3].vtx.x += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * cos(vertexBoard.radAngle) - (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * sin(vertexBoard.radAngle);
			pVtx[3].vtx.y += (vertexBoard.size.x / 2.0f) * vertexBoard.scale.x * sin(vertexBoard.radAngle) + (vertexBoard.size.y / 2.0f) * vertexBoard.scale.y * cos(vertexBoard.radAngle);

			// 頂点データをアンロックする
			vertexBoard.pD3DVtxBuff->Unlock();
			break;

		}
	case boardType::Billboard:
		{
			//頂点バッファの中身を埋める
			VERTEX_3D * pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, vertexBoard.size.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

			// 頂点データをアンロックする
			vertexBoard.pD3DVtxBuff->Unlock();
			break;
		}
	case boardType::Polygon3d:
		{
			//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, size.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

			// 頂点データをアンロックする
			vertexBoard.pD3DVtxBuff->Unlock();
			break;
		}
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::createTexture(TEXTURE_DATA &Texture)
{
	// 例外処理
	if (!fileName)
		return false;
	if (size <= 0)
		return false;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, fileName, &Texture.pD3DTexture))
	{
		return true;
	}

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::makevertexBoard()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();
	
	// 頂点情報生成
	switch (boardType)
	{
	case boardType::Polygon2d:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (Vertex),	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&pD3DVtxBuff,							// 頂点バッファインターフェースへのポインタ
			nullptr)))										// nullptrに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(vertexBoard.pos.x - vertexBoard.size.x * 0.5f, vertexBoard.pos.y + vertexBoard.size.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(vertexBoard.pos.x + vertexBoard.size.x * 0.5f, vertexBoard.pos.y + vertexBoard.size.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(vertexBoard.pos.x - vertexBoard.size.x * 0.5f, vertexBoard.pos.y - vertexBoard.size.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(vertexBoard.pos.x + vertexBoard.size.x * 0.5f, vertexBoard.pos.y - vertexBoard.size.y * 0.5f, 0.0f);

				// rhwの設定
				pVtx[0].rhw =
					pVtx[1].rhw =
					pVtx[2].rhw =
					pVtx[3].rhw = 1.0f;

				// 反射光の設定
				pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			// 頂点データをアンロックする
			pD3DVtxBuff->Unlock();
		}
		break;
	case boardType::Billboard:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))						// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

		// 法線の設定
		if (size.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	case boardType::Polygon3d:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&vertexBoard.pD3DVtxBuff,			// 頂点バッファインターフェースへのポインタ
			nullptr)))						// nullptrに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(vertexBoard.size.x, vertexBoard.size.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-vertexBoard.size.x, -vertexBoard.size.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(vertexBoard.size.x, -vertexBoard.size.y, 0.0f);

		// 法線の設定
		if (size.z == 0.0f)
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else
		{
			// 法線の設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}

		// 反射光の設定
		pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		pD3DVtxBuff->Unlock();

		break;
	default:
		break;
	}

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成 (法線情報設定)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::makevertexBoard(D3DXVECTOR3 nor)
{
	static constexpr INT Vertex = 4;

	LPDIRECT3DDEVICE9 devicePtr =  DirectX3D::getDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * Vertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuff,	// 頂点バッファインターフェースへのポインタ
		nullptr)))						// nullptrに設定
	{
		return false;
	}

	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-size.x / 2.0f, 0.0f, -size.z);
	pVtx[1].vtx = D3DXVECTOR3(-size.x / 2.0f, 0.0f,  size.z);
	pVtx[2].vtx = D3DXVECTOR3( size.x / 2.0f, 0.0f, -size.z);
	pVtx[3].vtx = D3DXVECTOR3( size.x / 2.0f, 0.0f,  size.z);
	
	/*
	{
	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(size.x, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f,size.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(size.x,size.y, 0.0f);
	}
	*/

	// 法線の設定
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
	pVtx[2].nor = nor;
	pVtx[3].nor = nor;

	// 反射光の設定
	pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	// 頂点データをアンロックする
	pD3DVtxBuff->Unlock();


	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 分割したテクスチャのセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setTexture()
{
	switch (boardType)
	{
	case boardType::Polygon2d:
			// 頂点バッファの中身を埋める
			VERTEX_2D  *pVtx;

			FLOAT fPosXLeft, fPosXRight;
			FLOAT fPosYUp, fPosYDown;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// UV座標の設定

			// 各座標の計算
			fPosXLeft = (FLOAT)(currentAnimPattern % texPatternDivideX) * texUV_SizeX;
			fPosXRight = fPosXLeft + texUV_SizeX;
			fPosYUp = (FLOAT)(currentAnimPattern / (animPattern / texPatternDivideY)) * texUV_SizeY;
			fPosYDown = fPosYUp + texUV_SizeY;

			pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

			// 頂点データをアンロックする
			vertexBoard.pD3DVtxBuff->Unlock();
		break;
	case boardType::Billboard:
	{
		// 頂点バッファの中身を埋める
		VERTEX_3D  *pVtx;

		FLOAT fPosXLeft, fPosXRight;
		FLOAT fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 各座標の計算
		fPosXLeft = (FLOAT)(currentAnimPattern % texPatternDivideX) * texUV_SizeX;
		fPosXRight = fPosXLeft + texUV_SizeX;
		fPosYUp = (FLOAT)(currentAnimPattern / (animPattern / texPatternDivideY)) * texUV_SizeY;
		fPosYDown = fPosYUp + texUV_SizeY;

		pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

		// 頂点データをアンロックする
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	case boardType::Polygon3d:
		break;
	default:
		break;
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点カラーの変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setcolor()
{
	switch (boardType)
	{
	case boardType::Polygon2d:
			// 2Dポリゴンのとき
			//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].diffuse = color;
			pVtx[1].diffuse = color;
			pVtx[2].diffuse = color;
			pVtx[3].diffuse = color;

			// 頂点データをアンロックする
			vertexBoard.pD3DVtxBuff->Unlock();
		break;
	case boardType::Billboard:
	{
		// 3Dポリゴンのとき
		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].diffuse = color;
		pVtx[1].diffuse = color;
		pVtx[2].diffuse = color;
		pVtx[3].diffuse = color;

		// 頂点データをアンロックする
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	case boardType::Polygon3d:
	{
		// 3Dポリゴンのとき
		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		vertexBoard.pD3DVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].diffuse = color;
		pVtx[1].diffuse = color;
		pVtx[2].diffuse = color;
		pVtx[3].diffuse = color;

		// 頂点データをアンロックする
		vertexBoard.pD3DVtxBuff->Unlock();
	}
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::initializeStatus()
{
	vertexBoard.pos = D3DXVECTOR3(Application::ScreenCenterX, Application::ScreenCenterY, 0.0f);
	vertexBoard.rotDeg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertexBoard.scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	setVtx();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Board::getPosition()
{
	return pos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// サイズ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Board::getSize()
{
	return size;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setNumber(BYTE nSetNumber)
{
	idNumber = nSetNumber;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメーションセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setAnimation()
{
	animCnt ++;

	if ((animCnt % intervalChangePattern) == 0)
	{
		// パターンの切り替え
		currentAnimPattern = (currentAnimPattern + 1) % animPattern;

		//param->currentAnimPattern = nAnim;

		// テクスチャ座標を切り替え
		//SetTextureExplosion(0, currentAnimPattern);
		setTexture();
		//Setcolor(D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f));
		//SetvtxBoard();
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setNarrowFlg(bool bSet)
{
	isNarrow = bSet;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用フラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setUsedFlg(bool SetFlg)
{
	isUsed = SetFlg;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DTEXTURE9 Board::getTexture()
{
	return pD3DTexture;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DVERTEXBUFFER9 Board::getVtxBuff()
{
	return pD3DVtxBuff;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用フラグ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BOOL Board::getUsedFlg()
{
	return isUsed;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setPosition(D3DXVECTOR3 SetPos)
{
	vertexBoard.pos = SetPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 移動量セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setvelocity(D3DXVECTOR3 velocityNum)
{
	pos += velocityNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::destroyBoard()
{
//	SAFE_RELEASE(pD3DVtxBuff);	// 頂点解放
//	SAFE_RELEASE(pD3DTexture);	// テクスチャ解放
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメパターンセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setCurrentAnimPattern(INT nSetNum)
{
	currentAnimPattern = nSetNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 補間用開始位置セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::setStartCurvePos(D3DXVECTOR3 SetStart,size_t index)
{
	curvePos[index] = SetStart;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメパターン取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
INT Board::getCurrentAnim()
{
	return currentAnimPattern;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 当たっているか
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::isHit(std::string keyName)
{
	const Transform* ball1Transform = Collision::getTransform(keyName, 0);
	const Transform* ball2Transform = Collision::getTransform(keyName, 1);

	D3DXVECTOR3 ballToBallVector = ball1Transform->pos - ball2Transform->pos;
	ballToBallVector.y = 0.0f;

	FLOAT length = MyVector3::getLength(ballToBallVector);

	if (length < Collision::HitLength)
	{
		return true;

	}
	else
	{
		return false;
	}
}


//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 3Dから2Dへの座標変換
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
const D3DXVECTOR3 Board::checkUnProject(D3DXVECTOR3 worldPos,size_t index)
{
	D3DXMATRIX  viewMtx =		 Collision::getCameraTransform("Camera", 0)->viewMtx;
	D3DXMATRIX  projectionMtx =	 Collision::getCameraTransform("Camera", 0)->projectionMtx;

	D3DXVECTOR3 outVec;
	D3DXVECTOR3 outVec2;

	D3DXVec3TransformCoord(&outVec, &worldPos, &viewMtx);
	D3DXVec3TransformCoord(&outVec2, &outVec, &projectionMtx);

	outVec2.y *= -1;

	outVec2.x *= 0.5f;
	outVec2.y *= 0.5f;

	outVec2.x += 0.5f;
	outVec2.y += 0.5f;

	outVec2.x *= Application::ScreenWidth;
	outVec2.y *= Application::ScreenHeight;

	return outVec2;
}