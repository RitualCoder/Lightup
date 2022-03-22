#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../libgame/game.h"
#include "../libgame/game_aux.h"
#include "../libgame/game_tools.h"
#include "SDLUtils.h"
#include "env.h"

static void draw_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, SDL_Texture* items[], int nbItem) {
    int w, h;
    SDL_GetWindowSize(pWindow, &w, &h);

    int item_height = ((h - 60) - nbItem * 10) / nbItem;
    int item_width = (w - 60);

    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

    SDL_Rect rItem;
    rItem.w = item_width;
    rItem.h = item_height;
    rItem.x = 30;
    rItem.y = 30;
    for (int i = 0; i < nbItem; i++) {
        SDL_RenderDrawRect(pRenderer, &rItem);
        SDL_RenderFillRect(pRenderer, &rItem);

        SDL_Rect text;
        SDL_QueryTexture(items[i], NULL, NULL, &text.w, &text.h);
        if (text.w < rItem.w) {
            text.x = rItem.w / 2;
        } else {
            text.w = rItem.w;
            text.x = rItem.x;
        }

        if (text.h < rItem.h) {
            text.y = rItem.h / 2;
        } else {
            text.h = rItem.h;
            text.y = rItem.y;
        }

        SDL_RenderCopy(pRenderer, items[i], NULL, &text);

        rItem.y += item_height + 10;
    }

    SDL_RenderPresent(pRenderer);
    return;
}

static SDL_Texture* text_at_texture(SDL_Renderer* pRenderer, char* text, TTF_Font* pFont) {
    SDL_Surface* surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(pFont, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

static SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, game_env genv) {
    SDL_Texture** tab = malloc(sizeof(SDL_Texture*) * nbtext);

    for (int i = 0; i < nbtext; i++) {
        SDL_Texture* texture = text_at_texture(pRenderer, text[i], genv->pFont);
        tab[i] = texture;
    }

    return tab;
}
void SDL_DrawCaseCord(game_env genv, SDL_Renderer* pRenderer, int x, int y) {
    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    // creation of texture (black rectangle in this case)
    SDL_Rect rect;
    rect.h = genv->sprite_size;
    rect.w = genv->sprite_size;
    rect.x = start_x + genv->sprite_size * x;
    rect.y = start_y + genv->sprite_size * y;

    // draw and fill rectangle
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 128);
    SDL_RenderDrawRect(pRenderer, &rect);
    SDL_RenderFillRect(pRenderer, &rect);
}

static void SDL_Draw_level(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], int nbLevel) {
    int levelIndex = 0;
    for (int x = 0; x < genv->nb_cols; x++) {
        for (int y = 0; y < genv->nb_rows; y++) {
            if (x % 2 == 0 && y % 2 == 0) {
                SDL_DrawCaseCord(genv, pRenderer, y, x);
                draw_texture_at_pos(level_tex[levelIndex], pRenderer, genv, y, x);
                levelIndex++;
            }
        }
    }
}

static void render_level_menu(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], int nbLevel) {
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    int ret = SDL_RenderClear(pRenderer);
    if (ret != 0) {
        SDL_printError(true);
    }

    SDL_Draw_level(genv, pRenderer, level_tex, nbLevel);

    SDL_RenderPresent(pRenderer);
}

bool level_precesse(SDL_Event event, SDL_Window* pWindow, game_env genv, game* g) {
    SDL_GetWindowSize(pWindow, &genv->windows_width, &genv->window_height);
    if (genv->window_height < genv->windows_width) {
        genv->sprite_size = (genv->window_height - 60) / genv->nb_cols;
    } else {
        genv->sprite_size = (genv->windows_width - 60) / genv->nb_rows;
    }
    genv->button = SDL_GetMouseState(&genv->mouse_x, &genv->mouse_y);
    SDL_MouseToCase(genv);

    if (event.type == SDL_QUIT) {
        *g = NULL;
        return false;
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (genv->case_x % 2 == 0 && genv->case_y % 2 == 0) {
                int levelID = genv->case_x / 2 + (genv->case_y / 2) * 3;

                switch (levelID) {
                    case 0:
                        *g = game_load("levels/level1.txt");
                        return false;
                        break;
                    case 1:
                        *g = game_load("levels/level2.txt");
                        return false;
                        break;
                    case 2:
                        *g = game_load("levels/level3.txt");
                        return false;
                        break;
                    case 3:
                        *g = game_load("levels/level4.txt");
                        return false;
                        break;
                    case 4:
                        *g = game_load("levels/level5.txt");
                        return false;
                        break;
                    case 5:
                        *g = game_load("levels/level6.txt");
                        return false;
                        break;
                    case 6:
                        *g = game_load("levels/level7.txt");
                        return false;
                        break;
                    case 7:
                        *g = game_load("levels/level8.txt");
                        return false;
                        break;
                    case 8:
                        *g = game_load("levels/level9.txt");
                        return false;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return true;
}

game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv) {
    bool level_run = true;
    char* levels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    SDL_Texture** levels_tex = make_all_text_texture(pRenderer, levels, 9, genv);
    SDL_Event event;
    game g;
    genv->nb_cols = 5;
    genv->nb_rows = 5;

    while (level_run) {
        render_level_menu(genv, pRenderer, levels_tex, 9);
        while (SDL_PollEvent(&event)) {  // process input
            level_run = level_precesse(event, pWindow, genv, &g);
            if (!level_run) {
                break;
            }
        }
    }
    return g;
}

static bool menu_process(SDL_Event event, SDL_Window* pWindow, int nbItem, game* g, SDL_Renderer* pRenderer,
                         game_env genv) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    int w, h;
    SDL_GetWindowSize(pWindow, &w, &h);
    bool run = true;

    switch (event.type) {
        case SDL_QUIT:
            *g = NULL;
            run = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == (SDL_BUTTON_LEFT)) {
                if (mouse_x > 30 && mouse_x < w - 30 && mouse_y > 30 && mouse_y < h - 30) {
                    mouse_y -= 30;
                    int item_height = ((h - 60) - nbItem * 10) / nbItem;
                    int case_x = mouse_y / (item_height + 5);
                    if (case_x >= nbItem) {
                        case_x--;
                    }
                    switch (case_x) {
                        case 0:
                            *g = game_default();
                            break;
                        case 1:
                            *g = level_menu(pRenderer, pWindow, genv);
                            break;
                        case 2:
                            *g = NULL;
                            break;

                        default:
                            break;
                    }
                }
            }
            run = false;
            break;

        default:
            break;
    }
    return run;
}

static void clean_texture_tab(SDL_Texture* tab[], int nbItem) {
    for (int i = 0; i < nbItem; i++) {
        SDL_DestroyTexture(tab[i]);
    }
    free(tab);
}

game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv) {
    bool run = true;
    char* items[] = {"New game", " Load ", " Quit "};
    SDL_Texture** items_texture = make_all_text_texture(pRenderer, items, 3, genv);
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