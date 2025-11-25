#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAMPLE_RATE  32000
#define BUFFER_SIZE  0x6144
#define SOUND_NUMBER 4

static ndspWaveBuf waveBuf[SOUND_NUMBER];
static u8* audioBuffer[SOUND_NUMBER] = {NULL};
static bool isPlaying = false;

bool audioInit(void) {

	ndspInit();
	ndspSetOutputCount(1);
	char sound_address[SOUND_NUMBER][20] = {
		"romfs:/1.wav",
		"romfs:/2.wav",
		"romfs:/3.wav",
		"romfs:/4.wav",
	};

	for (int i = 0; i < SOUND_NUMBER; ++i) {
		FILE* file = fopen(sound_address[i], "rb");
		if (!file) {
			printf("Could not open file: %s\n", sound_address[i]);
			return false;
		}

		// Skip WAV header (44 bytes)
		fseek(file, 44, SEEK_SET);

		// Allocate buffer
		audioBuffer[i] = linearAlloc(BUFFER_SIZE);
		if (!audioBuffer[i]) {
			printf("Failed to allocate audio buffer.\n");
			fclose(file);
			return false;
		}

		// Read audio data
		size_t size = fread(audioBuffer[i], 1, BUFFER_SIZE, file);
		fclose(file);

		if (size == 0) {
			printf("No audio data read.\n");
			linearFree(audioBuffer);
			return false;
		}

		// Configure NDSP
		ndspChnReset(i);
		ndspChnSetInterp(i, NDSP_INTERP_LINEAR);
		ndspChnSetRate(i, SAMPLE_RATE);
		ndspChnSetFormat(i, NDSP_FORMAT_MONO_PCM16);

		// Set up wave buffer
		memset(&waveBuf, 0, sizeof(ndspWaveBuf[i]));
		waveBuf.data_vaddr = audioBuffer[i];
		waveBuf.nsamples = size / 2; // 1 channels * 16-bit samples
		waveBuf.looping = false;
		DSP_FlushDataCache(audioBuffer[i], size);
	}
	return true;
}

bool audioPlay(int num) {
	ndspChnWaveBufClear(num);
	ndspChnWaveBufAdd(num, &waveBuf[num]);
	return true;
}

void audioExit(void) {
	for (int i = 0; i < SOUND_NUMBER; ++i) {
		linearFree(audioBuffer[i]);
		audioBuffer[i] = NULL;
	}
	ndspExit();
}