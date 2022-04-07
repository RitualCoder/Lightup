#include "SDLMenu.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>

#include "../libgame/game.h"
#include "../libgame/game_tools.h"
#include "SDLUtils.h"
#include "env.h"

bool init(SDL_Renderer** pRenderer, SDL_Window** pWindow, game g, game_env genv) {
    // init time managment
    gettimeofday(&genv->startTime, NULL);

    // event init
    bool run = true;

    // window init
    SDL_initAndSetName(pRenderer, pWindow);

    // load font
    if (TTF_Init() == -1) {
        printf("[Error] TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    genv->pFont = TTF_OpenFont("04B_20__.ttf", 1000);

    // mouse
    SDL_PumpEvents();

    // Some debug info on Video Mode
    printDebug(*pRenderer, *pWindow);

    // load wall texture
    loadWallTexture(genv, *pRenderer);

    // Startup recap
    gettimeofday(&genv->endTime, NULL);
    SDL_Log("Startup finish in %.2lf ms", deltaTime(genv->startTime, genv->endTime));

    return run;
}

game_env init_game_environment() {
    game_env genv = malloc(sizeof(struct envS));
    genv->window_height = WINDOW_LENGHT;
    genv->windows_width = WINDOW_HEIGHT;
    genv->sprite_size = SPRITE_SIZE;
    genv->state = malloc(sizeof(char) * 12);
    genv->state = "main_menu";
    return genv;
}

void game_loop(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv, game* g, double fps) {
    int buttonid;
    int NxtLevel = genv->actualgame + 1;
    char buffer[20] = {"\0"};
    sprintf(buffer, "%d", NxtLevel);  // Int to Char in a buffer
    char nextLevel[50] = {"\0"};      // Stock the filename of the next level
    strcat(nextLevel, "level");
    strcat(nextLevel, buffer);
    strcat(nextLevel, ".txt");
    const SDL_MessageBoxButtonData buttons[] = {
        {/* .flags, .buttonid, .text */ 0, 0, "Menu"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Retry"},
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "Next level"},
    };
    SDL_MessageBoxData messageboxwin = {SDL_MESSAGEBOX_INFORMATION, pWindow, "You win !", "select a button",
                                        SDL_arraysize(buttons),     buttons, NULL};

    bool run = true;
    SDL_Event event;
    update_genv(genv, *g);

    while (run) {
        // fps start frame
        gettimeofday(&genv->startTime, NULL);

        while (SDL_PollEvent(&event)) {  // process input
            run = process(event, pWindow, genv, *g);
            if (!run) {
                break;
            }
        }
        update_title_Window(pWindow, genv);
        // game Render
        render(genv, pRenderer, pWindow, fps, *g);
        if (game_is_over(*g)) {
            if (SDL_ShowMessageBox(&messageboxwin, &buttonid) < 0) {
                SDL_Log("error displaying message box");
            }
            if (buttonid == 0) {
                SDL_Log("Menu");
                run = false;
                genv->state = "main_menu";
            }
            if (buttonid == 1) {
                SDL_Log("Retry");
                game_restart(*g);
            }
            if (buttonid == 2) {
                SDL_Log("Next Level");
                genv->state = "game";
                run = false;
                if (genv->actualgame == 9 || genv->actualgame == 0) {
                    genv->actualgame = 1;
                    *g = game_load("level1.txt");
                } else {
                    *g = game_load(nextLevel);
                    genv->actualgame = genv->actualgame + 1;
                }

            } else {
                SDL_Log("selection was %s", buttons[buttonid].text);
            }
            update_title_Window(pWindow, genv);
            update_genv(genv, *g);
        }
    }

    return;
}

game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv) {
    bool level_run = true;
    char* levels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char* txt_lvl[] = {"LEVEL 1", "LEVEL 2", "LEVEL 3", "LEVEL 4", "LEVEL 5",
                       "LEVEL 6", "LEVEL 7", "LEVEL 8", "LEVEL 9"};
    SDL_Color white = WHITE;
    SDL_Color black = BLACK;
    SDL_Texture** levels_tex = make_all_text_texture(pRenderer, levels, 9, genv, white);
    SDL_Texture** txt_lvl_tex = make_all_text_texture(pRenderer, txt_lvl, 9, genv, black);
    SDL_Event event;
    game g;
    genv->nb_cols = 5;
    genv->nb_rows = 5;

    while (level_run) {
        render_level_menu(genv, pRenderer, levels_tex, txt_lvl_tex, 9);
        while (SDL_PollEvent(&event)) {  // process input
            level_run = level_process(event, pWindow, genv, &g);
            if (!level_run) {
                break;
            }
        }
    }
    clean_texture_tab(levels_tex, 9);
    clean_texture_tab(txt_lvl_tex, 9);
    return g;
}

game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv) {
    bool run = true;
    char* items[] = {"NEW GAME", "   LOAD   ", "    QUIT    "};  // spaces are for not distorting the font
    SDL_Color white = WHITE;
    SDL_Texture** items_texture = make_all_text_texture(pRenderer, items, 3, genv, white);
    SDL_Event event;
    game g;

    while (run) {
        draw_menu(pRenderer, pWindow, items_texture, 3);
        while (SDL_PollEvent(&event)) {  // process input
            run = menu_process(event, pWindow, 3, &g, pRenderer, genv);
            if (!run) {
                break;
            }
        }
    }

    clean_texture_tab(items_texture, 3);

    return g;
}

void SDL_end(SDL_Renderer* pRenderer, SDL_Window* pWindow) {
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

void quit(game_env genv) {
    SDL_DestroyTexture(genv->lightbulb);
    SDL_DestroyTexture(genv->wall0);
    SDL_DestroyTexture(genv->wall1);
    SDL_DestroyTexture(genv->wall2);
    SDL_DestroyTexture(genv->wall3);
    SDL_DestroyTexture(genv->wall4);
    SDL_DestroyTexture(genv->wallu);
    SDL_DestroyTexture(genv->back);
    SDL_DestroyTexture(genv->help);
    free(genv);
}