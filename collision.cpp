//=====================================================================
//
// collision [collision.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "collision.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************

//=====================================================================
// 衝突判定処理（点と立方体）
// 点posAがposBとsizeBで作られる立方体の内にあるか判定します。
// posA  : 点
// posB  : 立方体の位置
// sizeB : 立方体のサイズ
//=====================================================================
bool CollisionPointBox(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 sizeB)
{
	D3DXVECTOR3 boxMin = D3DXVECTOR3(-sizeB.x / 2.0f, -sizeB.y / 2.0f, -sizeB.z / 2.0f);
	D3DXVECTOR3 boxMax = D3DXVECTOR3(sizeB.x / 2.0f, sizeB.y / 2.0f, sizeB.z / 2.0f);

	return CollisionPointBoxDirection(posA, posB, boxMin, boxMax);

}

//=====================================================================
// 衝突判定処理（点と立方体）
// 点posAがposBとboxMin、boxMaxで作られる立方体の内にあるか判定します。
// posA : 点
// posB : 立方体の位置
// boxMin・boxMax : 立方体の最小・最大頂点位置
//=====================================================================
bool CollisionPointBox(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 boxMin, D3DXVECTOR3 boxMax)
{
	if (posA.x >= posB.x + boxMin.x &&
		posA.x <= posB.x + boxMax.x &&
		posA.y >= posB.y + boxMin.y &&
		posA.y <= posB.y + boxMax.y &&
		posA.z >= posB.z + boxMin.z &&
		posA.z <= posB.z + boxMax.z)
	{
		return true;
	}

	return false;
}

//=====================================================================
// 衝突判定処理（点と立方体）
// 点posAがposBとsizeBで作られる立方体の内にあるか判定します。
// 衝突判定があった場合にはBYTE型で衝突方向を返します。
// 衝突判定がなかった場合にはCOLLSION_NONE(0x00)を返します。
//=====================================================================
BYTE CollisionPointBoxDirection(D3DXVECTOR3 posA, D3DXVECTOR3 posAOld, D3DXVECTOR3 posB, D3DXVECTOR3 sizeB)
{
    D3DXVECTOR3 boxMin = D3DXVECTOR3(-sizeB.x / 2.0f, -sizeB.y / 2.0f, -sizeB.z / 2.0f);
    D3DXVECTOR3 boxMax = D3DXVECTOR3( sizeB.x / 2.0f,  sizeB.y / 2.0f,  sizeB.z / 2.0f);

    return CollisionPointBoxDirection(posA, posAOld, posB, boxMin, boxMax);
}

//=====================================================================
// 衝突判定処理（点と立方体）
// 点posAがposBとboxMin、boxMaxで作られる立方体の内にあるか判定します。
// 衝突判定があった場合にはBYTE型で衝突方向を返します。
// 衝突判定がなかった場合にはCOLLSION_NONE(0x00)を返します。
//=====================================================================
BYTE CollisionPointBoxDirection(D3DXVECTOR3 posA, D3DXVECTOR3 posAOld, D3DXVECTOR3 posB, D3DXVECTOR3 boxMin, D3DXVECTOR3 boxMax)
{
	BYTE byHit = 0;

	// モデルの衝突判定処理
	if (
		posAOld.x <= posB.x + boxMin.x
		&& posA.x > posB.x + boxMin.x
		&& posA.z < posB.z + boxMax.z
		&& posA.z > posB.z + boxMin.z
		&& posA.y < posB.y + boxMax.y
		&& posA.y >= posB.y + boxMin.y
		)
	{// 左
		byHit |= COLLISION_LEFT;
	}
	if (
		posAOld.x >= posB.x + boxMax.x
		&& posA.x < posB.x + boxMax.x
		&& posA.z < posB.z + boxMax.z
		&& posA.z > posB.z + boxMin.z
		&& posA.y < posB.y + boxMax.y
		&& posA.y >= posB.y + boxMin.y
		)
	{// 右
		byHit |= COLLISION_RIGHT;
	}

	if (
		posAOld.z <= posB.z + boxMin.z
		&& posA.z > posB.z + boxMin.z
		&& posA.x > posB.x + boxMin.x
		&& posA.x < posB.x + boxMax.x
		&& posA.y < posB.y + boxMax.y
		&& posA.y >= posB.y + boxMin.y
		)
	{// 前
		byHit |= COLLISION_FRONT;
	}
	if (
		posAOld.z >= posB.z + boxMax.z
		&& posA.z < posB.z + boxMax.z
		&& posA.x > posB.x + boxMin.x
		&& posA.x < posB.x + boxMax.x
		&& posA.y < posB.y + boxMax.y
		&& posA.y >= posB.y + boxMin.y
		)
	{// 後ろ
		byHit |= COLLISION_BACK;
	}

	if (
		posAOld.y >= posB.y + boxMax.y
		&& posA.y <= posB.y + boxMax.y
		&& posA.x > posB.x + boxMin.x
		&& posA.x < posB.x + boxMax.x
		&& posA.z > posB.z + boxMin.z
		&& posA.z < posB.z + boxMax.z
		)
	{// 上
		byHit |= COLLISION_UP;
	}

	if (
		posAOld.y <= posB.y + boxMin.y
		&& posA.y > posB.y + boxMin.y
		&& posA.x > posB.x + boxMin.x
		&& posA.x < posB.x + boxMax.x
		&& posA.z > posB.z + boxMin.z
		&& posA.z < posB.z + boxMax.z
		)
	{// 下
		byHit |= COLLISION_DOWN;
	}

	return byHit;
}