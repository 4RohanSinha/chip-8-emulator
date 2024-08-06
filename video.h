#ifndef VIDEO_H
#define VIDEO_H

#include <SDL.h>
#include <stdbool.h>

typedef struct { 
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
} ch_video;

void initialize_video(ch_video*, const char*, int, int, int, int);
void deinit_video(ch_video*);
void update_video(ch_video*, const void*, int);
bool proc_input(unsigned char*);

#endif
