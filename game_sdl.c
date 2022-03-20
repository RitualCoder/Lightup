#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "libSDLUtils/SDLMenu.h"
#include "libSDLUtils/SDLUtils.h"
#include "libSDLUtils/env.h"
#include "libgame/game.h"
#include "libgame/game_aux.h"
#include "libgame/game_tools.h"

int game_loop(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv, game g, double fps) {
    bool run = true;
    SDL_Event event;

    while (run) {
        // fps start frame
        gettimeofday(&genv->startTime, NULL);

        while (SDL_PollEvent(&event)) {  // process input
            run = process(event, pWindow, genv, g);
        }

        // game logic
        render(genv, pRenderer, fps, g);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    // load game in passed in parametter
    bool load_from_cmd = false;
    game g;
    if (argc == 2) {
        g = game_load(argv[1]);
        load_from_cmd = true;
    } else {
        g = game_default();
    }

    // init sdl system
    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;
    game_env genv = init_game_environment(g);
    TTF_Font* pFont;
    double fps = 0;

    init(&pRenderer, &pWindow, &pFont, g, genv);

    if (!load_from_cmd) {
        g = main_menu(pRenderer, pWindow, pFont);
    }

    if (g != NULL) {
        game_loop(pRenderer, pWindow, genv, g, fps);
    }

    quit(genv);
    SDL_end(pRenderer, pWindow);

    return EXIT_SUCCESS;
}