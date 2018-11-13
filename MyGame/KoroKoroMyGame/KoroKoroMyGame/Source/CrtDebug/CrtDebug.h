//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// CrtDebug.h
// Author : Masaya Hayashi
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

// ===== 多重インクルード防止 =====
#ifndef CRT_DEBUG_H
#define CRT_DEBUG_H

// ===== インクルード部 =====
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// ===== 定数・マクロ定義 =====
#if defined(DEBUG) || defined(_DEBUG)
#define NEW  new(_NORMAL_BLOCK, __FILE__, __LINE__)  // new によるメモリリーク検出
#else
#define NEW  new									 // リリースの場合通常のメモリ確保
#endif

#endif