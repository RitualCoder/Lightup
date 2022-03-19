/**
 * @file env.h
 * @author Guillaume Dindart (you@domain.com)
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
#include <sys/time.h>
// TODO, load texture!
/**
 * @brief
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
    Uint32 button;

    SDL_Texture* wallu;
    SDL_Texture* wall1;
    SDL_Texture* wall2;
    SDL_Texture* wall3;
    SDL_Texture* wall4;

    SDL_Texture* lighbulb;
    SDL_Texture* mark;
};

typedef struct envS* game_env;

#endif