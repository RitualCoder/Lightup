/**
 * @file SDLMenu.h
 * @author Guillaume Dindart (guillaume.dindart@etu.u-bordeaux.fr)
 * @author Enzo Cornaggia (enzo.cornaggia@etu.u-bordeaux.fr)
 * @brief
 * @version 0.1
 * @date 2022-03-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SDLMENU_H__
#define __SDLMENU_H__

#include <SDL.h>
#include <stdbool.h>

#include "../libgame/game.h"
#include "env.h"

#define WHITE \
    { 255, 255, 255, 255 }
#define BLACK \
    { 0, 0, 0, 255 }

/**
 * @brief Init base of SDL2 and game environement
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 Window
 * @param g A game instance
 * @param genv The game environment variable for SDL2 rendering
 * @return True All good
 * @return False problem append
 */
bool init(SDL_Renderer** pRenderer, SDL_Window** pWindow, game g, game_env genv);

/**
 * @brief Init the game_env
 *
 * @return The game environment game_env
 */
game_env init_game_environment();

/**
 * @brief The main game loop
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 * @param genv Game environment
 * @param g Instance of playing game
 * @param fps An fps counter (Only in DEBUG mode)
 */
void game_loop(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv, game* g, double fps);

/**
 * @brief A level selection menu
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 * @param genv The game environment
 * @return Return the game selected by the user
 */
game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);

/**
 * @brief Main menu loop
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 * @param genv The game environment
 * @return The game
 */
game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv);

/**
 * @brief Destroy pRenderer and pWindow
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 */
void SDL_end(SDL_Renderer* pRenderer, SDL_Window* pWindow);

/**
 * @brief Free the game environment structure
 *
 * @param genv The game environment
 */
void quit(game_env genv);

#endif