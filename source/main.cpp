#include <stdio.h>
#include <stdlib.h>
#include "audio.h"
#include "header.h"
#define BUFFER_SIZE 160

C2D_Sprite sprites[32];	//画像用
static C2D_SpriteSheet spriteSheet;
C2D_TextBuf g_dynamicBuf;
C2D_Text dynText;
int scene_state = 0;

void draw_debug(float x, float y, const char *text) {

	//使用例
	//snprintf(get_buffer(), BUFFER_SIZE, "%d", 10);
	//draw_debug(300, 0, get_buffer());

	C2D_TextBufClear(g_dynamicBuf);
	C2D_TextParse(&dynText, g_dynamicBuf, text);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor, x, y, 0.5f, 0.5f, 0.5f, C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f));
}

int main() {

	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	g_dynamicBuf = C2D_TextBufNew(4096);
	audioInit();
	touchPosition tp;	//下画面タッチした座標

	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	while (aptMainLoop()) {

		hidScanInput();
		hidTouchRead(&tp);
		unsigned int key = hidKeysDown(), keyhold = hidKeysHeld();
		if (keyhold == KEY_START) break;
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		//上画面
		C2D_TargetClear(top, C2D_Color32(0x42, 0x42, 0x42, 0xFF));
		C3D_FrameDrawOn(top);
		C2D_SceneTarget(top);

		switch (scene_state) {

		case 0:	//ロード画面

			snprintf(get_buffer(), BUFFER_SIZE, "SuperConga v%s", VERSION);
			draw_debug(120, 70, get_buffer());
			draw_debug(120, 100, "Now Loading...");
			C3D_FrameEnd(0);
			scene_state = 0;
			if (key == KEY_A) audioPlay(0);
			break;

		case 1:	//test

			draw_debug(0, 0, debug ok);
			break;
		}
		C3D_FrameEnd(0);
	}
	audioExit()
	exit(0);
}

char *get_buffer() {
	return buffer;
}
