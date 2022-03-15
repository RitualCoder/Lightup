#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "libSDLUtils/SDLUtils.h"
#include "libSDLUtils/env.h"
#include "libgame/game.h"
#include "libgame/game_aux.h"

static double deltaTime(struct timeval start, struct timeval end) {
    return (end.tv_sec - end.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
}

int main(int argc, char* argv[]) {
    game g = game_default_solution();

    SDL_Window* pWindow = NULL;
    SDL_Renderer* pRenderer = NULL;
    game_env genv = init_game_environment(g);
    bool run;
    double fps = 0;
    SDL_Event event;

    run = init(&pRenderer, &pWindow, g, genv);

    // game loop
    while (run) {
        // fps start frame
        gettimeofday(&genv->startTime, NULL);

        while (SDL_PollEvent(&event)) {  // process input
            run = porcess(event, pWindow, genv);
        }

        // game logic
        render(genv, pRenderer, fps, g);
    }

    quit(genv);
    SDL_end(pRenderer, pWindow);

    return EXIT_SUCCESS;
}