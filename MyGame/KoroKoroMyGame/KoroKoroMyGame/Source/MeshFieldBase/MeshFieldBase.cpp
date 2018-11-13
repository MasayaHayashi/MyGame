//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// C_MeshField.cpp
// メッシュフィールドクラス
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== インクルード部 =====
#include "MeshFieldBase.h"
#include "../DirectX3D/DirectX3D.h"

#define BLOCK_SPACE (0.0f)

D3DMATERIAL9 test;

#define SAFE_DELETE(p)       { if(p!=nullptr) { delete (p);     (p) = nullptr; } }
#define SAFE_RELEASE(p)      { if(p!=nullptr) { (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=nullptr; }}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// コンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_MESH_FIELD_BASE::C_MESH_FIELD_BASE()
{
	PosField   = D3DXVECTOR3(0.0f,0.0f,0.0f);
	MoveField  = D3DXVECTOR3(0.0f,0.0f,0.0f);
	RotField   = D3DXVECTOR3(0.0f,0.0f,0.0f);
	ScaleField = D3DXVECTOR3(1.0f,1.0f,1.0f);

	pD3DTextureField  = nullptr;		// テクスチャへのポインタ
	pD3DXMeshField    = nullptr;		// メッシュ情報へのポインタ
	pD3DXBuffMatField = nullptr;		// マテリアル情報へのポインタ
	pD3DIdxBuffField  = nullptr;
	pVtx			  = nullptr;
	bLoadMesh		  = false;

	texUV_SizeX			= 0.0f;
	currentAnimPattern  = 0;
	animPattern		= 0;
	texPatternDivideX	= 0;
	texPatternDivideY	= 0;
	texUV_SizeX			= 0;
	texUV_SizeY			= 0;


	test.Ambient.r = 1.0f;
	test.Ambient.g = 0.0f;
	test.Ambient.b = 0.0f;
	test.Ambient.a = 0.6f;
	
	test.Diffuse.r = 1.0f;
	test.Diffuse.g = 0.0f;
	test.Diffuse.b = 0.0f;
	test.Diffuse.a = 0.6f;
	
	test.Emissive.r = 1.0f;
	test.Emissive.g = 0.0f;
	test.Emissive.b = 0.0f;
	test.Emissive.a = 0.6f;
	
	test.Power = 0.5f;

	test.Specular.r = 1.0f;
	test.Specular.g = 0.0f;
	test.Specular.b = 0.0f;
	test.Specular.a = 1.0f;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// デストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
C_MESH_FIELD_BASE::~C_MESH_FIELD_BASE()
{

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ、マテリアルなどの解放処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MESH_FIELD_BASE::DestroyFieldMesh()
{
	// テクスチャ、メッシュ解放
	SAFE_RELEASE(pD3DTextureField);
	SAFE_RELEASE(pD3DXMeshField);
	SAFE_RELEASE(pD3DXBuffMatField);

	// 頂点情報解放
	SAFE_DELETE(pVtx);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Xファイルからモデル生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_MESH_FIELD_BASE::MakeModel()
{
	// 例外処理
	if (!fileName)
		return E_FAIL;
	if (pD3DXMeshField)
		return E_FAIL;

	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM, devicePtr, nullptr, &pD3DXBuffMatField, nullptr, &nNumMatField, &pD3DXMeshField)))
	{
		return E_FAIL;
	}
	
	// 各種フラグ更新
	bLoadMesh = true;

	// 頂点情報サイズ数取得
	dwNumVtx = pD3DXMeshField->GetNumVertices();	// 頂点数取得
	pVtx = NEW MESH_VTX[dwNumVtx];
	LPVOID pVtxWork;

	// 頂点バッファロック
	pD3DXMeshField->LockVertexBuffer(D3DLOCK_READONLY, &pVtxWork);
	CopyMemory(pVtx, pVtxWork, sizeof(MESH_VTX) * dwNumVtx);

	// 頂点バッファアンロック
//	pD3DXMeshField->UnlockVertexBuffer();

	// 頂点最大サイズ、最小サイズ取得
	MaxVtx = pVtx[0].VtxPos;
	MinVtx = pVtx[0].VtxPos;
	for (WORD i = 1; i < dwNumVtx; i++)
	{
		// 最大値取得
		if(MaxVtx.x < pVtx[i].VtxPos.x)
			MaxVtx.x = pVtx[i].VtxPos.x;

		if (MaxVtx.y < pVtx[i].VtxPos.y)
			MaxVtx.y = pVtx[i].VtxPos.y;

		if (MaxVtx.z < pVtx[i].VtxPos.z)
			MaxVtx.z = pVtx[i].VtxPos.z;

		// 最小値取得
		if (MinVtx.x > pVtx[i].VtxPos.x)
			MinVtx.x = pVtx[i].VtxPos.x;

		if (MinVtx.y > pVtx[i].VtxPos.y)
			MinVtx.y = pVtx[i].VtxPos.y;

		if (MinVtx.z > pVtx[i].VtxPos.z)
			MinVtx.z = pVtx[i].VtxPos.z;
	}

	// 最大、最小値をスケールに合わせる
//	MaxVtx.x *= ScaleField.x;
//	MaxVtx.y *= ScaleField.y;
//	MaxVtx.z *= ScaleField.z;

//	MinVtx.x *= ScaleField.x;
//	MinVtx.y *= ScaleField.y;
//	MinVtx.z *= ScaleField.z;

	// 中心座標、あたり判定取得
	CenterPos	 = (MaxVtx + MinVtx) * 0.5f;
	collitionBox = (MaxVtx - MinVtx) * 0.5f;
	
	// 境界球取得
	float dx = pVtx[0].VtxPos.x * ScaleField.x - CenterPos.x ;
	float dy = pVtx[0].VtxPos.y * ScaleField.y - CenterPos.y ;
	float dz = pVtx[0].VtxPos.z * ScaleField.z - CenterPos.z ;
	fCollisionRadus = sqrtf(dx * dx + dy * dy + dz * dz);

	for (DWORD i = 1; i < dwNumVtx; ++i)
	{
		dx = pVtx[i].VtxPos.x * ScaleField.x - CenterPos.x ;
		dy = pVtx[i].VtxPos.y * ScaleField.y - CenterPos.y ;
		dz = pVtx[i].VtxPos.z * ScaleField.z - CenterPos.z ;
		float fRadius = sqrtf(dx * dx + dy * dy + dz * dz);
		if (fCollisionRadus < fRadius)
			fCollisionRadus = fRadius;
	}

	CenterPos += PosField * 2;	// 中心座標をワールド座標系へ

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールドの頂点情報生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT C_MESH_FIELD_BASE::MakeVertexField()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// ブロック数
	nNumBlockX = nNumBlockX;
	nNumBlockZ = nNumBlockZ;

	// 頂点数(縮退ポリゴン用を考慮する)
	nNumVertex = nNumBlockX * nNumBlockZ * 4;

	// インデックス数の設定(縮退ポリゴン用を考慮する)
	nNumVertexIndex = nNumBlockX * nNumBlockZ * (4 + 4);

	// ポリゴン数(縮退ポリゴン用を考慮する)
	nNumPolygon = nNumBlockX * nNumBlockZ * (2 + 6);

	// ブロックサイズ
	fSizeBlockX = fSizeBlockX;
	fSizeBlockZ = fSizeBlockZ;
	float halfSizeX = fSizeBlockX * 0.5f;
	float halfSizeZ = fSizeBlockZ * 0.5f;

	// オブジェクトの頂点バッファを生成
	if (FAILED(devicePtr->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_3D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&pD3DVtxBuffField,			// 頂点バッファインターフェースへのポインタ
		nullptr)))						// nullptrに設定
	{
		return E_FAIL;
	}

	// オブジェクトのインデックスバッファを生成
	if (FAILED(devicePtr->CreateIndexBuffer(sizeof(WORD) * nNumVertexIndex,	// インデックスデータ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,													// インデックスバッファの使用法　
		D3DFMT_INDEX16,														// 使用するインデックスフォーマット
		D3DPOOL_MANAGED,													// リソースのバッファを保持するメモリクラスを指定
		&pD3DIdxBuffField,													// インデックスバッファインターフェースへのポインタ
		nullptr)))																// nullptrに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;
#if 0
		const float fSizeTexX = 1.0f / g_nNumBlockX;
		const float fSizeTexZ = 1.0f / g_nNumBlockZ;
#else
		const float fSizeTexX = 1.0f;
		const float fSizeTexZ = 1.0f;
#endif

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxZ = 0; nCntVtxZ < nNumBlockZ; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < nNumBlockX; nCntVtxX++, pVtx += 4)
			{
				D3DXVECTOR3 base = D3DXVECTOR3(-(nNumBlockX / 2.0f) * fSizeBlockX + nCntVtxX * (fSizeBlockX ) + halfSizeX,
								   0.0f,
								   (nNumBlockZ / 2.0f) * fSizeBlockZ - nCntVtxZ * (fSizeBlockZ ) - halfSizeZ);

				pVtx[0].vtx = D3DXVECTOR3(base.x - halfSizeX, 0.0f, base.z - halfSizeZ);	// 左手前
				pVtx[1].vtx = D3DXVECTOR3(base.x - halfSizeX, 0.0f, base.z + halfSizeZ);	// 左奥
				pVtx[2].vtx = D3DXVECTOR3(base.x + halfSizeX, 0.0f, base.z - halfSizeZ);	// 右手前
				pVtx[3].vtx = D3DXVECTOR3(base.x + halfSizeX, 0.0f, base.z + halfSizeZ);	// 右奥

				// 法線の設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);
				pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);
				pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

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

			}
		}

		// 頂点データをアンロックする
		pD3DVtxBuffField->Unlock();
	}

	{//インデックスバッファの中身を埋める
		WORD *pIdx;

		// インデックスデータの範囲をロックし、頂点バッファへのポインタを取得
		pD3DIdxBuffField->Lock(0, 0, (void**)&pIdx, 0);

		int nCntIdx = 0;
		int nBlockNo = 0;

		for (int nCntVtxZ = 0; nCntVtxZ < nNumBlockZ; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < nNumBlockX; nCntVtxX++)
			{
				// ブロック部分
				pIdx[nCntIdx] = nBlockNo;
				nCntIdx++;
				pIdx[nCntIdx] = nBlockNo + 1;
				nCntIdx++;
				pIdx[nCntIdx] = nBlockNo + 2;
				nCntIdx++;
				pIdx[nCntIdx] = nBlockNo + 3;
				nCntIdx++;

				// 縮退ポリゴン部分
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];	// 最後の頂点
				nCntIdx++;

				nBlockNo += 4;

				pIdx[nCntIdx] = nBlockNo + 1;	// 次の四角の左奥
				nCntIdx++;

				pIdx[nCntIdx] = nBlockNo + 1;	// 次の四角の左奥2
				nCntIdx++;

				pIdx[nCntIdx] = nBlockNo;		// 次の四角の左手前1
				nCntIdx++;
			}
		}

		// インデックスデータの範囲をアンロックする
		pD3DIdxBuffField->Unlock();
	}

	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// フィールド描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MESH_FIELD_BASE::DrawMeshField()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	if (!bLoadMesh)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorldField);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, RotField.y, RotField.x, RotField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// 縮小を反映
		D3DXMatrixTranslation(&mtxTranslate, ScaleField.x, ScaleField.y, ScaleField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// ワールドマトリックスの設定
		devicePtr->SetTransform(D3DTS_WORLD, &mtxWorldField);

		// 頂点バッファをレンダリングパイプラインに設定
		devicePtr->SetStreamSource(0, pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// インデックスバッファをレンダリングパイプラインに設定
		devicePtr->SetIndices(pD3DIdxBuffField);

		// 頂点フォーマットの設定
		devicePtr->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		devicePtr->SetTexture(0, pD3DTextureField);

		// ポリゴンの描画
		devicePtr->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nNumVertex, 0, nNumPolygon);
	}
	else
	{
		/*
		LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
		D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
		D3DXMATERIAL *pD3DXMat;
		D3DMATERIAL9 matDef;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorldField);

		// 拡大縮小
		D3DXMatrixScaling(&mtxScale, ScaleField.x, ScaleField.y, ScaleField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxScale);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, RotField.y, RotField.x, RotField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxRot);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// ワールドマトリックスの設定
		devicePtr->SetTransform(D3DTS_WORLD, &mtxWorldField);

		// 現在のマテリアルを取得
		devicePtr->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatField->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < static_cast<INT>nNumMatField; nCntMat++)
		{
			// マテリアルの設定
			devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			devicePtr->SetTexture(0, pD3DTextureField);

			// 描画
			pD3DXMeshField->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		devicePtr->SetMaterial(&matDef);
		*/
		LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();
		D3DXMATRIX mtxRot, mtxTranslate, mtxScale;
		D3DXMATERIAL *pD3DXMat;
		D3DMATERIAL9 matDef;

		// ライティングしない
		devicePtr->SetRenderState(D3DRS_LIGHTING, FALSE);

		// Zバッファ更新を無効
		devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxWorldField);

		// スケールを反映
		D3DXMatrixScaling(&mtxScale, ScaleField.x, ScaleField.y, ScaleField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxScale);

		// 移動を反映
		D3DXMatrixTranslation(&mtxTranslate, PosField.x, PosField.y, PosField.z);
		D3DXMatrixMultiply(&mtxWorldField, &mtxWorldField, &mtxTranslate);

		// ワールドマトリックスの設定
		devicePtr->SetTransform(D3DTS_WORLD, &mtxWorldField);
		
		// 現在のマテリアルを取得
		devicePtr->GetMaterial(&matDef);

		// マテリアル情報に対するポインタを取得
		pD3DXMat = (D3DXMATERIAL*)pD3DXBuffMatField->GetBufferPointer();

		for (INT nCntMat = 0; nCntMat < static_cast<INT>(nNumMatField); nCntMat++)
		{
			// マテリアルの設定
			devicePtr->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

			// テクスチャの設定
			devicePtr->SetTexture(0, pD3DTextureField);

			// 描画
			pD3DXMeshField->DrawSubset(nCntMat);
		}

		// マテリアルをデフォルトに戻す
		devicePtr->SetMaterial(&matDef);

		// 元に戻す
		devicePtr->SetRenderState(D3DRS_LIGHTING, TRUE);
		devicePtr->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テクスチャ生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void C_MESH_FIELD_BASE::CreateTexture()
{
	LPDIRECT3DDEVICE9 devicePtr = DirectX3D::getDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(devicePtr,					// デバイスへのポインタ
		texFileName,			// ファイルの名前
		&pD3DTextureField);	// 読み込むメモリー
}

void C_MESH_FIELD_BASE::SetTexture()
{		// 頂点バッファの中身を埋める
	VERTEX_3D  *pVtx;

	float fPosXLeft, fPosXRight;
	float fPosYUp, fPosYDown;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	pD3DVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	// 各座標の計算
	fPosXLeft = (float)(currentAnimPattern % texPatternDivideX) * texUV_SizeX;
	fPosXRight = fPosXLeft + texUV_SizeX;
	fPosYUp = (float)(currentAnimPattern / (animPattern / texPatternDivideY)) * texUV_SizeY;
	fPosYDown = fPosYUp + texUV_SizeY;

	pVtx[0].tex = D3DXVECTOR2(fPosXLeft, fPosYUp);
	pVtx[1].tex = D3DXVECTOR2(fPosXLeft, fPosYDown);
	pVtx[2].tex = D3DXVECTOR2(fPosXRight, fPosYUp);
	pVtx[3].tex = D3DXVECTOR2(fPosXRight, fPosYDown);

	// 頂点データをアンロックする
	pD3DVtxBuffField->Unlock();
}