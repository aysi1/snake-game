#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<SDL2/SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

typedef struct {
	SDL_Rect* body;
	SDL_Point last_pos;
	int speed, allc;
	short direct, score;
} snake;

void init_snake(snake* s) {
	s->body = (SDL_Rect*) calloc(32, sizeof(SDL_Rect));
	s->allc=2;
	int i;
	for (i=0; i<2 ; i++) {
		s->body[i].x = 20*i;
		s->body[i].y = 300;
		s->body[i].w = 20;
		s->body[i].h = 20;
	}
	s->last_pos.x=s->body[s->allc-1].x;
	s->last_pos.y=s->body[s->allc-1].y;
	s->direct = 3;
	s->speed=5;
}

void handle_snake_motion(snake* s) {
	s->body[s->allc-2].x = s->last_pos.x;
	s->body[s->allc-2].y = s->last_pos.y;
	for (int i=0; i<s->allc-2; i++)
		s->body[i]=s->body[i+1];
	s->last_pos.x = s->body[s->allc-1].x;
	s->last_pos.y = s->body[s->allc-1].y;
}

void gen_food(SDL_Rect* food) {
	food->x = 20 * (rand()%30);
	food->y = 20 * (rand()%30);
}

void handle_snake_action(snake* s, SDL_Rect* food) {
	if (s->body[s->allc-1].x != food->x || s->body[s->allc-1].y != food->y) return;
	s->score+=1000;
	if (s->speed < 20) s->speed++;
	s->allc++;
	s->body[s->allc-1].w = 20;
	s->body[s->allc-1].h = 20;
	s->body[s->allc-1].x = s->body[s->allc-2].x;
	s->body[s->allc-1].y = s->body[s->allc-2].y;
	handle_snake_motion(s);
	gen_food(food);
}

void draw_map(SDL_Renderer* r) {
	SDL_SetRenderDrawColor(renderer, 0x7f, 0x7f, 0x7f, 0xff);
	for (int i=1; i<30; i++) {
		SDL_RenderDrawLine(r, i*20, 0, i*20, 600);
		SDL_RenderDrawLine(r, 0, i*20, 600, i*20);
	}
}

int init() {
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_RESIZABLE);
	if (!window) return -1;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) return -2;
	return 0;
}

int main(int argc, char* argv[]) {
	if (init() < 0) {
		printf("%s\n", SDL_GetError());
		return -1;
	}
	srand(time(NULL));
	SDL_Event e;
	snake* sk = (snake*) calloc(1, sizeof(snake));
	init_snake(sk);
	SDL_Rect food;
	food.w = 20;
	food.h = 20;
	gen_food(&food);
	while (e.type != SDL_QUIT) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				switch(e.key.keysym.sym) {
					case SDLK_UP: sk->direct=1; break;
					case SDLK_DOWN: sk->direct=2; break;
					case SDLK_RIGHT: sk->direct=3; break;
					case SDLK_LEFT: sk->direct=4; break;
				}
			}
		}
		SDL_Delay(500*1.0/sk->speed);
		switch (sk->direct) {
			case 1: sk->body[sk->allc-1].y-=20; break;
			case 2: sk->body[sk->allc-1].y+=20; break;
			case 3: sk->body[sk->allc-1].x+=20; break;
			case 4: sk->body[sk->allc-1].x-=20; break;
		}
		handle_snake_motion(sk);
		handle_snake_action(sk, &food);
		SDL_RenderClear(renderer);
		draw_map(renderer);
		SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
		SDL_RenderFillRect(renderer, &food);
		SDL_SetRenderDrawColor(renderer, 0, 0x7f, 0xff, 0xff);
		SDL_RenderFillRects(renderer, sk->body, sk->allc);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
		SDL_RenderPresent(renderer);
	
	}
	free(sk);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
