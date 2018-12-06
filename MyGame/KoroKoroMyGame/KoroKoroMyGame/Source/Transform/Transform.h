////＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//// Transform.h
////＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//
//// ===== 多重インクルード防止 =====
//#ifndef TRANSFORM_H
//#define TRANSFORM_H
//
//// ===== インクルード部 =====
//#include "../DirectX3D/DirectX3D.h"
//
//class Transform
//{
//public:
//	Transform();
//	~Transform();
//
//	void setPos(D3DXVECTOR3 setPos);
//	void setVelocity(D3DXVECTOR3 setVelocity);
//	void setAccele(D3DXVECTOR3 setAccle);
//	void setRotDeg(D3DXVECTOR3 setRotDeg);
//	void setRotDegDest(D3DXVECTOR3 setRotDegDest);
//	void setScale(D3DXVECTOR3 setScale );
//	void setCross(D3DXVECTOR3 setCross);
//	void setNormal(D3DXVECTOR3 setNormal);
//
//private:
//	D3DXVECTOR3			pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3			velocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3			accele		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3			rotDeg		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3			rotDegDest  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3			scale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//	D3DXVECTOR3			cross		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3			normal		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//};
//
//#endif