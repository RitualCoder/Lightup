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

#define MENU_X 250
#define MENU_Y 160

game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);

game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);
#endif