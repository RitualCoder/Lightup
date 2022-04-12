#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

#include "libSDLUtils/SDLMenu.h"
#include "libSDLUtils/SDLUtils.h"
#include "libSDLUtils/env.h"
#include "libgame/game.h"
#include "libgame/game_aux.h"
#include "libgame/game_tools.h"
#include "libgame/game_random.h"

int main(int argc, char* argv[]) {
    // load game in passed in parametter
    game_env genv = init_game_environment();
    game g;
    if (argc == 2) {
        g = game_load(argv[1]);
        genv->state = "game";
    } else {
        g = game_random(MIN + rand() % (MAX + 1 - MIN), MIN + rand() % (MAX + 1 - MIN), false, 10, false);
    }
    update_genv(genv, g);

    // init sdl system
    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;

    double fps = 0;
    bool main_loop_run = true;

    init(&pRenderer, &pWindow, g, genv);

    while (main_loop_run) {
        fprintf(stderr, "state: %s\n", genv->state);
        if (strcmp("main_menu", genv->state) == 0) {
            g = main_menu(pRenderer, pWindow, genv);
        }

        else if (strcmp("level_sel", genv->state) == 0) {
            g = level_menu(pRenderer, pWindow, genv);
        }

        else if (strcmp("game", genv->state) == 0) {
            game_loop(pRenderer, pWindow, genv, &g, fps);
        }

        else if (strcmp("exit", genv->state) == 0) {
            main_loop_run = false;
        }
    }

    quit(genv);
    SDL_end(pRenderer, pWindow);

    return EXIT_SUCCESS;
}