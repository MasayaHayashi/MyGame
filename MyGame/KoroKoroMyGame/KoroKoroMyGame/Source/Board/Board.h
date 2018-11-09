//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// Board.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef BOARD_H
#define BOARD_H

// ===== インクルード部 =====
#include "../GameObjectBase/GameObjectBase.h"
#include "../ResoueceManager/ResourceManager.h"

// ===== 列挙体定義 =====
enum class  BoardType
{
	Polygon2d = 0,
	Billboard,
	Polygon3d,
};

// ===== クラス定義 =====
class Board final : public GameObjectBase
{
public:
	Board();
	virtual ~Board();
	virtual void initialize();
	virtual void update();
	virtual const void draw();
	virtual void initializeStatus();
	virtual void finalize();

	bool makeVertexBoard();	
	bool makeVertexBoard(D3DXVECTOR3);		// 頂点情報生成
	void setTexture();						// UV座標セット
	void setVtxBoard();						// 頂点情報セット
	void setColor();						// 頂点カラーセット
	bool createTexture(TEXTURE_DATA &Texture);

	void setNumber(BYTE);					// 使用番号セット
	void setAnimation();					// アニメーションセット
	void setPosition(D3DXVECTOR3);			// 位置セット
	void setUsedFlg(bool Setflg);			// 使用フラグセット
	void setMoveNum(D3DXVECTOR3 MoveNum);	// 移動量セット
	void setCnt(FLOAT fSet);				// カウンタセット
	void setCnt(INT nSet);					// カウンタセット
	void setCurrentAnimPattern(INT nAnim);	// アニメーションセット
	void setStartCurvePos(D3DXVECTOR3 );	// ベジェ曲線、エルミート曲線用スタート位置セット
	void setNarrowFlg(bool bSet);
	void destroyBoard();					// 板ポリゴン解放


	// ゲッター
	LPDIRECT3DTEXTURE9		getTexture();		// テクスチャ情報生成
	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// 頂点情報生成
	BOOL					getUsedFlg();		// 使用フラグ取得 
	D3DXVECTOR3				getPosition();		// 座標取得
	D3DXVECTOR3				getSize();			// 大きさ取得
	FLOAT					getCurveCnt();		// 線形補間用カウンタ取得
	UINT					getCurrentAnim();	// アニメパターン取得

	// セッター

protected:
	static constexpr INT MaxLerp = 4;

	VERTEX_BOARD_DATA		vertexBoard;		// 頂点情報データ
	TEXTURE_DATA			texture;			// テクスチャデータ

	CHAR					fileName[256];	// 読み込むファイル名
	LPDIRECT3DTEXTURE9		pD3DTextureBoard;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffBoard;	// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuffBoard;	// インデックスバッファへのポインタ
	D3DXVECTOR3				posBoard;			// 位置
	D3DXVECTOR3				sizeBoard;			// 大きさ
	D3DXVECTOR3				rotBoard;			// 回転
	D3DXVECTOR3				scaleBoard;			// 拡大率
	D3DXVECTOR3				posDestBoard;		// 目的位置
	D3DXVECTOR3				moveBoard;			// 移動量
	D3DXVECTOR3				curvePos[MaxLerp];	// ベジェ曲線、エルミート曲線用ベクトル(三次元)
	D3DXMATRIX				worldMtxBoard;		// ワールド行列
	D3DXCOLOR				color;				// 色
	D3DMATERIAL9			mat;				// マテリアル
	FLOAT					radAngleBoard;		// ポリゴンの回転角度(ラジアン値)
	FLOAT					rotSpeedBoard;		// ポリゴンの回転速度
	bool					alphaBlendBoard;	// テクスチャの加算合成設定
	BoardType				boardType;			// 2Dポリゴンか、ビルボードかの識別

	// テクスチャ関連
	INT						texPatternDivideX;		// テクスチャ内分割数X
	INT						texPatternDivideY;		// テクスチャ内分割数Y
	INT						animPattern;			// アニメーションのパターン数
	INT						intervalChangePattern;	// アニメーションの切り替わるフレーム数
	INT						animCnt;				// アニメーションカウンター
	INT						currentAnimPattern;		// アニメーションパターン番号
	FLOAT					texUV_SizeX;			// テクスチャ切り替え用UV座標X
	FLOAT					texUV_SizeY;			// テクスチャ切り替え用UV座標Y

	FLOAT					fCnt;					// 線形補間などに使用するカウンタ(浮動小数点数)
	INT						nCnt;					// 線形補間などに使用するカウンタ(整数)


	BYTE					number;				// 識別番号
	bool					narrow;				// 縮小処理
	bool					used;				// 使用フラグ
private:

	D3DXMATRIX				mtxTempView;			// 逆行列を求めるためのビュー行列
	D3DXQUATERNION			tempQuaternion;			// ローカル座標系で回転させるためのクォータニオン
	bool					isMoveUV;				// UV座標を動かすかどうか
	INT						nUVCnt;					// UV座標移動用カウンタ
	D3DXVECTOR3				MoveUV;					// UV座標移動用

};

#endif
