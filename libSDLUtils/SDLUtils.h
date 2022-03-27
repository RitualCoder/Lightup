/**
 * @file SDLUtils.h
 * @author Guillaume Dindart (guillaume.dindart@etu.u-bordeaux.fr)
 * @author Enzo Cornaggia (enzo.cornaggia@etu.u-bordeaux.fr)
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
 * @brief Calculate a delta time in ms
 *
 * @param start Start time
 * @param end End time
 * @return A double
 */
double deltaTime(struct timeval start, struct timeval end);

/**
 * @brief Update the title of the window
 *
 * @param pWindow An SDL2 window
 * @param genv The game environment
 * @return A double
 */
void update_title_Window(SDL_Window* pWindow, game_env genv);

/**
 * @brief Init the base of window and renderer
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 */
void SDL_initAndSetName(SDL_Renderer** pRenderer, SDL_Window** pWindow);

/**
 * @brief Load wall texture from directory img
 *
 * @param genv The game environment
 * @param pRenderer An SDL2 renderer
 */
void loadWallTexture(game_env genv, SDL_Renderer* pRenderer);

/**
 * @brief Print an SDL2 error
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 */
void printDebug(SDL_Renderer* pRenderer, SDL_Window* pWindow);

/**
 * @brief Render the main game
 *
 * @param genv The game environement
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 * @param fps An fps counter (Only in debug mode)
 * @param g Actual game instance
 */
void render(game_env genv, SDL_Renderer* pRenderer, SDL_Window* pWindow, double fps, game g);

/**
 * @brief procese of main game_loop
 *
 * @param event Event struct (SDL2)
 * @param pWindow An SDL2 window
 * @param genv The game environement
 * @param g Actual game instance
 * @return True then continue the loop
 * @return False then stop the loop
 */
bool process(SDL_Event event, SDL_Window* pWindow, game_env genv, game g);

/**
 * @brief Update the game environment with the actual playing game
 *
 * @param genv The game environment
 * @param g Actual game instance
 */
void update_genv(game_env genv, game g);

/**
 * @brief Convert a string tab to a texture tab
 *
 * @param pRenderer An SDL2 renderer
 * @param text Strings array
 * @param nbtext Number of strings
 * @param genv The game environment
 * @param textColor Color of the text
 * @return SDL_Texture** the texture array
 */
SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, game_env genv,
                                    SDL_Color textColor);

/**
 * @brief Clean a tab of texture
 *
 * @param tab The texture array
 * @param nbItem Number of texture in array
 */
void clean_texture_tab(SDL_Texture* tab[], int nbItem);

/**
 * @brief Render the level selection menu
 *
 * @param genv The game environment
 * @param pRenderer An SDL2 renderer
 * @param level_tex Texture of level array
 * @param txt Level text array
 * @param nbLevel Number of level
 */
void render_level_menu(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], SDL_Texture* txt[],
                       int nbLevel);

/**
 * @brief Draw the main menu
 *
 * @param pRenderer An SDL2 renderer
 * @param pWindow An SDL2 window
 * @param items Texture of the menu item
 * @param nbItem Number of the item in menu
 */
void draw_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, SDL_Texture* items[], int nbItem);

/**
 * @brief Event gestion of level selection
 *
 * @param event Event structure (SDL2)
 * @param pWindow An SDL2 window
 * @param genv The game environment
 * @param g Actual game instance
 * @return True then continue
 * @return False then stop selection
 */
bool level_process(SDL_Event event, SDL_Window* pWindow, game_env genv, game* g);

/**
 * @brief Write all the text of an array of char with a font and color in a texture
 *
 * @param pRenderer An SDL2 renderer
 * @param text An array of character
 * @param pFont A font
 * @param nbtext The number of text
 * @param genv The game environment
 * @param textColor An SDL2 color
 */
SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, game_env genv,
                                    SDL_Color textColor);

/**
 * @brief Event gestion of the main menu page
 *
 * @param event Event struct (SDL2)
 * @param pWindow An SDL2 window
 * @param nbItem Number of item
 * @param g Pointer to the next game instance
 * @param pRenderer An SDL2 renderer
 * @param genv The game environment
 * @return True then continue
 * @return False then stop
 */
bool menu_process(SDL_Event event, SDL_Window* pWindow, int nbItem, game* g, SDL_Renderer* pRenderer, game_env genv);

/**
 * @brief Event gestion of the level selection page
 *
 * @param event Event struct (SDL2)
 * @param genv the game environment
 * @param g Pointer to the next game instance
 * @return True then continue
 * @return False then stop
 */
bool level_process(SDL_Event event, SDL_Window* pWindow, game_env genv, game* g);

#endif