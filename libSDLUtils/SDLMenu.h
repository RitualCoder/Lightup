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

/**
 * @brief init the game_env
 *
 * @return game_env
 */
game_env init_game_environment();

/**
 * @brief Init base of SDL2 and game environement
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 Window
 * @param g a game instance
 * @param genv envorinment variable for SDL2 rendering
 * @return true All good
 * @return false A problem append
 */
bool init(SDL_Renderer** pRenderer, SDL_Window** pWindow, game g, game_env genv);

/**
 * @brief main game loop
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 Window
 * @param genv Game environment
 * @param g instance of playing game
 * @param fps An fps counter (Only in DEBUG mode)
 */
void game_loop(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv, game g, double fps);

/**
 * @brief A level selection menu
 *
 * @param pRenderer An SDL2 renderer An SDL2 renderer
 * @param pWindow An SDL2 Window
 * @param genv Game environment
 * @return Return the game selected by the user
 */
game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);

/**
 * @brief Main menu loop
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 Window
 * @param genv
 * @return game
 */
game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);

/**
 * @brief Deinit SDL
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 Window
 */
void SDL_end(SDL_Renderer* pRenderer, SDL_Window* pWindow);

/**
 * @brief Free the game environement structure
 *
 * @param genv
 */
void quit(game_env genv);
#endif