/**
 * @file env.h
 * @author Guillaume Dindart (guillaume.dindart@etu.u-bordeaux.fr)
 * @author Enzo Cornaggia (enzo.cornaggia@etu.u-bordeaux.fr)
 * @brief En environement struct for lightup game
 * @version 0.2
 * @date 2022-03-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __ENV_H__
#define __ENV_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <sys/time.h>

/**
 * @brief The environment game structure
 *
 */
struct envS {
    struct timeval startTime;
    struct timeval endTime;
    int windows_width, window_height;
    int mouse_x, mouse_y;
    int case_x, case_y;
    int nb_rows, nb_cols;
    int sprite_size;
    int actualgame;
    Uint32 button;

    SDL_Texture* wallu;
    SDL_Texture* wall0;
    SDL_Texture* wall1;
    SDL_Texture* wall2;
    SDL_Texture* wall3;
    SDL_Texture* wall4;

    SDL_Texture* lightbulb;
    SDL_Texture* mark;

    SDL_Texture* back;
    SDL_Texture* help;

    TTF_Font* pFont;

    char* state;
};

typedef struct envS* game_env;

#endif