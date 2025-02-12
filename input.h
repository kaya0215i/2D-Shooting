#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

//プロトタイプ宣言
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);

HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetPadPress(int pad);
bool GetPadTap(int pad);
bool GetPadLstickX(int pad);
bool GetPadLstickY(int pad);
bool GetPadRstickX(int pad);
bool GetPadRstickY(int pad);
bool GetPadLtrigger(void);
bool GetPadRtrigger(void);
void OnPadVibration(void);
#endif
