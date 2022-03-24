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

const SDL_MessageBoxButtonData buttons[] = {
    {/* .flags, .buttonid, .text */ 0, 0, "menu"},
    {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "retry"},
    {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "exit"},
};

const SDL_MessageBoxData messageboxdata = {
    SDL_MESSAGEBOX_INFORMATION, /* .flags */
    NULL,                       /* .window */
    "You win !",                /* .title */
    "select a button",          /* .message */
    SDL_arraysize(buttons),     /* .numbuttons */
    buttons,                    /* .buttons */
    NULL                        /* .colorScheme */
};

int game_loop(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv, game g, double fps) {
    int buttonid;
    bool run = true;
    SDL_Event event;
    update_genv(genv, g);

    while (run) {
        // fps start frame
        gettimeofday(&genv->startTime, NULL);

        while (SDL_PollEvent(&event)) {  // process input
            run = process(event, pWindow, genv, g);
            if (!run) {
                break;
            }
        }

        // game Render
        render(genv, pRenderer, fps, g);
        if (game_is_over(g)) {
            if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
                SDL_Log("error displaying message box");
            }
            if (buttonid == 0) {
                SDL_Log("Menu");
                run = false;
                genv->state = "main_menu";
            }
            if (buttonid == 1) {
                SDL_Log("Retry");
                game_restart(g);
            }
            if (buttonid == 2) {
                SDL_Log("EXIT");
                genv->state = "exit";
                run = false;
            } else {
                SDL_Log("selection was %s", buttons[buttonid].text);
            }
        }
    }

    return 0;
}

int main(int argc, char* argv[]) {
    // load game in passed in parametter
    game_env genv = init_game_environment();
    game g;
    if (argc == 2) {
        g = game_load(argv[1]);
        genv->state = "game";
    } else {
        g = game_default();
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
            game_loop(pRenderer, pWindow, genv, g, fps);
        }

        else if (strcmp("exit", genv->state) == 0) {
            main_loop_run = false;
        }
    }

    quit(genv);
    SDL_end(pRenderer, pWindow);

    return EXIT_SUCCESS;
}