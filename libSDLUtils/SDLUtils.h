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

/**
 * @brief Init the base of Window and renderer
 *
 * @param pRenderer An SDL2 Renderer
 * @param pWindow An SDL2 Windows
 */
void SDL_initAndSetName(SDL_Renderer** pRenderer, SDL_Window** pWindow);

/**
 * @brief Print an SDL2 error
 *
 * @param pRenderer An SDL2 Renderer
 * @param pWindow An SDL2 Windows
 */
void printDebug(SDL_Renderer* pRenderer, SDL_Window* pWindow);

/**
 * @brief load wall texture form directory img
 *
 * @param genv the game environement
 * @param pRenderer An SDL2 Renderer
 */
void loadWallTexture(game_env genv, SDL_Renderer* pRenderer);

/**
 * @brief updage the game environement with the actual planying game
 *
 * @param genv the game environement
 * @param g actuall game instance
 */
void update_genv(game_env genv, game g);

/**
 * @brief procese of main game_loop
 *
 * @param event Envent struct (SDL2)
 * @param pWindow An SDL2 Windows
 * @param genv the game environement
 * @param g actuall game instance
 * @return true Continue the loop
 * @return false Stop loop
 */
bool process(SDL_Event event, SDL_Window* pWindow, game_env genv, game g);

/**
 * @brief Render the main game
 *
 * @param genv the game environement
 * @param pRenderer An SDL2 Renderer
 * @param pWindow An SDL2 Windows
 * @param fps An fps counter (Only in debug mode)
 * @param g actuall game instance
 */
void render(game_env genv, SDL_Renderer* pRenderer, SDL_Window* pWindow, double fps, game g);

/**
 * @brief convert a string tab to a texture tab
 *
 * @param pRenderer An SDL2 Renderer
 * @param text strings tab
 * @param nbtext number of strings
 * @param genv the game environement
 * @param textColor color of the text
 * @return SDL_Texture** the texture tab
 */
SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, game_env genv,
                                    SDL_Color textColor);

/**
 * @brief reder the level selection menu
 *
 * @param genv the game environement
 * @param pRenderer An SDL2 Renderer
 * @param level_tex texture of level
 * @param txt level text
 * @param nbLevel number of level
 */
void render_level_menu(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], SDL_Texture* txt[],
                       int nbLevel);

/**
 * @brief event gestion of level selection
 *
 * @param event Event structure (SDL2)
 * @param pWindow An SDL2 Windows
 * @param genv the game environement
 * @param g actual gagame instace
 * @return true continue
 * @return false stop selection
 */
bool level_process(SDL_Event event, SDL_Window* pWindow, game_env genv, game* g);

/**
 * @brief clean a tab of texture
 *
 * @param tab the texture tab
 * @param nbItem number of texture in tab
 */
void clean_texture_tab(SDL_Texture* tab[], int nbItem);

/**
 * @brief Draw the main menu
 *
 * @param pRenderer An SDL2 Renderer
 * @param pWindow An SDL2 Windows
 * @param items texture of menu item
 * @param nbItem number of item in menu
 */
void draw_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, SDL_Texture* items[], int nbItem);

/**
 * @brief Event gestion of main menu
 *
 * @param event Envent struct (SDL2)
 * @param pWindow An SDL2 Windows
 * @param nbItem Number of item
 * @param g pointer to next game instance
 * @param pRenderer An SDL2 Renderer
 * @param genv the game environement
 * @return true continue
 * @return false stop
 */
bool menu_process(SDL_Event event, SDL_Window* pWindow, int nbItem, game* g, SDL_Renderer* pRenderer, game_env genv);

/**
 * @brief calculate a delta time in ms
 *
 * @param start start time
 * @param end end time
 * @return double
 */
double deltaTime(struct timeval start, struct timeval end);

#endif