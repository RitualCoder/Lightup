#include "SDLMenu.h"

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
    int mx = w/4;
    int my = h/4;

    //int item_height = ((h - 60) - nbItem * 100) / nbItem;
    int item_height = (w/(nbItem+6));
    int item_width = (w/2);

    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);

    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

    SDL_Rect rItem;
    rItem.w = item_width;
    rItem.h = item_height;
    rItem.x = mx;
    rItem.y = my;
    for (int i = 0; i < nbItem; i++) {
        SDL_RenderFillRect(pRenderer, &rItem);

        SDL_Rect text;
        SDL_QueryTexture(items[i], NULL, NULL, &text.w, &text.h);
            text.w = rItem.w - SPACE*6;
            text.x = rItem.x + SPACE*3;
            text.h = rItem.h - SPACE*2;
            text.y = rItem.y + SPACE;

        if (text.h < SPACE - SPACE/3){
            text.h = SPACE - SPACE/3;
        }

        SDL_RenderCopy(pRenderer, items[i], NULL, &text);

        rItem.y += item_height + 20;
    }

    SDL_RenderPresent(pRenderer);
    return;
}

static SDL_Texture* text_at_texture(SDL_Renderer* pRenderer, char* text, TTF_Font* pFont, SDL_Color textColor) {
    SDL_Surface* surface;
    surface = TTF_RenderText_Solid(pFont, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(pRenderer, surface);

    SDL_FreeSurface(surface);

    return texture;
}

static SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, game_env genv, SDL_Color textColor) {
    SDL_Texture** tab = malloc(sizeof(SDL_Texture*) * nbtext);

    for (int i = 0; i < nbtext; i++) {
        SDL_Texture* texture = text_at_texture(pRenderer, text[i], genv->pFont, textColor);
        tab[i] = texture;
    }

    return tab;
}
void SDL_DrawCaseCord(game_env genv, SDL_Renderer* pRenderer, int x, int y, bool extremityUP, bool extremityDOWN) {
    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    // creation of texture (black rectangle in this case)
    SDL_Rect rect;
    rect.h = genv->sprite_size;
    rect.w = genv->sprite_size;
    rect.x = start_x + genv->sprite_size * x;
    if (extremityUP == true){
        rect.y = start_y + genv->sprite_size * y + SPACE;
    }
    else if (extremityDOWN == true) {
        rect.y = start_y + genv->sprite_size * y - SPACE;
    }
    else if ((extremityDOWN == false) && (extremityUP == false)){
        rect.y = start_y + genv->sprite_size * y;
    }

    // draw and fill rectangle
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 128);
    SDL_RenderDrawRect(pRenderer, &rect);
    SDL_RenderFillRect(pRenderer, &rect);
}

static void SDL_Draw_level(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], SDL_Texture* txt[], int nbLevel) {
    int levelIndex = 0;

    for (int x = 0; x < genv->nb_cols; x++) {
        for (int y = 0; y < genv->nb_rows; y++) {
            if (x % 2 == 0 && y % 2 == 0) {
                if (x == 0){
                    SDL_DrawCaseCord(genv, pRenderer, y, x, true, false);
                    draw_number_level(level_tex[levelIndex], pRenderer, genv, y, x, true, false);
                    draw_txt_number_level(txt[levelIndex], pRenderer, genv, y, x, true, false);
                }
                else if (x == genv->nb_rows-1){
                    SDL_DrawCaseCord(genv, pRenderer, y, x, false, true);
                    draw_number_level(level_tex[levelIndex], pRenderer, genv, y, x, false, true);
                    draw_txt_number_level(txt[levelIndex], pRenderer, genv, y, x, false, true);
                }
                else {
                    SDL_DrawCaseCord(genv, pRenderer, y, x, false, false);
                    draw_number_level(level_tex[levelIndex], pRenderer, genv, y, x, false, false);
                    draw_txt_number_level(txt[levelIndex], pRenderer, genv, y, x, false, false);
                }
                levelIndex++;
            }
        }
    }
}

static void render_level_menu(game_env genv, SDL_Renderer* pRenderer, SDL_Texture* level_tex[], SDL_Texture* txt[], int nbLevel) {
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    int ret = SDL_RenderClear(pRenderer);
    if (ret != 0) {
        SDL_printError(true);
    }
    SDL_Draw_level(genv, pRenderer, level_tex, txt, nbLevel);
    SDL_Draw_back(genv, pRenderer);

    SDL_RenderPresent(pRenderer);
}

bool level_process(SDL_Event event, SDL_Window* pWindow, game_env genv, game* g) {
    bool run = true;
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
        genv->state = "exit";
        return false;
    } 
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (genv->mouse_x > 10 && genv->mouse_x < 70 && genv->mouse_y > 10 && genv->mouse_y < 30) {
                genv->state = "main_menu";
                run = false;
            }
            if (genv->case_x % 2 == 0 && genv->case_y % 2 == 0) {
                int levelID = genv->case_x / 2 + (genv->case_y / 2) * 3;

                switch (levelID) {
                    case 0:
                        *g = game_load("levels/level1.txt");
                        genv->state = "game";
                        run =  false;
                        break;
                    case 1:
                        *g = game_load("levels/level2.txt");
                        genv->state = "game";
                        run =  false;
                        break;
                    case 2:
                        *g = game_load("levels/level3.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    case 3:
                        *g = game_load("levels/level4.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    case 4:
                        *g = game_load("levels/level5.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    case 5:
                        *g = game_load("levels/level6.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    case 6:
                        *g = game_load("levels/level7.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    case 7:
                        *g = game_load("levels/level8.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    case 8:
                        *g = game_load("levels/level9.txt");
                        genv->state = "game";
                        run = false;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_ESCAPE){
            *g = NULL;
            genv->state = "main_menu";
            run = false;
        }
    }
    return run;
}

static void clean_texture_tab(SDL_Texture* tab[], int nbItem) {
    for (int i = 0; i < nbItem; i++) {
        SDL_DestroyTexture(tab[i]);
    }
    free(tab);
}

game level_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, game_env genv) {
    bool level_run = true;
    char* levels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char* txt_lvl[] = {"LEVEL 1", "LEVEL 2", "LEVEL 3", "LEVEL 4", "LEVEL 5", "LEVEL 6", "LEVEL 7", "LEVEL 8", "LEVEL 9"};
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
    clean_texture_tab(txt_lvl_tex,9);
    return g;
}

static bool menu_process(SDL_Event event, SDL_Window* pWindow, int nbItem, game* g, SDL_Renderer* pRenderer,
                         game_env genv) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    int w, h;
    SDL_GetWindowSize(pWindow, &w, &h);
    int mx = w/4;
    int my = h/4;
    bool run = true;

    switch (event.type) {
        case SDL_QUIT:
            *g = NULL;
            genv->state = "exit";
            run = false;
            break;
        default:
            break;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == (SDL_BUTTON_LEFT)) {  // NEW PROCESS (PAS UNIVERSEL QUE POUR 3 ITEM)
            int case_x;
            int item_height = (w/(nbItem+6));
            int item_width = (w/2);
            if ((mouse_x > mx) && mouse_x < (item_width + mx)) {
                if (mouse_y > my && mouse_y < (my + item_height)) {
                    case_x = 1;
                }
                if (mouse_y > (my + item_height + SPACE) && mouse_y < (my + (item_height*2) + SPACE)) {
                    case_x = 2;
                }
                if (mouse_y > (my + (item_height*2) + (SPACE*2)) && (mouse_y < (my + (item_height*3) + (SPACE * 2)))) {
                    case_x = 3;
                }
            }
            switch (case_x) {
                case 1:
                    run = false;
                    *g = game_default();
                    genv->state = "game";
                    break;
                case 2:
                    run = false;
                    *g = NULL;
                    genv->state = "level_sel";
                    break;
                case 3:
                    genv->state = "exit";
                    *g = NULL;
                    run = false;
                    break;

                default:
                    break;
            }
        }
    }
    else if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_ESCAPE){
            run = false;
            genv->state = "exit";
            *g = NULL;
        }
    }
    return run;
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