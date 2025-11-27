#include <stdio.h>
#include <stdlib.h>
#include "audio.h"
#include "main.h"
#include "header.h"
#define SPRITES_NUMER 4
#define BUFFER_SIZE 160

C2D_Sprite sprites[32];	//画像用
static C2D_SpriteSheet spriteSheet;
C2D_TextBuf g_dynamicBuf;
C2D_Text dynText;
int scene_state = 0;
bool isLeft = false, isRight = false, isDouble = false, isUp = false;
char buffer[BUFFER_SIZE];
LIST_T List[LIST_MAX];
NOTES_T Notes[NOTES_MAX];

inline void draw_debug(float x, float y, const char *text) {

	//使用例
	//snprintf(get_buffer(), BUFFER_SIZE, "%d", 10);
	//draw_debug(300, 0, get_buffer());

	C2D_TextBufClear(g_dynamicBuf);
	C2D_TextParse(&dynText, g_dynamicBuf, text);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor, x, y, 0.5f, 0.5f, 0.5f, C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f));
}

/*inline void draw_text(float x, float y, const char *text, float r, float g, float b) {

	//使用例
	//snprintf(get_buffer(), BUFFER_SIZE, "%d", 10);
	//draw_debug(300, 0, get_buffer());

	C2D_TextBufClear(g_dynamicBuf);
	C2D_TextParse(&dynText, g_dynamicBuf, text);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor | C2D_AlignCenter, x, y, 0.5f, 0.5f, 0.5f, C2D_Color32f(r, g, b, 1.0f));
}

inline int ctoi(char c) {

	switch (c) {
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	default: return 0;
	}
}

inline void load_file_main() {

	chdir(DEFAULT_DIR);
	load_file_list(DEFAULT_DIR);
	SongNumber = SongCount;
}*/

char *get_buffer() {
	return buffer;
}

inline void notes_button(unsigned int key) {

	if (key & KEY_A || key & KEY_B || key & KEY_X || key & KEY_Y) {
		isRight = true;
		audioPlay(0);
	}
	if (key & KEY_DUP || key & KEY_DDOWN || key & KEY_DLEFT || key & KEY_DRIGHT) {
		isLeft = true;
		audioPlay(1);
	}
	if (isLeft && isRight) {
		isLeft = false, isRight = false, isDouble = true;
	}
	if (key & KEY_L || key & KEY_R || key & KEY_ZL || key & KEY_ZR) {
		isUp = true;
		audioPlay(3);
	}
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
	unsigned int key, keyhold;

	while (aptMainLoop()) {

		hidScanInput();
		hidTouchRead(&tp);
		key = hidKeysDown(), keyhold = hidKeysHeld();
		notes_button(key);
		isLeft = false, isRight = false, isUp = false;
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
			spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
			if (!spriteSheet) svcBreak(USERBREAK_PANIC);
			for (int i = 0, j = SPRITES_NUMER; i < j; ++i) {
				C2D_SpriteFromSheet(&sprites[i], spriteSheet, i);
				C2D_SpriteSetCenter(&sprites[i], 0.5f, 0.5f);
				C2D_SpriteSetPos(&sprites[i], BOTTOM_WIDTH * 0.5, BOTTOM_HEIGHT * 0.5);
			}
			scene_state = 1;
			break;

		case 1:	//SOUNDTEST

			draw_debug(0, 0, "SOUNDTEST");
			notes_button(key);
			if (isRight) C2D_DrawImage(sprites[0].image, &sprites[0].params, NULL);
			if (isLeft) C2D_DrawImage(sprites[1].image, &sprites[1].params, NULL);
			if (isDouble) C2D_DrawImage(sprites[2].image, &sprites[2].params, NULL);
			if (isUp) C2D_DrawImage(sprites[3].image, &sprites[3].params, NULL);
			break;

		case 2:	//NOTESTEST

			notes_button(key);
			
			break;
		}
		C3D_FrameEnd(0);
		isDouble = false;
	}
	C2D_TextBufDelete(g_dynamicBuf);

	audioExit();
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	exit(0);
}
