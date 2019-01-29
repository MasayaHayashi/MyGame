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
enum class  boardType
{
	Polygon2d = 0,
	Billboard,
	Polygon3d,
};

// ===== クラス定義 =====
class Board : public GameObjectBase
{
public:
	Board();
	virtual ~Board();
	virtual void initialize();
	virtual void update();
	virtual void draw();
	virtual void initializeStatus();
	virtual void finalize();

	bool makevertexBoard();	
	bool makevertexBoard(D3DXVECTOR3);		// 頂点情報生成
	void setTexture();						// UV座標セット
	void setVtx();							// 頂点情報セット
	void setcolor();						// 頂点カラーセット
	bool createTexture(TEXTURE_DATA &Texture);

	void setNumber(BYTE);					// 使用番号セット
	void setAnimation();					// アニメーションセット
	void setPosition(D3DXVECTOR3);			// 位置セット
	void setUsedFlg(bool setflg);			// 使用フラグセット
	void setvelocity(D3DXVECTOR3 velocityNum);		// 移動量セット
	void setCurrentAnimPattern(INT nAnim);	// アニメーションセット
	void setStartCurvePos(D3DXVECTOR3 );	// ベジェ曲線、エルミート曲線用スタート位置セット
	void setNarrowFlg(bool bset);
	void destroyBoard();					// 板ポリゴン解放

	bool isHit(std::string keyName);
	// ゲッター
	LPDIRECT3DTEXTURE9		getTexture();		// テクスチャ情報生成
	LPDIRECT3DVERTEXBUFFER9 getVtxBuff();		// 頂点情報生成
	BOOL					getUsedFlg();		// 使用フラグ取得 
	D3DXVECTOR3				getPosition();		// 座標取得
	D3DXVECTOR3				getSize();			// 大きさ取得
	INT						getCurrentAnim();	// アニメパターン取得

protected:
	static constexpr INT MaxLerp = 4;
	static constexpr INT Vertex  = 4;
	static constexpr INT Polygon = 2;

	VERTEX_BOARD_DATA		vertexBoard;		// 頂点情報データ
	TEXTURE_DATA			texture;			// テクスチャデータ

	CHAR				fileName[256];		// 読み込むファイル名
	CHAR				textureFileName[256];
	LPDIRECT3DTEXTURE9		pD3DTexture;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;	// 頂点バッファインターフェースへのポインタ
	LPDIRECT3DINDEXBUFFER9	pD3DIdxBuff;	// インデックスバッファへのポインタ
	D3DXVECTOR3				pos;			// 位置
	D3DXVECTOR3				size;			// 大きさ
	D3DXVECTOR3				rot;			// 回転
	D3DXVECTOR3				scale;			// 拡大率
	D3DXVECTOR3				posDestBoard;		// 目的位置
	D3DXVECTOR3				curvePos[MaxLerp];	// ベジェ曲線、エルミート曲線用ベクトル(三次元)
	D3DXMATRIX				worldMtx;		// ワールド行列
	D3DXCOLOR				color;				// 色
	D3DMATERIAL9			mat;				// マテリアル
	FLOAT					radAngle;		// ポリゴンの回転角度(ラジアン値)
	FLOAT					rotSpeed;		// ポリゴンの回転速度
	bool					isAlphaBlend;	// テクスチャの加算合成設定
	boardType				boardType;			// 2Dポリゴンか、ビルボードかの識別

	// テクスチャ関連
	INT						texPatternDivideX;		// テクスチャ内分割数X
	INT						texPatternDivideY;		// テクスチャ内分割数Y
	INT						animPattern;			// アニメーションのパターン数
	INT						intervalChangePattern;	// アニメーションの切り替わるフレーム数
	INT						animCnt;				// アニメーションカウンター
	INT						currentAnimPattern;		// アニメーションパターン番号
	FLOAT					texUV_SizeX;			// テクスチャ切り替え用UV座標X
	FLOAT					texUV_SizeY;			// テクスチャ切り替え用UV座標Y

	BYTE					idNumber;				// 識別番号
	bool					isNarrow;				// 縮小処理
	bool					isUsed;
private:

	D3DXMATRIX				mtxTempView;			// 逆行列を求めるためのビュー行列
	D3DXQUATERNION			tempQuaternion;			// ローカル座標系で回転させるためのクォータニオン
	bool					isvelocityUV;				// UV座標を動かすかどうか
	INT						nUVCnt;					// UV座標移動用カウンタ
	D3DXVECTOR3				velocityUV;					// UV座標移動用

};

#endif
