#include "game.h"
#include "bg.h"
#include "player.h"
#include "player2.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "input.h"
#include "fade.h"
#include "effect.h"
#include "sound.h"
#include "pause.h"
#include "score.h"
#include "lifebar.h"
#include "enemylife.h"
#include "item.h"
#include "wavealert.h"

//グローバル宣言
GAMESTATE g_gameState = GAMESTATE_NONE; //ゲームの状態
int g_nCounterGameState = 0;            //状態管理カウンター
int g_nCounterWave;
int g_nCnt;
int g_nCounter;
bool g_bPause;
bool g_bSetEnemy;

Wave g_Wave;

void InitGame(void) {
	//背景の初期化処理
	InitBackground();

	//ポーズメニューの初期化
	InitPause();

	//プレイヤーの初期化処理
	InitPlayer();
	InitPlayer2();

	//敵の初期化処理
	InitEnemy();

	//弾の初期化処理
	InitBullet();

	//爆発の初期化処理
	InitExplosion();

	//エフェクトの初期化
	InitEffect();

	//スコアの初期化
	InitScore();

	InitLifebar();

	InitEnemyLife();

	InitWaveAlert();

	InitItem();

	g_gameState = GAMESTATE_NORMAL; //通常状態に設定
	g_nCounterGameState = 0;

	g_bPause = false;
	g_bSetEnemy = false;

	g_Wave.g_nWave = 1;
	g_Wave.g_bWaveMove = true;

	g_nCounter = 0;
	g_nCounterWave = 0;
	g_nCnt = 0;

	PlaySound(SOUND_LABEL_BGM5);
}

void UninitGame(void) {
	//背景の終了処理
	UninitBackground();

	//ポーズメニューの終了処理
	UninitPause();

	//プレイヤーの終了処理
	UninitPlayer();
	UninitPlayer2();

	//敵の終了処理
	UninitEnemy();

	//弾の終了処理
	UninitBullet();

	//爆発の終了処理
	UninitExplosion();
	
	//エフェクトの終了処理
	UninitEffect();

	//スコアの終了初期
	UninitScore();

	UninitLifebar();

	UninitEnemyLife();

	UninitWaveAlert();

	UninitItem();

	StopSound();
}

void UpdateGame(void) {
	if ((GetKeyboardTrigger(DIK_P) == true) || (GetPadTap(XINPUT_GAMEPAD_START) == true)) {
		g_bPause = g_bPause ? false : true;
	}

	if (g_bPause == true) {
		UpdatePause();
	}

	else if (g_bPause == false) {
		if (g_Wave.g_bWaveMove == true) {
			g_nCounterWave++;

			if ((g_nCounterWave % 3) == 0) {
				UpdateWaveAlert();
				g_nCnt++;

				if (g_nCnt == 55) {
					g_bSetEnemy = true;
				}
			}

			if (g_bSetEnemy == true) {
				if (g_Wave.g_nWave == 1) {
					SetEnemy(D3DXVECTOR3(200.0f, 450.0f, 0.0f), 5, NORMAL, 60, 3);
					SetEnemy(D3DXVECTOR3(800.0f, 600.0f, 0.0f), 3, NORMAL, 50, 5);
					SetEnemy(D3DXVECTOR3(500.0f, 600.0f, 0.0f), 0, NORMAL, 55, 5);
					SetEnemy(D3DXVECTOR3(400.0f, 550.0f, 0.0f), 2, NORMAL, 50, 5);
				}
				else if (g_Wave.g_nWave == 2) {
					SetEnemy(D3DXVECTOR3(200.0f, 450.0f, 0.0f), 0, SMALL, 30, 15);
					SetEnemy(D3DXVECTOR3(800.0f, 600.0f, 0.0f), 3, NORMAL, 70, 10);
					SetEnemy(D3DXVECTOR3(500.0f, 600.0f, 0.0f), 1, NORMAL, 70, 10);
					SetEnemy(D3DXVECTOR3(400.0f, 550.0f, 0.0f), 2, NORMAL, 70, 10);
				}
				else if (g_Wave.g_nWave == 3) {
					SetEnemy(D3DXVECTOR3(200.0f, 450.0f, 0.0f), 5, NORMAL, 120, 15);
					SetEnemy(D3DXVECTOR3(800.0f, 600.0f, 0.0f), 3, NORMAL, 120, 15);
					SetEnemy(D3DXVECTOR3(500.0f, 600.0f, 0.0f), 1, BIG, 200, 10);
					SetEnemy(D3DXVECTOR3(400.0f, 550.0f, 0.0f), 2, NORMAL, 120, 15);
				}
				else if (g_Wave.g_nWave == 4) {
					SetEnemy(D3DXVECTOR3(200.0f, 450.0f, 0.0f), 0, SMALL, 50, 20);
					SetEnemy(D3DXVECTOR3(800.0f, 600.0f, 0.0f), 3, SMALL, 50, 20);
					SetEnemy(D3DXVECTOR3(500.0f, 600.0f, 0.0f), 1, SMALL, 50, 20);
					SetEnemy(D3DXVECTOR3(400.0f, 550.0f, 0.0f), 2, SMALL, 50, 20);
				}
				else if (g_Wave.g_nWave == 5) {
					SetEnemy(D3DXVECTOR3(200.0f, 600.0f, 0.0f), 6, BIG, 300, 20);
					SetEnemy(D3DXVECTOR3(1200.0f, 590.0f, 0.0f), 3, SMALL, 40, 35);
					SetEnemy(D3DXVECTOR3(500.0f, 580.0f, 0.0f), 1, BIG, 250, 20);
					SetEnemy(D3DXVECTOR3(1000.0f, 550.0f, 0.0f), 2, NORMAL, 180, 30);
				}
				g_Wave.g_bWaveMove = false;
				g_bSetEnemy = false;
				g_nCounterWave = 0;
				g_nCnt = 0;
				ColorReset();
			}
		}

		else if (g_Wave.g_bWaveMove == false) {

			//背景の更新処理
			UpdateBackground();

			//プレイヤーの更新処理
			UpdatePlayer();
			UpdatePlayer2();

			//敵の更新処理
			UpdateEnemy();

			//弾の更新処理
			UpdateBullet();

			//爆発の更新処理
			UpdateExplosion();

			//エフェクトの更新処理
			UpdateEffect();

			//ライフバーの更新処理
			UpdateLifebar();
			UpdateEnemyLife();

			UpdateItem();

			if ((g_nCounter % 60) == 0) {
				if (g_Wave.g_nWave == 1) {
					if (GetScore() > 10) {
						AddScore(-10);
					}
				}
				else if (g_Wave.g_nWave == 2) {
					if (GetScore() > 20) {
						AddScore(-20);
					}
				}
				else if (g_Wave.g_nWave == 3) {
					if (GetScore() > 30) {
						AddScore(-30);
					}
				}
				else if (g_Wave.g_nWave == 4) {
					if (GetScore() > 40) {
						AddScore(-40);
					}
				}
				else if (g_Wave.g_nWave == 5) {
					if (GetScore() > 50) {
						AddScore(-50);
					}
				}
			}

			g_nCounter++;

			if ((g_nCounter % 100) == 0) {
				ResetCol();
			}

			switch (g_gameState) {
			case GAMESTATE_NORMAL: //通常状態
				break;

			case GAMESTATE_END:    //終了状態
				g_nCounterGameState++;

				if (g_nCounterGameState >= 30) {
					g_gameState = GAMESTATE_NONE; //何もしていない状態

					//画面の設定
					SetFade(MODE_RESULT);
				}
				break;
			}
		}
	}
}

void DrawGame(void) {
	//背景の描画処理
	DrawBackground();

	//エフェクトの描画処理
	DrawEffect();

	//弾の描画処理
	DrawBullet();

	//敵の描画処理
	DrawEnemy();

	//プレイヤーの描画処理
	DrawPlayer2();
	DrawPlayer();

	//爆発の描画処理
	DrawExplosion();

	//スコアの表示
	DrawScore();

	DrawLifebar();

	DrawEnemyLife();

	DrawWaveAlert();

	DrawItem();

	if (g_bPause == true) {
		//ポーズメニューの描画
		DrawPause();
	}
}

//ゲーム状態の設定
void SetGameState(GAMESTATE state) {
	g_gameState = state;

	g_nCounterGameState = 0;
}

//ゲーム状態の取得
GAMESTATE GetGameState(void) {
	return g_gameState;
}

void SetEnablePause(bool bPause) {
	g_bPause = bPause;
}

Wave* GetWave(void) {
	return &g_Wave;
}