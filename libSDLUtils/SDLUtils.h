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

/**
 * @brief Init game environement
 *
 * @return game_env : the game environement
 */
game_env init_game_environment();

/**
 * @brief Print SDL erre and de-init if necessary
 *
 * @param init true if windows is init
 */
void SDL_printError(bool init);

/**
 * @brief init game and set windows name
 *
 * @param pRenderer a pointer for the renderer
 * @param pWindow a pointer for the window
 */
void SDL_initAndSetName(SDL_Renderer** pRenderer, SDL_Window** pWindow);

/**
 * @brief de-init SDL for propoer exit
 *
 * @param pRenderer a pointer for the renderer
 * @param pWindow a pointer for the window
 */
void SDL_end(SDL_Renderer* pRenderer, SDL_Window* pWindow);

/**
 * @brief Draw a grid on window, the size is defined by nb_cols and nb_rows
 *
 * @param genv game environment
 * @param pRenderer a pointer for the Renderer
 */
void SDL_drawGrid(game_env genv, SDL_Renderer* pRenderer);

/**
 * @brief Convert mouse coordinate to game internal coordinate
 *
 * @param genv game environment
 */
void SDL_MouseToCase(game_env genv);

/**
 * @brief Fill the case in black
 *
 * @param genv game environment
 * @param pRenderer a pointer for the Renderer
 */
void SDL_DrawCase(game_env genv, SDL_Renderer* pRenderer);

/**
 * @brief Draw back button top left corner
 *
 * @param genv game environment
 * @param pRenderer a pointer for the Renderer
 */ 
void SDL_Draw_back(game_env genv, SDL_Renderer* pRenderer);


/**
 * @brief print some debug information on stdout
 *
 * @param pRenderer the game renderer
 * @param pWindow the game windows
 */
void printDebug(SDL_Renderer* pRenderer, SDL_Window* pWindow);

/**
 * @brief
 *
 * @param genv
 * @param pRenderer
 * @param fps
 */
void render(game_env genv, SDL_Renderer* pRenderer, SDL_Window* pWindow, double fps, game g);

/**
 * @brief
 *
 * @param event
 * @param pWindow
 * @param genv
 * @return true
 * @return false
 */
bool process(SDL_Event event, SDL_Window* pWindow, game_env genv, game g);

void loadWallTexture(game_env genv, SDL_Renderer* pRenderer);

void quit(game_env genv);

bool init(SDL_Renderer** pRenderer, SDL_Window** pWindow, game g, game_env genv);

void draw_texture_at_pos(SDL_Texture* tex, SDL_Renderer* pRenderer, game_env genv, int y, int x);

void draw_number_level(SDL_Texture* tex, SDL_Renderer* pRenderer, game_env genv, int y, int x);

void update_genv(game_env genv, game g);

#endif