//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Board.cpp
// 2Dポリゴン　ゲーム内のUIなど一枚の画像に使われるもの
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "Board.h"
#include "../SceneManager/SceneManager.h"
#include "C_Camera.h"

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
Board::Board()
{
	pD3DVtxBuffBoard	= nullptr;
	pD3DTextureBoard	= nullptr;
	VertexBoard.bFade   = false;
	PosBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ScaleBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SizeBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	RotBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	MoveBoard			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PosDestBoard		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	TempQuaternion		= D3DXQUATERNION(0, 0, 0, 1);
	fRadAngleBoard		= 0.0f;
	fRotSpeedBoard		= 0.0f;
	bAlphaBlendBoard	= false;
	bUVMove				= false;
	narrow				= false;
	boardType			= BoardType::Polygon2d;

	fCnt = 0.0f;

	// テクスチャ関連
	TexUV_SizeX			   = 0.0f;
	TexUV_SizeY			   = 0.0f;
	MoveUV				   = D3DXVECTOR3 (0.0f,0.0f,0.0f);
	TexPatternDivideX, TexPatternDivideY = 0;
	NumAnimPattern		   = 0;
	nIntervalChangePattern = 0;
	nAnimCnt			   = 0;
	CurrentAnimPattern	   = 1;
	Color				   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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
	bUsed = false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ボード後処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::finalize()
{
	// 頂点情報解放
	C_RESOURCE_MANAGER::DestroyVtx();
	C_RESOURCE_MANAGER::DestroyAllTexture();
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
const void Board::draw()
{
	// 例外処理
	if (!used)
		return;
	if (!vertexBoard.pD3DVtxBuffBoard)
		return;

	// オブジェクト取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	D3DXMATRIX mtxScale, mtxTranslate, mtxRoll,mtxRot;

	C_CAMERA *pCamera = NULL;

	// 描画
	switch (VertexBoard.BoardType)
	{
	case POLYGON_2D:

			// αテストを有効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, true);	// アルファテストオン
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);				// 5以下は描画しない
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// 頂点バッファをデバイスのデータストリームにバインド
			devicePtr->SetStreamSource(0, VertexBoard.pD3DVtxBuffBoard, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			devicePtr->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			//if(Texture.pD3DTexture)
			devicePtr->SetTexture(0, Texture.pD3DTexture);

			// アルファブレンド設定
			if (bAlphaBlendBoard)
				devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// テクスチャの設定（フレーム部分）
			//	devicePtr->SetTexture(0,pD3DTexture);

			// ポリゴンの描画（フレーム部分）
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// アルファブレンド設定を戻す
			devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// αテストを無効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case BILLBOARD:

			pCamera = GetSceneManager()->GetInstanse()->GetCamera();

			// αテストを有効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			devicePtr->SetRenderState(D3DRS_ALPHAREF, 5);
			devicePtr->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			// ラインティングを無効にする
			devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);


			// ビューマトリックスを取得
			mtxTempView = pCamera->GetMtxView();

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&WorldMtxBoard);

			// ポリゴンを正面に向ける

			// 逆行列をもとめる
			D3DXMatrixInverse(&WorldMtxBoard, NULL, &mtxTempView);
			WorldMtxBoard._41 = 0.0f;
			WorldMtxBoard._42 = 0.0f;
			WorldMtxBoard._43 = 0.0f;

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, VertexBoard.ScaleBoard.x, VertexBoard.ScaleBoard.y, VertexBoard.ScaleBoard.z);
			D3DXMatrixMultiply(&WorldMtxBoard, &WorldMtxBoard, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, VertexBoard.PosBoard.x, VertexBoard.PosBoard.y, VertexBoard.PosBoard.z);
			D3DXMatrixMultiply(&WorldMtxBoard, &WorldMtxBoard, &mtxTranslate);

			// ワールドマトリックスの設定
			devicePtr->SetTransform(D3DTS_WORLD, &WorldMtxBoard);

			// 頂点バッファをデバイスのデータストリームにバインド
			devicePtr->SetStreamSource(0, VertexBoard.pD3DVtxBuffBoard, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			devicePtr->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			devicePtr->SetTexture(0, Texture.pD3DTexture);
	
			// アルファブレンド
			if (bAlphaBlendBoard)
				devicePtr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			// 半透明処理を行う
			devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

			// ポリゴンの描画
			devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

			// ラインティングを有効にする
			devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);

			// αテストを無効に
			devicePtr->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// アルファブレンド設定を戻す
	//		devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		break;
	case POLYGON_3D:

	//	D3DXMATRIX mtxRot, mtxTranslate;
		/*
		// 減算合成
		devicePtr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// 結果 = 転送先(DEST) - 転送元(SRC)
		devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// Z比較なし
		//	devicePtr->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&WorldMtxBoard);

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
		D3DXMatrixRotationYawPitchRoll(&mtxRot, RotBoard.y, RotBoard.x, RotBoard.z);
		D3DXMatrixMultiply(&WorldMtxBoard, &WorldMtxBoard, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, PosBoard.x,
			PosBoard.y,
			PosBoard.z);
		D3DXMatrixMultiply(&WorldMtxBoard,
			&WorldMtxBoard, &mtxTranslate);

		// ワールドマトリックスの設定
		devicePtr->SetTransform(D3DTS_WORLD, &WorldMtxBoard);

		// 頂点バッファをレンダリングパイプラインに設定
		devicePtr->SetStreamSource(0, VertexBoard.pD3DVtxBuffBoard, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		devicePtr->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		devicePtr->SetTexture(0, Texture.pD3DTexture);

		// ポリゴンの描画
		devicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, NULL, NUM_POLYGON);

		/*
		// 通常ブレンド
		devicePtr->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// 結果 = 転送元(SRC) + 転送先(DEST)
		devicePtr->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		devicePtr->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Z比較あり
		devicePtr->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		*/
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 生成した頂点情報のセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetVtxBoard()
{
	switch (BoardType)
	{
	case POLYGON_2D:
		{
			// 頂点バッファの中身を埋める
			VERTEX_2D  *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定

			// 回転中心座標(ポリゴンの中心)
			D3DXVECTOR3 center = D3DXVECTOR3(VertexBoard.PosBoard.x, VertexBoard.PosBoard.y, 0.0f);

			// 左上
			pVtx[0].vtx = center;
			pVtx[0].vtx.x += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[0].vtx.y += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// 右上
			pVtx[1].vtx = center;
			pVtx[1].vtx.x += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[1].vtx.y += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (-VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// 左下
			pVtx[2].vtx = center;
			pVtx[2].vtx.x += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[2].vtx.y += (-VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// 右下
			pVtx[3].vtx = center;
			pVtx[3].vtx.x += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * cos(VertexBoard.fRadAngleBoard) - (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * sin(VertexBoard.fRadAngleBoard);
			pVtx[3].vtx.y += (VertexBoard.SizeBoard.x / 2.0f) * VertexBoard.ScaleBoard.x * sin(VertexBoard.fRadAngleBoard) + (VertexBoard.SizeBoard.y / 2.0f) * VertexBoard.ScaleBoard.y * cos(VertexBoard.fRadAngleBoard);

			// 頂点データをアンロックする
			VertexBoard.pD3DVtxBuffBoard->Unlock();
			break;

		}
	case BILLBOARD:
		{
			//頂点バッファの中身を埋める
			VERTEX_3D * pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

			// 頂点データをアンロックする
			VertexBoard.pD3DVtxBuffBoard->Unlock();
			break;
		}
	case POLYGON_3D:
		{
			//頂点バッファの中身を埋める
			VERTEX_3D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, SizeBoard.y, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

			// 頂点データをアンロックする
			VertexBoard.pD3DVtxBuffBoard->Unlock();
			break;
		}
	case MAX_BOARD_TYPE:
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::CreateTexture(TEXTURE_DATA &Texture)
{
	// 例外処理
	if (!szFileName)
		return false;
	if (SizeBoard <= 0)
		return false;

	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(devicePtr, szFileName, &Texture.pD3DTexture))
		return true;

	return false;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::MakeVertexBoard()
{
	// デバイス取得
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();
	
	// 頂点情報生成
	switch (BoardType)
	{
	case POLYGON_2D:

		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_2D) * (NUM_VERTEX),	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,							// 頂点バッファの使用法　
			FVF_VERTEX_2D,								// 使用する頂点フォーマット
			D3DPOOL_MANAGED,							// リソースのバッファを保持するメモリクラスを指定
			&pD3DVtxBuffBoard,							// 頂点バッファインターフェースへのポインタ
			NULL)))										// NULLに設定
		{
			return false;
		}

		{//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			{
				// 頂点座標の設定
				pVtx[0].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x - VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y + VertexBoard.SizeBoard.y * 0.5f, 0.0f);
				pVtx[1].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x + VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y + VertexBoard.SizeBoard.y * 0.5f, 0.0f);
				pVtx[2].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x - VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y - VertexBoard.SizeBoard.y * 0.5f, 0.0f);
				pVtx[3].vtx = D3DXVECTOR3(VertexBoard.PosBoard.x + VertexBoard.SizeBoard.x * 0.5f, VertexBoard.PosBoard.y - VertexBoard.SizeBoard.y * 0.5f, 0.0f);

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
			pD3DVtxBuffBoard->Unlock();
		}
		break;
	case BILLBOARD:
	{
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

		// 法線の設定
		if (SizeBoard.z == 0.0f)
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
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case POLYGON_3D:
		// オブジェクトの頂点バッファを生成
		if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
			D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
			FVF_VERTEX_3D,				// 使用する頂点フォーマット
			D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
			&VertexBoard.pD3DVtxBuffBoard,			// 頂点バッファインターフェースへのポインタ
			NULL)))						// NULLに設定
		{
			return false;
		}

		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, VertexBoard.SizeBoard.y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(-VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(VertexBoard.SizeBoard.x, -VertexBoard.SizeBoard.y, 0.0f);

		// 法線の設定
		if (SizeBoard.z == 0.0f)
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
		pD3DVtxBuffBoard->Unlock();

		break;
	default:
		break;
	}

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成 (法線情報設定)
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool Board::MakeVertexBoard(D3DXVECTOR3 nor)
{
	LPDIRECT3DDEVICE9 devicePtr = GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffBoard,	// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return false;
	}

	//頂点バッファの中身を埋める
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);
	
	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
	pVtx[1].vtx = D3DXVECTOR3(-SizeBoard.x / 2.0f, 0.0f,  SizeBoard.z);
	pVtx[2].vtx = D3DXVECTOR3( SizeBoard.x / 2.0f, 0.0f, -SizeBoard.z);
	pVtx[3].vtx = D3DXVECTOR3( SizeBoard.x / 2.0f, 0.0f,  SizeBoard.z);
	
	/*
	{
	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(SizeBoard.x, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f,SizeBoard.y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(SizeBoard.x,SizeBoard.y, 0.0f);
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
	pD3DVtxBuffBoard->Unlock();


	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 分割したテクスチャのセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetTexture()
{
	switch (BoardType)
	{
	case POLYGON_2D:
			// 頂点バッファの中身を埋める
			VERTEX_2D  *pVtx;

			FLOAT fPosXLeft, fPosXRight;
			FLOAT fPosYUp, fPosYDown;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			// UV座標の設定

			// 各座標の計算
			fPosXLeft = (FLOAT)(CurrentAnimPattern % TexPatternDivideX) * TexUV_SizeX;
			fPosXRight = fPosXLeft + TexUV_SizeX;
			fPosYUp = (FLOAT)(CurrentAnimPattern / (NumAnimPattern / TexPatternDivideY)) * TexUV_SizeY;
			fPosYDown = fPosYUp + TexUV_SizeY;

			pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
			pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
			pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
			pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

			// 頂点データをアンロックする
			VertexBoard.pD3DVtxBuffBoard->Unlock();
		break;
	case BILLBOARD:
	{
		// 頂点バッファの中身を埋める
		VERTEX_3D  *pVtx;

		FLOAT fPosXLeft, fPosXRight;
		FLOAT fPosYUp, fPosYDown;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		// 各座標の計算
		fPosXLeft = (FLOAT)(CurrentAnimPattern % TexPatternDivideX) * TexUV_SizeX;
		fPosXRight = fPosXLeft + TexUV_SizeX;
		fPosYUp = (FLOAT)(CurrentAnimPattern / (NumAnimPattern / TexPatternDivideY)) * TexUV_SizeY;
		fPosYDown = fPosYUp + TexUV_SizeY;

		pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
		pVtx[1].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
		pVtx[2].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
		pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

		// 頂点データをアンロックする
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case POLYGON_3D:
		break;
	default:
		break;
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点カラーの変更
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetColor()
{
	switch (BoardType)
	{
	case POLYGON_2D:
			// 2Dポリゴンのとき
			//頂点バッファの中身を埋める
			VERTEX_2D *pVtx;

			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点座標の設定
			pVtx[0].diffuse = Color;
			pVtx[1].diffuse = Color;
			pVtx[2].diffuse = Color;
			pVtx[3].diffuse = Color;

			// 頂点データをアンロックする
			VertexBoard.pD3DVtxBuffBoard->Unlock();
		break;
	case BILLBOARD:
	{
		// 3Dポリゴンのとき
		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].diffuse = Color;
		pVtx[1].diffuse = Color;
		pVtx[2].diffuse = Color;
		pVtx[3].diffuse = Color;

		// 頂点データをアンロックする
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case POLYGON_3D:
	{
		// 3Dポリゴンのとき
		//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		VertexBoard.pD3DVtxBuffBoard->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].diffuse = Color;
		pVtx[1].diffuse = Color;
		pVtx[2].diffuse = Color;
		pVtx[3].diffuse = Color;

		// 頂点データをアンロックする
		VertexBoard.pD3DVtxBuffBoard->Unlock();
	}
		break;
	case MAX_BOARD_TYPE:
		break;
	default:
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ステータス初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::InitStatus()
{
	VertexBoard.PosBoard = D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f);
	VertexBoard.RotBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VertexBoard.ScaleBoard = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetVtxBoard();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Board::GetPosition()
{
	return PosBoard;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// サイズ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
D3DXVECTOR3 Board::GetSize()
{
	return SizeBoard;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 番号セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetNumber(BYTE nSetNumber)
{
	uNumber = nSetNumber;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメーションセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetAnimation()
{
	nAnimCnt ++;

	if ((nAnimCnt % nIntervalChangePattern) == 0)
	{
		// パターンの切り替え
		CurrentAnimPattern = (CurrentAnimPattern + 1) % NumAnimPattern;

		//param->currentAnimPattern = nAnim;

		// テクスチャ座標を切り替え
		//SetTextureExplosion(0, CurrentAnimPattern);
		SetTexture();
		//SetColor(D3DXCOLOR(1.0f, 1.0, 1.0f, 1.0f));
		//SetVtxBoard();
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 縮小してから使用フラグをfalseへ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetNarrowFlg(bool bSet)
{
	bNarrow = bSet;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用フラグセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetUsedFlg(bool SetFlg)
{
	bUsed = SetFlg;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ情報取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DTEXTURE9 Board::GetTexture()
{
	return pD3DTextureBoard;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
LPDIRECT3DVERTEXBUFFER9 Board::GetVtxBuff()
{
	return pD3DVtxBuffBoard;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 使用フラグ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
BOOL Board::GetUsedFlg()
{
	return bUsed;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 位置セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetPosition(D3DXVECTOR3 SetPos)
{
	VertexBoard.PosBoard = SetPos;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 移動量セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetMoveNum(D3DXVECTOR3 MoveNum)
{
	PosBoard += MoveNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// モデル解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::DestroyBoard()
{
	SAFE_RELEASE(pD3DVtxBuffBoard);	// 頂点解放
	SAFE_RELEASE(pD3DTextureBoard);	// テクスチャ解放
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カウンタセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetCnt(FLOAT fSet)
{
	fCnt = fSet;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// カウンタセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetCnt(INT nSet)
{
	nCnt = nSet;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメパターンセット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetCurrentAnimPattern(INT nSetNum)
{
	CurrentAnimPattern = nSetNum;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 補間用開始位置セット
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void Board::SetStartCurvePos(D3DXVECTOR3 SetStart)
{
	CurvePos[0] = SetStart;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 線形補間用カウンタ取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
FLOAT Board::GetCurveCnt()
{
	return fCnt;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// アニメパターン取得
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
UINT Board::GetCurrentAnim()
{
	return CurrentAnimPattern;
}