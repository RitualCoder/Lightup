/**
 * @file SDLMenu.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SDLMENU_H__
#define __SDLMENU_H__

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../libgame/game.h"
#include "env.h"

#define WHITE \
    { 255, 255, 255, 0 }
#define BLACK \
    { 0, 0, 0, 0 }

bool init(SDL_Renderer** pRenderer, SDL_Window** pWindow, game g, game_env genv);
void game_loop(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv, game g, double fps);
game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);
game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);
void SDL_end(SDL_Renderer* pRenderer, SDL_Window* pWindow);
void quit(game_env genv);
game_env init_game_environment();
#endif