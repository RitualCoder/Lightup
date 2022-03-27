/**
 * @file SDLUtils.h
 * @author Guillaume Dindart (you@domain.com)
 * @brief Basic SDL helper function for lightup game
 * @version 0.2
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SDLUTILS_H__
#define __SDLUTILS_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../libgame/game.h"
#include "env.h"

#define WINDOW_NAME "Lightup"
#define SPACE 20
#define SPRITE_SIZE 64
#define WINDOW_HEIGHT 800
#define WINDOW_LENGHT 600
#define MENU_X 250
#define MENU_Y 160

void SDL_initAndSetName(SDL_Renderer** pRenderer, SDL_Window** pWindow);
void printDebug(SDL_Renderer* pRenderer, SDL_Window* pWindow);
void loadWallTexture(game_env genv, SDL_Renderer* pRenderer);
void update_genv(game_env genv, game g);
bool process(SDL_Event event, SDL_Window* pWindow, game_env genv, game g);
void render(game_env genv, SDL_Renderer* pRenderer, SDL_Window* pWindow, double fps, game g);
SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, game_env genv,
                                    SDL_Color textColor);
void render_level_menu(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], SDL_Texture* txt[],
                       int nbLevel);

bool level_process(SDL_Event event, SDL_Window* pWindow, game_env genv, game* g);
void clean_texture_tab(SDL_Texture* tab[], int nbItem);
void draw_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, SDL_Texture* items[], int nbItem);
bool menu_process(SDL_Event event, SDL_Window* pWindow, int nbItem, game* g, SDL_Renderer* pRenderer, game_env genv);
double deltaTime(struct timeval start, struct timeval end);

#endif