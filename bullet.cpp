#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;      //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];                    //弾の情報
float g_fLengthBullet;    //対角線の長さ
float g_fAngleBullet;     //対角線の角度

int g_nCounterB;

D3DXVECTOR3 g_Enemypos;

void InitBullet(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet000.png",
		&g_pTextureBullet);

	//弾の情報の初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++) {
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;
		//対角線の長さを算出する
		g_aBullet[nCntBullet].fLengthBullet = sqrtf(20.0f * 20.0f + 20.0f * 20.0f) / 2.0f;
		//対角線の角度を算出する
		g_aBullet[nCntBullet].fAngleBullet = atan2f(20.0f, 20.0f);
		g_aBullet[nCntBullet].nDamage = 1;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++) {
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

void UninitBullet(void) {
	//テクスチャの破棄
	if (g_pTextureBullet != NULL) {
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL) {
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

void UpdateBullet(void) {
	Player* pPlayer = GetPlayer(); //敵の情報へのポインタ
	Enemy* pEnemy; //敵の情報へのポインタ

	g_nCounterB++;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++) {
		if (g_aBullet[nCntBullet].bUse == true) { //弾が使用されている
			//弾の位置の更新
			//ノーマル弾
			if (g_aBullet[nCntBullet].type == BULLETTYPE_NORMAL) {

				g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

				if (g_aBullet[nCntBullet].isPlayer == true) {
					//弾のエフェクトセット
					SetEffect(
						g_aBullet[nCntBullet].pos,
						D3DXCOLOR(0.0f, 0.8f, 0.0f, 1.0f),
						20.0f,
						90,
						false
					);
				}
				else if (g_aBullet[nCntBullet].isPlayer == false) {
					g_aBullet[nCntBullet].move.y *= 1.05f;

					//弾のエフェクトセット
					SetEffect(
						g_aBullet[nCntBullet].pos,
						D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f),
						20.0f,
						90,
						false
					);
				}
			}
			//ホーミング弾
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_HOMING) {
				if (g_aBullet[nCntBullet].isPlayer == true) {
					pEnemy = GetEnemy();
					int nCntTargetEnemy = -1; //弾が狙いに行く敵のインデックス
					float nealyLength = 1280.0f; //最も近い敵と弾の距離

					for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++, pEnemy++) {
						if (pEnemy->bUse == false) {
							//使用中でなければ次
							continue;
						}

						//弾と敵との距離を測るため x,y の差分を計算
						D3DXVECTOR3 pos = pEnemy->pos - g_aBullet[nCntBullet].pos;

						//弾から敵までの距離を計算
						int fMoveLength = sqrtf(pos.x * pos.x + pos.y * pos.y);

						//ほかの敵よりも弾と近い場合は情報を更新
						if (fMoveLength < nealyLength) {
							nCntTargetEnemy = nCntEnemy;
							nealyLength = fMoveLength;
						}
					}

					pEnemy = GetEnemy();

					for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++, pEnemy++){
						if (nCntEnemy == nCntTargetEnemy) {
							float fRotMove, fRotDest, fRotDiff;

							//弾の現在位置から移動先までの距離を計算する
							float moveX = g_aBullet[nCntBullet].move.x;
							float moveY = g_aBullet[nCntBullet].move.y;

							//もともと移動する予定だった距離を計算
							float fMoveLength = sqrtf(moveX * moveX + moveY * moveY);

							//敵の現在地から移動先までの角度を取得
							fRotMove = atan2f(moveX, moveY);

							//弾の現在地から敵の現在地までの角度を取得
							fRotDest = atan2f(
								pEnemy->pos.x - g_aBullet[nCntBullet].pos.x,
								pEnemy->pos.y - g_aBullet[nCntBullet].pos.y);

							//目的の移動方向までの角度の差分
							fRotDiff = fRotDest - fRotMove;

							//角度の値を修正する
							if (fRotDiff >= D3DX_PI) {
								fRotDiff += -D3DX_PI * 2;
							}
							else if (fRotDiff <= -D3DX_PI) {
								fRotDiff += D3DX_PI * 2;
							}

							fRotMove += fRotDiff * 0.35f;

							g_aBullet[nCntBullet].move.x = sinf(fRotMove) * fMoveLength;
							g_aBullet[nCntBullet].move.y = cosf(fRotMove) * fMoveLength;
						}
					}
					g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move * 5;

					//弾のエフェクトセット
					SetEffect(
						g_aBullet[nCntBullet].pos,
						D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
						g_aBullet[nCntBullet].fLengthBullet,
						90,
						true
					);

				}
				else if (g_aBullet[nCntBullet].isPlayer == false) {
					
				}
			}

			VERTEX_2D* pVtx; //頂点情報へのポインタ

		    //頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);

			//頂点座標の更新
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(D3DX_PI + g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(D3DX_PI + g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-D3DX_PI - g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-D3DX_PI - g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].fAngleBullet) * g_aBullet[nCntBullet].fLengthBullet;
			pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z;

			//頂点バッファをアンロックする
			g_pVtxBuffBullet->Unlock();

			if (g_aBullet[nCntBullet].isPlayer == true) { //プレイヤーの弾
				pEnemy = GetEnemy();

				for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++, pEnemy++) {
					if (pEnemy->bUse == true) { //敵が使用されている
						if (pEnemy->pos.x - pEnemy->fSize <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pEnemy->pos.x + pEnemy->fSize &&
							pEnemy->pos.y - pEnemy->fSize <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pEnemy->pos.y + pEnemy->fSize) { //敵と弾が当たった
							//爆発の設定
							SetExplosion(pEnemy->pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

							HitEnemy(nCntEnemy, g_aBullet[nCntBullet].nDamage);

							g_aBullet[nCntBullet].bUse = false; //弾を使用していない状態にする
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].isPlayer == false) {
				pPlayer = GetPlayer();

				if (pPlayer->pos.x - SIZE_X / 4 <= g_aBullet[nCntBullet].pos.x && g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + SIZE_X / 4 &&
					pPlayer->pos.y - SIZE_Y / 4 <= g_aBullet[nCntBullet].pos.y && g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + SIZE_Y / 4) {
					//爆発の設定
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f));
					HitPlayer(g_aBullet[nCntBullet].nDamage);
					g_aBullet[nCntBullet].bUse = false; //弾を使用していない状態にする
				}
			}
			



			//痔命のカウントダウン
			g_aBullet[nCntBullet].nLife--;

			//画面外に出た
			if ((g_aBullet[nCntBullet].pos.x <= 0.0f) || (1280.0f <= g_aBullet[nCntBullet].pos.x) ||
				(g_aBullet[nCntBullet].pos.y <= 0.0f) || (720.0f <= g_aBullet[nCntBullet].pos.y)) {
				g_aBullet[nCntBullet].nLife = 0;
			}

			//痔命が0になった時
			if (g_aBullet[nCntBullet].nLife == 0) {
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_aBullet[nCntBullet].bUse = false;
			}
		}
	}
}

void DrawBullet(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++) {
		if (g_aBullet[nCntBullet].bUse == true) { //弾が使用されている
			//弾の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, float fLength, int nDamage, BULLETTYPE type, bool isPlayer) {

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++) {
		if (g_aBullet[nCntBullet].bUse == false) {
			g_aBullet[nCntBullet].pos = pos;

			VERTEX_2D* pVtx; //頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntBullet * 4);

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z;
			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z;
			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(-g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(-g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z;
			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z;

			//頂点バッファをアンロックする
			g_pVtxBuffBullet->Unlock();

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].isPlayer = isPlayer;
			g_aBullet[nCntBullet].fLengthBullet = sqrtf(fLength * fLength + fLength * fLength) / 2.0f;
			g_aBullet[nCntBullet].fAngleBullet = atan2f(fLength, fLength);
			g_aBullet[nCntBullet].nDamage = nDamage;
			g_aBullet[nCntBullet].bUse = true; //使用している状況にする

			break;
		}
	}
}

Bullet* GetBullet(void) {
	return &g_aBullet[0];
}