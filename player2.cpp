#include "player2.h"
#include "player.h"
#include "input.h"

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer_2 = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer_2 = NULL; //頂点バッファへのポインタ
int g_nCounterAnimPlayer_2; //アニメーションカウンター
int g_nPatternAnimPlayer_2; //アニメーションパターンNo.
int g_nCollision_2;
int g_nCounterCol;
D3DXVECTOR3 g_posPlayer_2;  //位置
D3DXVECTOR3 g_posPlayer_22;  //位置
D3DXVECTOR3 g_movePlayer_2; //移動量
D3DXVECTOR3 g_movePlayer_22; //移動量

//当たり判定用
D3DXVECTOR3 p1R;
D3DXVECTOR3 p1L;
D3DXVECTOR3 p1U;
D3DXVECTOR3 p1D;

D3DXVECTOR3 p2R;
D3DXVECTOR3 p2L;
D3DXVECTOR3 p2U;
D3DXVECTOR3 p2D;

bool xPlus_2;
bool yPlus_2;
bool xPlus_22;
bool yPlus_22;

bool Hitcol;

//プレイヤーの初期化処理
void InitPlayer2(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/yama2.png",
		&g_pTexturePlayer_2);

	g_nCounterAnimPlayer_2 = 0; //カウンターを初期化する
	g_nPatternAnimPlayer_2 = 0; //パターンNo.を初期化する

	g_posPlayer_2 = D3DXVECTOR3(500.0f, 500.0f, 0.0f);  //位置を初期化する
	g_posPlayer_22 = D3DXVECTOR3(300.0f, 300.0f, 0.0f);  //位置を初期化する

	g_movePlayer_2 = D3DXVECTOR3(15.0f, 12.0f, 0.0f); //移動量を初期化する
	g_movePlayer_22 = D3DXVECTOR3(5.0f, 8.0f, 0.0f); //移動量を初期化する

	xPlus_2 = true;
	yPlus_2 = true;

	xPlus_22 = false;
	yPlus_22 = false;

	Hitcol = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer_2,
		NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer_2->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posPlayer_2.x - 50.0f, g_posPlayer_2.y - 50.0f, g_posPlayer_2.z);
	pVtx[1].pos = D3DXVECTOR3(g_posPlayer_2.x + 50.0f, g_posPlayer_2.y - 50.0f, g_posPlayer_2.z);
	pVtx[2].pos = D3DXVECTOR3(g_posPlayer_2.x - 50.0f, g_posPlayer_2.y + 50.0f, g_posPlayer_2.z);
	pVtx[3].pos = D3DXVECTOR3(g_posPlayer_2.x + 50.0f, g_posPlayer_2.y + 50.0f, g_posPlayer_2.z);

	pVtx[4].pos = D3DXVECTOR3(g_posPlayer_22.x - 50.0f, g_posPlayer_22.y - 50.0f, g_posPlayer_22.z);
	pVtx[5].pos = D3DXVECTOR3(g_posPlayer_22.x + 50.0f, g_posPlayer_22.y - 50.0f, g_posPlayer_22.z);
	pVtx[6].pos = D3DXVECTOR3(g_posPlayer_22.x - 50.0f, g_posPlayer_22.y + 50.0f, g_posPlayer_22.z);
	pVtx[7].pos = D3DXVECTOR3(g_posPlayer_22.x + 50.0f, g_posPlayer_22.y + 50.0f, g_posPlayer_22.z);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.2f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer_2->Unlock();
}

//プレイヤーの終了処理
void UninitPlayer2(void) {
	//テクスチャの破棄
	if (g_pTexturePlayer_2 != NULL) {
		g_pTexturePlayer_2->Release();
		g_pTexturePlayer_2 = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer_2 != NULL) {
		g_pVtxBuffPlayer_2->Release();
		g_pVtxBuffPlayer_2 = NULL;
	}
}

//プレイヤーの更新処理
void UpdatePlayer2(void) {
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	g_nCounterAnimPlayer_2++; //カウンターを加算


	if ((g_nCounterAnimPlayer_2 % 1) == 0) { //一定時間経過
		g_nPatternAnimPlayer_2 = (g_nPatternAnimPlayer_2 + 1) % 10;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer_2->Lock(0, 0, (void**)&pVtx, 0);

		if ((g_nCounterAnimPlayer_2 % 9) == 0) {
			pVtx[0].tex = D3DXVECTOR2(0.0f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.0f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.2f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.0f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.5f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.2f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.5f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);

			pVtx[4].tex = D3DXVECTOR2(0.0f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.0f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
			pVtx[5].tex = D3DXVECTOR2(0.2f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.0f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
			pVtx[6].tex = D3DXVECTOR2(0.0f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.5f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
			pVtx[7].tex = D3DXVECTOR2(0.2f + (g_nPatternAnimPlayer_2 % 5) * 0.2f, 0.5f + (g_nPatternAnimPlayer_2 / 5) * 0.5f);
		}

		//頂点座標の設定

		float speed = ((rand() % 9 + 1) / 150.0f) + 1.0f;

		pVtx[0].pos = D3DXVECTOR3((g_posPlayer_2.x - 50.0f) * speed, (g_posPlayer_2.y - 50.0f) * speed, g_posPlayer_2.z);
		pVtx[1].pos = D3DXVECTOR3((g_posPlayer_2.x + 50.0f) * speed, (g_posPlayer_2.y - 50.0f) * speed, g_posPlayer_2.z);
		pVtx[2].pos = D3DXVECTOR3((g_posPlayer_2.x - 50.0f) * speed, (g_posPlayer_2.y + 50.0f) * speed, g_posPlayer_2.z);
		pVtx[3].pos = D3DXVECTOR3((g_posPlayer_2.x + 50.0f) * speed, (g_posPlayer_2.y + 50.0f) * speed, g_posPlayer_2.z);

		pVtx[4].pos = D3DXVECTOR3(g_posPlayer_22.x - 50.0f, g_posPlayer_22.y - 50.0f, g_posPlayer_22.z);
		pVtx[5].pos = D3DXVECTOR3(g_posPlayer_22.x + 50.0f, g_posPlayer_22.y - 50.0f, g_posPlayer_22.z);
		pVtx[6].pos = D3DXVECTOR3(g_posPlayer_22.x - 50.0f, g_posPlayer_22.y + 50.0f, g_posPlayer_22.z);
		pVtx[7].pos = D3DXVECTOR3(g_posPlayer_22.x + 50.0f, g_posPlayer_22.y + 50.0f, g_posPlayer_22.z);

		int Rcol = rand() % 256;
		int Gcol = rand() % 256;
		int Bcol = rand() % 256;

		pVtx[0].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
		pVtx[1].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
		pVtx[2].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);
		pVtx[3].col = D3DCOLOR_RGBA(Rcol, Gcol, Bcol, 255);

		//位置を更新

		//キャラ衝突判定
		p1L = D3DXVECTOR3(g_posPlayer_2.x - 50.0f, g_posPlayer_2.y, g_posPlayer_2.z);
		p1R = D3DXVECTOR3(g_posPlayer_2.x + 50.0f, g_posPlayer_2.y, g_posPlayer_2.z);
		p1U = D3DXVECTOR3(g_posPlayer_2.x, g_posPlayer_2.y - 50.0f, g_posPlayer_2.z);
		p1D = D3DXVECTOR3(g_posPlayer_2.x, g_posPlayer_2.y + 50.0f, g_posPlayer_2.z);

		p2L = D3DXVECTOR3(g_posPlayer_22.x - 50.0f, g_posPlayer_22.y, g_posPlayer_22.z);
		p2R = D3DXVECTOR3(g_posPlayer_22.x + 50.0f, g_posPlayer_22.y, g_posPlayer_22.z);
		p2U = D3DXVECTOR3(g_posPlayer_22.x, g_posPlayer_22.y - 50.0f, g_posPlayer_22.z);
		p2D = D3DXVECTOR3(g_posPlayer_22.x, g_posPlayer_22.y + 50.0f, g_posPlayer_22.z);
	
		if ((g_nCounterAnimPlayer_2 % 3) == 0) {
			if ((((p2L.x <= p1L.x) && (p1L.x <= p2R.x)) && ((p2U.y <= p1L.y) && (p1L.y <= p2D.y))) ||
				(((p2L.x <= p1R.x) && (p1R.x <= p2R.x)) && ((p2U.y <= p1R.y) && (p1R.y <= p2D.y)))) {
				if (xPlus_2 == true && xPlus_22 == true) {
					if (g_posPlayer_22.x < g_posPlayer_2.x) {
						if (xPlus_22 == true) {
							xPlus_22 = false;
						}
						else if (xPlus_22 == false) {
							xPlus_22 = true;
						}
					}
					else if (g_posPlayer_2.x < g_posPlayer_22.x) {
						if (xPlus_2 == true) {
							xPlus_2 = false;
						}
						else if (xPlus_2 == false) {
							xPlus_2 = true;
						}
					}
				}
				else if (xPlus_2 == false && xPlus_22 == false) {
					if (g_posPlayer_22.x > g_posPlayer_2.x) {
						if (xPlus_22 == true) {
							xPlus_22 = false;
						}
						else if (xPlus_22 == false) {
							xPlus_22 = true;
						}
					}
					else if (g_posPlayer_2.x > g_posPlayer_22.x) {
						if (xPlus_2 == true) {
							xPlus_2 = false;
						}
						else if (xPlus_2 == false) {
							xPlus_2 = true;
						}
					}
				}
				else {
					if (xPlus_2 == true) {
						xPlus_2 = false;
					}
					else if (xPlus_2 == false) {
						xPlus_2 = true;
					}
					if (xPlus_22 == true) {
						xPlus_22 = false;
					}
					else if (xPlus_22 == false) {
						xPlus_22 = true;
					}
				}
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				pVtx[4].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[5].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[6].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[7].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				Hitcol = true;
			}
			if ((((p2L.x <= p1U.x) && (p1U.x <= p2R.x)) && ((p2U.y <= p1U.y) && (p1U.y <= p2D.y))) ||
				(((p2L.x <= p1D.x) && (p1D.x <= p2R.x)) && ((p2U.y <= p1D.y) && (p1D.y <= p2D.y)))) {
				if (yPlus_2 == true && yPlus_22 == true) {
					if (g_posPlayer_22.y < g_posPlayer_2.y) {
						if (yPlus_22 == true) {
							yPlus_22 = false;
						}
						else if (yPlus_22 == false) {
							yPlus_22 = true;
						}
					}
					else if (g_posPlayer_2.y < g_posPlayer_22.y) {
						if (yPlus_2 == true) {
							yPlus_2 = false;
						}
						else if (yPlus_2 == false) {
							yPlus_2 = true;
						}
					}
				}
				else if (yPlus_2 == false && yPlus_22 == false) {
					if (g_posPlayer_22.y > g_posPlayer_2.y) {
						if (yPlus_22 == true) {
							yPlus_22 = false;
						}
						else if (yPlus_22 == false) {
							yPlus_22 = true;
						}
					}
					else if (g_posPlayer_2.y > g_posPlayer_22.y) {
						if (yPlus_2 == true) {
							yPlus_2 = false;
						}
						else if (yPlus_2 == false) {
							yPlus_2 = true;
						}
					}
				}
				else {
					if (yPlus_2 == true) {
						yPlus_2 = false;
					}
					else if (yPlus_2 == false) {
						yPlus_2 = true;
					}
					if (yPlus_22 == true) {
						yPlus_22 = false;
					}
					else if (yPlus_22 == false) {
						yPlus_22 = true;
					}
				}
				pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

				pVtx[4].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[5].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[6].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[7].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				Hitcol = true;

			}
		}

		//壁判定
		if ((g_posPlayer_2.x + 50.0f >= 1280.0f) || (g_posPlayer_2.x - 50.0f <= 0.0f)) {
			if (xPlus_2 == true) {
				xPlus_2 = false;
			}
			else if (xPlus_2 == false) {
				xPlus_2 = true;
			}
		}
		if ((g_posPlayer_2.y + 50.0f >= 720.0f) || (g_posPlayer_2.y - 50.0f <= 0.0f)) {
			if (yPlus_2 == true) {
				yPlus_2 = false;
			}
			else if (yPlus_2 == false) {
				yPlus_2 = true;
			}
		}

		if ((g_posPlayer_22.x + 50.0f >= 1280.0f) || (g_posPlayer_22.x - 50.0f <= 0.0f)) {
			if (xPlus_22 == true) {
				xPlus_22 = false;
			}
			else if (xPlus_22 == false) {
				xPlus_22 = true;
			}
		}
		if ((g_posPlayer_22.y + 50.0f >= 720.0f) || (g_posPlayer_22.y - 50.0f <= 0.0f)) {
			if (yPlus_22 == true) {
				yPlus_22 = false;
			}
			else if (yPlus_22 == false) {
				yPlus_22 = true;
			}
		}

		//--------------------------------------------------

		if (xPlus_2 == true) {
			g_posPlayer_2.x += g_movePlayer_2.x;
		}
		else if (xPlus_2 == false) {
			g_posPlayer_2.x -= g_movePlayer_2.x;
		}

		if (yPlus_2 == true) {
			g_posPlayer_2.y += g_movePlayer_2.y;
		}
		else if (yPlus_2 == false) {
			g_posPlayer_2.y -= g_movePlayer_2.y;
		}

		if (xPlus_22 == true) {
			g_posPlayer_22.x += g_movePlayer_22.x;
		}
		else if (xPlus_22 == false) {
			g_posPlayer_22.x -= g_movePlayer_22.x;
		}

		if (yPlus_22 == true) {
			g_posPlayer_22.y += g_movePlayer_22.y;
		}
		else if (yPlus_22 == false) {
			g_posPlayer_22.y -= g_movePlayer_22.y;
		}

		if (Hitcol == true) {
			g_nCounterCol += 1;
			if ((g_nCounterCol % 30) == 0) {
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				Hitcol = false;
			}
		}

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer_2->Unlock();
	}
}

//プレイヤーの描画設定
void DrawPlayer2(void) {
	bool Godmode = GetGod();

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer_2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer_2);

	if (Godmode == true) {
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
	}
}