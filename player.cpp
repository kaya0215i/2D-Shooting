#include "player.h"
#include "input.h"
#include "bullet.h"
#include "sound.h"
#include "score.h"
#include "effect.h"
#include "explosion.h"
#include "game.h"
#include "fade.h"

#define NUM_PLAYER (2)

//グローバル宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer[NUM_PLAYER] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL; //頂点バッファへのポインタ
Player g_player;
int g_nCounterAnimPlayer; //アニメーションカウンター
int g_nPatternAnimPlayer; //アニメーションパターンNo.
float g_fLengthPlayer;    //対角線の長さ
float g_fAnglePlayer;     //対角線の角度

bool Dash;
bool GodMode;
int nGod;

float BulletUD;

//プレイヤーの初期化処理
void InitPlayer(void) {
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/face.png",
		&g_pTexturePlayer[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/yamaPi.png",
		&g_pTexturePlayer[1]);

	g_nCounterAnimPlayer = 0; //カウンターを初期化する
	g_nPatternAnimPlayer = 0; //パターンNo.を初期化する

	g_player.pos = D3DXVECTOR3(200.0f, 160.0f, 0.0f);  //位置を初期化する

	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //移動量を初期化する

	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //向きを初期化する

	g_player.nXsize = SIZE_X;
	g_player.nYsize = SIZE_Y;

	g_player.bDisp = true;
	g_player.state = PLAYERSTATE_APPEAR;
	g_player.nLife = 100;
	g_player.nPower = 1;
	g_player.nPowerUp = 1;
	g_player.nPowerUpTime = 100;
	g_player.nUpgrade = 0;

	Dash = false;
	GodMode = false;
	nGod = 0;

	//対角線の長さを算出する
	g_fLengthPlayer = sqrtf(SIZE_X * SIZE_X + SIZE_Y * SIZE_Y) / 2.0f;

	//対角線の角度を算出する
	g_fAnglePlayer = atan2f(SIZE_X, SIZE_Y);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 8,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffPlayer,
		                        NULL);

	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++) {

		//頂点座標の設定
		pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[0].pos.z = g_player.pos.z;
		pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
		pVtx[1].pos.z = g_player.pos.z;
		pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[2].pos.z = g_player.pos.z;
		pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
		pVtx[3].pos.z = g_player.pos.z;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		if (nCntPlayer == 0) {
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else {
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		}

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤーの終了処理
void UninitPlayer(void) {
	//テクスチャの破棄
	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++) {
		if (g_pTexturePlayer[nCntPlayer] != NULL) {
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL) {
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	StopSound();
}

//プレイヤーの更新処理
void UpdatePlayer(void) {
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	g_nCounterAnimPlayer++; //カウンターを加算

	if (g_player.state != PLAYERSTATE_DEATH) {

		Command();

		if ((GetKeyboardPress(DIK_LSHIFT) == true) || (GetPadLtrigger() == true)) {
			//左SHIFTキーが押された
			Dash = true;
		}

		//===========================================================

		//pad←
		if ((GetPadPress(XINPUT_GAMEPAD_DPAD_LEFT) == true) || (GetPadLstickX(-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == true) || (GetKeyboardPress(DIK_A) == true)) {
			if ((GetPadPress(XINPUT_GAMEPAD_DPAD_UP) == true) || (GetPadLstickY(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) || (GetKeyboardPress(DIK_W) == true)) {
				if (Dash == true) {
					g_player.move.x += sinf(-D3DX_PI * 0.75f) * 10.0f;
					g_player.move.y += cosf(-D3DX_PI * 0.75f) * 10.0f;
				}
				else {
					g_player.move.x += sinf(-D3DX_PI * 0.75f) * 5.0f;
					g_player.move.y += cosf(-D3DX_PI * 0.75f) * 5.0f;
				}
			}
			else if ((GetPadPress(XINPUT_GAMEPAD_DPAD_DOWN) == true) || (GetPadLstickY(-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == true) || (GetKeyboardPress(DIK_S) == true)) {
				if (Dash == true) {
					g_player.move.x += sinf(-D3DX_PI * 0.25f) * 10.0f;
					g_player.move.y += cosf(-D3DX_PI * 0.25f) * 10.0f;
				}
				else {
					g_player.move.x += sinf(-D3DX_PI * 0.25f) * 5.0f;
					g_player.move.y += cosf(-D3DX_PI * 0.25f) * 5.0f;
				}
			}
			else {
				if (Dash == true) {
					g_player.move.x -= 10.0f;
				}
				else {
					g_player.move.x -= 5.0f;
				}
			}
		}

		//pad→
		else if ((GetPadPress(XINPUT_GAMEPAD_DPAD_RIGHT) == true) || (GetPadLstickX(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) || (GetKeyboardPress(DIK_D) == true)) {
			if ((GetPadPress(XINPUT_GAMEPAD_DPAD_UP) == true) || (GetPadLstickY(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) || (GetKeyboardPress(DIK_W) == true)) {
				if (Dash == true) {
					g_player.move.x += sinf(D3DX_PI * 0.75f) * 10.0f;
					g_player.move.y += cosf(D3DX_PI * 0.75f) * 10.0f;
				}
				else {
					g_player.move.x += sinf(D3DX_PI * 0.75f) * 5.0f;
					g_player.move.y += cosf(D3DX_PI * 0.75f) * 5.0f;
				}
			}
			else if ((GetPadPress(XINPUT_GAMEPAD_DPAD_DOWN) == true) || (GetPadLstickY(-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == true) || (GetKeyboardPress(DIK_S) == true)) {
				if (Dash == true) {
					g_player.move.x += sinf(D3DX_PI * 0.25f) * 10.0f;
					g_player.move.y += cosf(D3DX_PI * 0.25f) * 10.0f;
				}
				else {
					g_player.move.x += sinf(D3DX_PI * 0.25f) * 5.0f;
					g_player.move.y += cosf(D3DX_PI * 0.25f) * 5.0f;
				}
			}
			else {
				if (Dash == true) {
					g_player.move.x += 10.0f;
				}
				else {
					g_player.move.x += 5.0f;
				}
			}
		}

		//pad↑
		else if ((GetPadPress(XINPUT_GAMEPAD_DPAD_UP) == true) || (GetPadLstickY(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) || (GetKeyboardPress(DIK_W) == true)) {
			if (Dash == true) {
				g_player.move.y -= 10.0f;
			}
			else {
				g_player.move.y -= 5.0f;
			}
		}

		//pad↓
		else if ((GetPadPress(XINPUT_GAMEPAD_DPAD_DOWN) == true) || (GetPadLstickY(-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == true) || (GetKeyboardPress(DIK_S) == true)) {
			if (Dash == true) {
				g_player.move.y += 10.0f;
			}
			else {
				g_player.move.y += 5.0f;
			}
		}

		// Z
		if ((GetKeyboardPress(DIK_Z) == true) || (GetPadRstickX(-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) == true)) {
			
		}
		// C
		if ((GetKeyboardPress(DIK_C) == true) || (GetPadRstickX(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) == false)) {
			
		}
		// X
		if ((GetKeyboardPress(DIK_X) == true) || (GetPadPress(XINPUT_GAMEPAD_RIGHT_THUMB) == true)) {
			
		}

		// E / pad L
		if ((GetKeyboardPress(DIK_E) == true) || (GetPadPress(XINPUT_GAMEPAD_RIGHT_SHOULDER) == true)) {
			
		}
		// Q / pad R
		if ((GetKeyboardPress(DIK_Q) == true) || (GetPadPress(XINPUT_GAMEPAD_LEFT_SHOULDER) == true)) {
			
		}

		if (GodMode == false) {
			// SPACE pad / A , RtriggerButton
			if ((GetPadPress(XINPUT_GAMEPAD_A) == true) || (GetPadRtrigger() == true) || (GetKeyboardPress(DIK_SPACE) == true)) {
				if ((g_nCounterAnimPlayer % 8) == 0) {
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, 5.0f, 0.0f), 100, 20.0f, g_player.nPower * g_player.nPowerUp, BULLETTYPE_HOMING, true);
					PlaySound(SOUND_LABEL_SE_SHOT_PLAYER);
				}
			}

			if (GetPadTap(XINPUT_GAMEPAD_X) == true) {
				if (g_player.nUpgrade == 0) {
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10) * g_player.nPowerUp, BULLETTYPE_NORMAL, true);
				}
				else if (g_player.nUpgrade == 1) {
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, -15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
				}
				else if (g_player.nUpgrade == 2) {
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, -15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);

					SetBullet(g_player.pos, D3DXVECTOR3(7.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
					SetBullet(g_player.pos, D3DXVECTOR3(-7.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
				}
				else if (g_player.nUpgrade == 3) {
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
					SetBullet(g_player.pos, D3DXVECTOR3(0.0f, -15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);

					SetBullet(g_player.pos, D3DXVECTOR3(7.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
					SetBullet(g_player.pos, D3DXVECTOR3(-7.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);

					SetBullet(g_player.pos, D3DXVECTOR3(15.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
					SetBullet(g_player.pos, D3DXVECTOR3(-15.0f, 15.0f, 0.0f), 100, 20.0f, (g_player.nPower + 10)* g_player.nPowerUp, BULLETTYPE_NORMAL, true);
				}
			}
		}
		else if (GodMode == true) {
			if ((GetPadPress(XINPUT_GAMEPAD_A) == true) || (GetPadRtrigger() == true) || (GetKeyboardPress(DIK_SPACE) == true)) {
				SetBullet(g_player.pos, D3DXVECTOR3(g_player.rot.z * -5.0f, 5.0f, 0.0f), 50, 500.0f, g_player.nPower * 1000, BULLETTYPE_HOMING, true);
				PlaySound(SOUND_LABEL_SE_SHOT_PLAYER);
			}

			if (GetPadTap(XINPUT_GAMEPAD_X) == true) {
				SetBullet(g_player.pos, D3DXVECTOR3(g_player.rot.z * -5.0f, 5.0f * 20, 0.0f), 100, 0.1f, 99999999, BULLETTYPE_NORMAL, true);
			}

			SetEffect(g_player.pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), g_fLengthPlayer, 10, true);
		}
	}

	Dash = false;

	if (g_player.pos.x + 40.0f > 1280.0f) {
		g_player.pos.x = 1240.0f;
		g_player.move.x = 0.0f;
	}
	else if (g_player.pos.x - 40.0f < 0.0f) {
		g_player.pos.x = 40.0f;
		g_player.move.x = 0.0f;
	}
	if (g_player.pos.y + 40.0f > 720.0f) {
		g_player.pos.y = 680.0f;
		g_player.move.y = 0.0f;
	}
	else if (g_player.pos.y - 40.0f < 0.0f) {
		g_player.pos.y = 40.0f;
		g_player.move.y = 0.0f;
	}

	if (g_player.nPowerUp == 2) {
		g_player.nPowerUpTime--;
		if (g_player.nPowerUpTime >= 0) {
			g_player.nPowerUp = 1;
			g_player.nPowerUpTime = 100;
		}
	}

	if ((g_nCounterAnimPlayer % 3) == 0) { //一定時間経過
		//パターンNo.を更新する
		g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % 10;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//位置を更新
		g_player.pos.x += g_player.move.x;
		g_player.pos.y += g_player.move.y;

		//移動量を更新(減衰)
		g_player.move.x += (0.0f - g_player.move.x) * 0.5f;
		g_player.move.y += (0.0f - g_player.move.y) * 0.5f;

		switch (g_player.state) {
		case PLAYERSTATE_APPEAR:
			g_player.state = PLAYERSTATE_NORMAL;
			break;

		case PLAYERSTATE_NORMAL:
			if (GodMode == true) {
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

				pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			else if (GodMode == false) {
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			}

			break;

		case PLAYERSTATE_DAMAGE:
			g_player.nCounterState--;
			if (g_player.nCounterState <= 0) {
				g_player.state = PLAYERSTATE_NORMAL;

				if (GodMode == true) {
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

					pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				else if (GodMode == false) {
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

					pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				}
			}

			break;

		case PLAYERSTATE_DEATH:
			g_player.nCounterState--;
			if (g_player.nCounterState == 0) {
				SetGameState(GAMESTATE_END);
			}

			break;
		}

		for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++) {
			//頂点座標の設定
			pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
			pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z + (-D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
			pVtx[0].pos.z = g_player.pos.z;
			pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
			pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
			pVtx[1].pos.z = g_player.pos.z;
			pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
			pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
			pVtx[2].pos.z = g_player.pos.z;
			pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
			pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
			pVtx[3].pos.z = g_player.pos.z;

			pVtx += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();

	}
}

//プレイヤーの描画設定
void DrawPlayer(void) {
	if (g_player.bDisp == false) {
		return;
	}

	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ
	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPlayer = 0; nCntPlayer < NUM_PLAYER; nCntPlayer++) {
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer[nCntPlayer]);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
	}
}

Player* GetPlayer(void) {
	return &g_player;
}

int Godnum(void) {
	return nGod;
}

void Command(void) {
	//God =================================================================
	if ((nGod == 0) && (GetPadTap(XINPUT_GAMEPAD_A) == true)) {
		nGod++;;
	}
	//---------------------------------------------------------
	else if ((nGod == 1) && (GetPadTap(XINPUT_GAMEPAD_B) == true)) {
		nGod++;
	}
	//---------------------------------------------------------
	else if ((nGod == 2) && (GetPadTap(XINPUT_GAMEPAD_A) == true)) {
		nGod++;
	}
	//---------------------------------------------------------
	else if ((nGod == 3) && (GetPadTap(XINPUT_GAMEPAD_B) == true)) {
		nGod++;
	}
	//---------------------------------------------------------
	else if ((nGod == 4) && (GetPadTap(XINPUT_GAMEPAD_X) == true)) {
		nGod++;
	}
	//---------------------------------------------------------
	else if ((nGod == 5) && (GetPadTap(XINPUT_GAMEPAD_Y) == true)) {
		nGod++;
	}
	//---------------------------------------------------------
	else if ((nGod == 6) && (GetPadTap(XINPUT_GAMEPAD_DPAD_UP) == true)) {
		nGod++;
		GodMode = GodMode ? false: true;
	}
	else if ((GetPadTap(XINPUT_GAMEPAD_A) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_B) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_X) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_Y) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_DPAD_UP) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_DPAD_DOWN) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_DPAD_LEFT) == true) ||
			 (GetPadTap(XINPUT_GAMEPAD_DPAD_RIGHT) == true)) {
		nGod = 0;
	}


	//=====================================================================
}

bool GetGod(void) {
	return GodMode;
}

void HitPlayer(int nDamage) {
	Wave* pWave = GetWave();

	g_player.nLife -= nDamage;

	if (g_player.nUpgrade > 0) {
		g_player.nUpgrade--;
		if (g_player.nUpgrade < 0) {
			g_player.nUpgrade = 0;
		}
	}

	OnPadVibration();

	if (pWave->g_nWave == 1) {
		AddScore(-100);
	}
	else if (pWave->g_nWave == 2) {
		AddScore(-200);
	}
	else if (pWave->g_nWave == 3) {
		AddScore(-300);
	}
	else if (pWave->g_nWave == 4) {
		AddScore(-400);
	}
	else if (pWave->g_nWave == 5) {
		AddScore(-500);
	}

	if (g_player.nLife <= 0) {
		SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));

		g_player.bDisp = false;
		g_player.state = PLAYERSTATE_DEATH;
		g_player.nCounterState = 1;
	}
	else {
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 10;

		VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		if (GodMode == true) {
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 0);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 0);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 0);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 0);

			pVtx[4].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[5].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[6].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[7].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		}
		else if (GodMode == false) {
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

			pVtx[4].col = D3DCOLOR_RGBA(255, 0, 0, 0);
			pVtx[5].col = D3DCOLOR_RGBA(255, 0, 0, 0);
			pVtx[6].col = D3DCOLOR_RGBA(255, 0, 0, 0);
			pVtx[7].col = D3DCOLOR_RGBA(255, 0, 0, 0);
		}
		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}
}

int GetPlayerLife(void) {
	return g_player.nLife;
}