#include <stdio.h>
#include "video.h"
#include "chip8.h"
#include <time.h>

int main(int argc, char** argv) {
	const char* fname = argv[1];

	int videoScale = 10;
	int cycleDelay = 1;
	//printf("%i %i %s %s %s\n", videoScale, cycleDelay, argv[0], argv[1], fname);
	
	ch_video platform;
	initialize_video(&platform, fname, VIDEO_WIDTH*videoScale, VIDEO_HEIGHT*videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	chip_8 emulator;
	ch_initialize(&emulator);
	ch_loadRom(&emulator, fname);

	int videoPitch = sizeof(emulator.video[0]) * VIDEO_WIDTH;
	struct timespec last_cycle;
	clock_gettime(CLOCK_MONOTONIC, &last_cycle);

	while (!proc_input(emulator.keypad)) {
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		double dt = (now.tv_sec - last_cycle.tv_sec) * 1000.0f + (now.tv_nsec - last_cycle.tv_nsec) / 1000000.0f;

		if (dt > cycleDelay) {
			last_cycle = now;
			ch_cycle(&emulator);
			update_video(&platform, emulator.video, videoPitch);
		}
	}

	deinit_video(&platform);
	return 0;


}
