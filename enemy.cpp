#include "main.h"
#include "enemy.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "enemylife.h"
#include "effect.h"
#include "item.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL; //頂点バッファへのポインタ
Enemy g_aEnemy[NUM_ENEMY]; //敵の情報
int g_nNumEnemy; //敵の総数

float g_fLengthEnemy;    //対角線の長さ
float g_fAngleEnemy;     //対角線の角度

//敵の初期化処理
void InitEnemy(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy004.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy005.png",
		&g_apTextureEnemy[5]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/enemy006.png",
		&g_apTextureEnemy[6]);

	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++) {
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //位置を初期化する
		g_aEnemy[nCntEnemy].pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量を初期化する
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //向きを初期化する
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nCounterAnimEnemy = 0; //カウンターを初期化する
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bRot = true;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].nMaxLife = 0;
		g_aEnemy[nCntEnemy].nPower = 0;
		g_aEnemy[nCntEnemy].fSize = 0.0f;
		g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf(0.0f * 0.0f + 0.0f * 0.0f) / 2.0f;
		g_aEnemy[nCntEnemy].fAngleEnemy = atan2f(0.0f, 0.0f);
	}

	g_nNumEnemy = 0; //敵の総数をクリア

	//対角線の長さを算出する
	//g_fLengthEnemy = sqrtf(100.0f * 100.0f + 100.0f * 100.0f) / 2.0f;

	//対角線の角度を算出する
	//g_fAngleEnemy = atan2f(100.0f, 100.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_ENEMY * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++) {
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (-D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (-D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z;
		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z;
		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z;
		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
		pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//敵の終了処理
void UninitEnemy(void) {
	//テクスチャの破棄
	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++) {
		if (g_apTextureEnemy[nCntEnemy] != NULL) {
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL) {
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	StopSound();
}

//敵の更新処理
void UpdateEnemy(void) {
	Wave* pWave = GetWave();

	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++) {
		if (g_aEnemy[nCntEnemy].bUse == true) {
			VERTEX_2D* pVtx; //頂点情報へのポインタ

			g_aEnemy[nCntEnemy].nCounterAnimEnemy++; //カウンターを加算

			switch (g_aEnemy[nCntEnemy].state) {
			case ENEMYSTATE_NORMAL:
				VERTEX_2D* pVtx; //頂点情報へのポインタ

					//頂点バッファをロックし、頂点情報へのポインタを取得
				g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

				pVtx += (nCntEnemy * 4);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//頂点バッファをアンロックする
				g_pVtxBuffEnemy->Unlock();

				break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0) {
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				}

				break;
			}

			if (g_aEnemy[nCntEnemy].pos.x - 40.0f >= 1280.0f) {
				g_aEnemy[nCntEnemy].pos.x -= 1280.0f;
			}
			else if (g_aEnemy[nCntEnemy].pos.x - 40.0f <= 0.0f) {
				g_aEnemy[nCntEnemy].pos.x += 1280.0f;
			}
			if (g_aEnemy[nCntEnemy].pos.y - 40.0f >= 720.0f) {
				g_aEnemy[nCntEnemy].pos.y -= 720.0f;
			}
			else if (g_aEnemy[nCntEnemy].pos.y - 40.0f <= 0.0f) {
				g_aEnemy[nCntEnemy].pos.y += 720.0f;
			}

			g_aEnemy[nCntEnemy].pos.x += 1.0f;

			switch (g_aEnemy[nCntEnemy].size) {
			case SMALL:
				if ((g_aEnemy[nCntEnemy].nCounterAnimEnemy % 10) == 0) {
					if (g_aEnemy[nCntEnemy].bRot == true) {
						g_aEnemy[nCntEnemy].rot.z = 0.5f;
						g_aEnemy[nCntEnemy].bRot = false;
					}
					else if (g_aEnemy[nCntEnemy].bRot == false) {
						g_aEnemy[nCntEnemy].rot.z = -0.5f;
						g_aEnemy[nCntEnemy].bRot = true;
					}
					SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(g_aEnemy[nCntEnemy].rot.z * 5, -2.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
				}
				break;

			case NORMAL:
				if ((g_aEnemy[nCntEnemy].nCounterAnimEnemy % 18) == 0) {
					if (pWave->g_nWave == 1) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 2) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 3) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(10.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 4) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-10.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(10.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 5) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-20.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(20.0f, -20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
				}
				break;

			case BIG:
				if ((g_aEnemy[nCntEnemy].nCounterAnimEnemy % 18) == 0) {
					if (pWave->g_nWave == 1) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);

					}
					else if (pWave->g_nWave == 2) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 3) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(18.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-18.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 4) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(18.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-18.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
					else if (pWave->g_nWave == 5) {
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, 35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(18.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(-18.0f, -35.0f, 0.0f), 100, 20.0f, g_aEnemy[nCntEnemy].nPower, BULLETTYPE_NORMAL, false);
					}
				}
				break;
			}

			PlaySound(SOUND_LABEL_SE_SHOT_ENEMY);

			//位置を更新
			g_aEnemy[nCntEnemy].pos.y = (50.0f * sinf(((2 * D3DX_PI) / 100.0f) * g_aEnemy[nCntEnemy].pos.x)) + g_aEnemy[nCntEnemy].pos2.y;

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntEnemy * 4);

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (-D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (-D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z;
			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z;
			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z;
			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z;

			//頂点バッファをアンロックする
			g_pVtxBuffEnemy->Unlock();

		}
	}
}

//敵の描画設定
void DrawEnemy(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++) {
		if (g_aEnemy[nCntEnemy].bUse == true) {

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//敵の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

void SetEnemy(D3DXVECTOR3 pos, int nType, ENEMYSIZE size, int nLife, int nPower) {
	for (int nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++) {
		if (g_aEnemy[nCntEnemy].bUse == false) { //敵が使用されていない
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].pos2 = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].bUse = true;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].nCounterState = 0;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nMaxLife = nLife;
			g_aEnemy[nCntEnemy].nPower = nPower;
			g_aEnemy[nCntEnemy].size = size;
			g_aEnemy[nCntEnemy].rot.z = 0.0f;

			switch (size) {
			case SMALL:
				g_aEnemy[nCntEnemy].fSize = E_SIZE_SMALL;
				break;

			case NORMAL:
				g_aEnemy[nCntEnemy].fSize = E_SIZE_NORMAL;
				break;

			case BIG:
				g_aEnemy[nCntEnemy].fSize = E_SIZE_BIG;
				break;
			}

			float fSize = g_aEnemy[nCntEnemy].fSize * 2.0f;

			g_aEnemy[nCntEnemy].fLengthEnemy = sqrtf(fSize * fSize + fSize * fSize) / 2.0f;

			g_aEnemy[nCntEnemy].fAngleEnemy = atan2f(fSize, fSize);

			SetEnemyLife(nCntEnemy);

			VERTEX_2D* pVtx; //頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (nCntEnemy * 4);

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (-D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (-D3DX_PI + g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z;
			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + (D3DX_PI - g_aEnemy[nCntEnemy].fAngleEnemy)) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z;
			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z;
			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngleEnemy) * g_aEnemy[nCntEnemy].fLengthEnemy;
			pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z;

			//頂点バッファをアンロックする
			g_pVtxBuffEnemy->Unlock();

			g_nNumEnemy++;

			break;
		}
	}
}

Enemy* GetEnemy(void) {
	return &g_aEnemy[0];
}

//敵のヒット処理
void HitEnemy(int nCntEnemy, int nDamage) {
	Wave* pWave = GetWave();
	Bullet* pBullet = GetBullet();
	Effect* pEffect = GetEffect();

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (GetGod() == false) {
		if (pWave->g_nWave == 1) {
			AddScore(100);
		}
		else if (pWave->g_nWave == 2) {
			AddScore(150);
		}
		else if (pWave->g_nWave == 3) {
			AddScore(200);
		}
		else if (pWave->g_nWave == 4) {
			AddScore(250);
		}
		else if (pWave->g_nWave == 5) {
			AddScore(300);
		}
	}
	else if (GetGod() == true) {
		AddScore(rand() % 99900 + 100);
	}

	if (g_aEnemy[nCntEnemy].nLife <= 0) {
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		g_aEnemy[nCntEnemy].bUse = false;

		SetItem(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(0.0f, -5.0f, 0.0f), ITEM_MAX, true);

		g_nNumEnemy--; //敵の総数を減らす

		if (pWave->g_nWave == 1) {
			AddScore(10000);
		}
		else if (pWave->g_nWave == 2) {
			AddScore(20000);
		}
		else if (pWave->g_nWave == 3) {
			AddScore(30000);
		}
		else if (pWave->g_nWave == 4) {
			AddScore(40000);
		}
		else if (pWave->g_nWave == 5) {
			AddScore(50000);
		}
		
		if (g_nNumEnemy <= 0 && pWave->g_nWave < 5) {
			pWave->g_nWave++;
			pWave->g_bWaveMove = true;
			for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++) {
				pBullet->bUse = false;
			}
			for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++) {
				pEffect->bUse = false;
			}
		}
		else if (g_nNumEnemy <= 0 && pWave->g_nWave == 5) {
			SetGameState(GAMESTATE_END);
		}
	}
	else {
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;

		VERTEX_2D* pVtx; //頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nCntEnemy * 4);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffEnemy->Unlock();
	}

}
