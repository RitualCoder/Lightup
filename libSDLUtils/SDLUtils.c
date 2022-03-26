#include "SDLUtils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libgame/game.h"
#include "../libgame/game_ext.h"
#include "../libgame/game_tools.h"
#include "SDLMenu.h"
#include "env.h"

static double deltaTime(struct timeval start, struct timeval end) {
    return (end.tv_sec - end.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
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

void SDL_Draw_back(game_env genv, SDL_Renderer* pRenderer) {
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* back_x = TTF_RenderText_Blended(genv->pFont, "< BACK", color);
    genv->back = SDL_CreateTextureFromSurface(pRenderer, back_x);
    SDL_FreeSurface(back_x);
    SDL_Rect rect;
    SDL_QueryTexture(genv->back, NULL, NULL, &rect.w, &rect.h);  // TROUVER COEFFICIENT
    rect.h = 20;
    rect.w = 70;
    rect.x = 10;
    rect.y = 10;
    SDL_RenderCopy(pRenderer, genv->back, NULL, &rect);
    SDL_DestroyTexture(genv->back);
}

void SDL_Draw_help(game_env genv, SDL_Renderer* pRenderer, SDL_Window* pWindow) {
    int w,h;
    SDL_GetWindowSize(pWindow, &w, &h);
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* back_x = TTF_RenderText_Blended(genv->pFont, "HELP", color);
    genv->help = SDL_CreateTextureFromSurface(pRenderer, back_x);
    SDL_FreeSurface(back_x);
    SDL_Rect rect;
    SDL_QueryTexture(genv->help, NULL, NULL, &rect.w, &rect.h);  // TROUVER COEFFICIENT
    rect.h = 20;
    rect.w = 60;
    rect.x = w - 70;
    rect.y = 10;
    SDL_RenderCopy(pRenderer, genv->help, NULL, &rect);
    SDL_DestroyTexture(genv->help);
}

void SDL_printError(bool init) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
    if (init) {
        SDL_Quit();
    }
    exit(EXIT_FAILURE);
}

void SDL_initAndSetName(SDL_Renderer** pRenderer, SDL_Window** pWindow) {
    // SDL2 init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_printError(false);
    }
    // window init with name
    *pWindow =
        SDL_CreateWindow(WINDOW_NAME, 100, 100, WINDOW_HEIGHT, WINDOW_LENGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (*pWindow == NULL) {
        SDL_printError(true);
    }
    // Renderer init with gpu and frame limit
    *pRenderer = SDL_CreateRenderer(*pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  //
    if (*pRenderer == NULL) {
        SDL_printError(true);
    }
    SDL_SetWindowMinimumSize(*pWindow, 400, 300);
    // Init Image
    IMG_Init(IMG_INIT_PNG);
}

void loadWallTexture(game_env genv, SDL_Renderer* pRenderer) {
    genv->wall0 = IMG_LoadTexture(pRenderer, "./img/wall0.png");
    genv->wall1 = IMG_LoadTexture(pRenderer, "./img/wall1.png");
    genv->wall2 = IMG_LoadTexture(pRenderer, "./img/wall2.png");
    genv->wall3 = IMG_LoadTexture(pRenderer, "./img/wall3.png");
    genv->wall4 = IMG_LoadTexture(pRenderer, "./img/wall4.png");
    genv->wallu = IMG_LoadTexture(pRenderer, "./img/wallu.png");

    genv->lightbulb = IMG_LoadTexture(pRenderer, "./img/lightbulb.png");
    genv->mark = IMG_LoadTexture(pRenderer, "./img/mark.png");

    if (genv->wall0 == NULL || genv->wall1 == NULL || genv->wall2 == NULL || genv->wall3 == NULL ||
        genv->wall4 == NULL || genv->wallu == NULL || genv->lightbulb == NULL || genv->mark == NULL) {
        SDL_printError(true);
    }
}

void SDL_end(SDL_Renderer* pRenderer, SDL_Window* pWindow) {
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

void SDL_drawGrid(game_env genv, SDL_Renderer* pRenderer) {
    // set color of grid
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

    // calculate start coordinate
    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    int end_x = genv->sprite_size * genv->nb_rows + start_x;
    int end_y = genv->sprite_size * genv->nb_cols + start_y;

    // draw horizontale line
    for (int i = 0; i <= genv->nb_rows; i++) {
        SDL_RenderDrawLine(pRenderer, i * genv->sprite_size + start_x, start_y, i * genv->sprite_size + start_x, end_y);
    }

    // draw vertical line
    for (int i = 0; i <= genv->nb_cols; i++) {
        SDL_RenderDrawLine(pRenderer, start_x, i * genv->sprite_size + start_y, end_x, i * genv->sprite_size + start_y);
    }
}

void SDL_MouseToCase(game_env genv) {
    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;
    int mouse_x_local = genv->mouse_x - start_x;
    int mouse_y_local = genv->mouse_y - start_y;

    if (mouse_y_local < 0 || mouse_x_local < 0 || mouse_x_local >= genv->sprite_size * genv->nb_rows ||
        mouse_y_local >= genv->sprite_size * genv->nb_cols) {
        genv->case_x = -1;  // return -1 if mouse is out of grid
        genv->case_y = -1;
        return;
    } else {
        genv->case_x = mouse_x_local / genv->sprite_size;
        genv->case_y = mouse_y_local / genv->sprite_size;
        return;
    }
}

void SDL_DrawCase(game_env genv, SDL_Renderer* pRenderer) {
    if (genv->case_x < 0 || genv->case_y < 0) {  // not draw if mouse is out of grid
        return;
    }

    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    // creation of texture (black rectangle in this case)
    SDL_Rect rect;
    rect.h = genv->sprite_size;
    rect.w = genv->sprite_size;
    rect.x = start_x + genv->sprite_size * genv->case_x;
    rect.y = start_y + genv->sprite_size * genv->case_y;

    // draw and fill rectangle
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 128);
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);  // activate blend mode
    SDL_RenderFillRect(pRenderer, &rect);
    SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_NONE);  // deactivate blend mode
}

void draw_texture_at_pos(SDL_Texture* tex, SDL_Renderer* pRenderer, game_env genv, int y, int x) {
    int start_y = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_x = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    SDL_Rect pos;
    pos.x = start_y + y * genv->sprite_size;
    pos.y = start_x + x * genv->sprite_size;
    pos.w = genv->sprite_size;
    pos.h = genv->sprite_size;

    SDL_RenderCopy(pRenderer, tex, NULL, &pos);
}

void draw_number_level(SDL_Texture* tex, SDL_Renderer* pRenderer, game_env genv, int y, int x, bool extremityUP, bool extremityDOWN) {
    int space = genv->sprite_size/4;
    int start_y = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_x = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    SDL_Rect pos;
    pos.x = start_y + y * genv->sprite_size + space;
    if (extremityUP){
        pos.y = start_x + x * genv->sprite_size + space + SPACE;
    }
    else if (extremityDOWN){
        pos.y = start_x + x * genv->sprite_size + space - SPACE;
    }
    else if (extremityDOWN == false && extremityUP == false){
        pos.y = start_x + x * genv->sprite_size + space;
    }
    pos.w = genv->sprite_size - (space*2);
    pos.h = pos.w;
    if (pos.w < (space/2)){
        pos.w = (space/2);
        pos.h = (space/2);
    }

    SDL_RenderCopy(pRenderer, tex, NULL, &pos);
}

void draw_txt_number_level(SDL_Texture* tex, SDL_Renderer* pRenderer, game_env genv, int y, int x, bool extremityUP, bool extremityDOWN) {
    int space = genv->sprite_size/6;
    int start_y = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_x = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    SDL_Rect pos;
    pos.x = start_y + y * genv->sprite_size + space;
    if (extremityUP){
        pos.y = start_x + x * genv->sprite_size + genv->sprite_size + SPACE + (SPACE/2);
    }
    else if (extremityDOWN){
        pos.y = start_x + x * genv->sprite_size + genv->sprite_size - SPACE + (SPACE/2);
    }
    else if (extremityDOWN == false && extremityUP == false){
        pos.y = start_x + x * genv->sprite_size + genv->sprite_size + (SPACE/2);
    }
    pos.w = genv->sprite_size - (space*2);
    pos.h = pos.w/4;
    if (pos.w < (space/2)){
        pos.w = (space/2);
        pos.h = (space/2);
    }

    SDL_RenderCopy(pRenderer, tex, NULL, &pos);
}

void drawLighted(game g, game_env genv, SDL_Renderer* pRenderer) {
    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    for (int x = 0; x < game_nb_cols(g); x++) {
        for (int y = 0; y < game_nb_rows(g); y++) {
            if (game_is_lighted(g, y, x) && !game_has_error(g, y, x)) {
                SDL_Rect rect;
                rect.h = genv->sprite_size;
                rect.w = genv->sprite_size;
                rect.x = start_x + genv->sprite_size * x;
                rect.y = start_y + genv->sprite_size * y;

                SDL_SetRenderDrawColor(pRenderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(pRenderer, &rect);
                SDL_RenderFillRect(pRenderer, &rect);
            }
        }
    }
}

void drawError(game g, game_env genv, SDL_Renderer* pRenderer) {
    int start_x = genv->windows_width / 2 - (genv->sprite_size * genv->nb_rows) / 2;
    int start_y = genv->window_height / 2 - (genv->sprite_size * genv->nb_cols) / 2;

    for (int x = 0; x < game_nb_cols(g); x++) {
        for (int y = 0; y < game_nb_rows(g); y++) {
            if (game_has_error(g, y, x)) {
                SDL_Rect rect;
                rect.h = genv->sprite_size;
                rect.w = genv->sprite_size;
                rect.x = start_x + genv->sprite_size * x;
                rect.y = start_y + genv->sprite_size * y;

                SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 128);
                SDL_RenderDrawRect(pRenderer, &rect);
                SDL_RenderFillRect(pRenderer, &rect);
            }
        }
    }
}

void SDL_DrawGame(game g, game_env genv, SDL_Renderer* pRenderer) {
    drawLighted(g, genv, pRenderer);
    drawError(g, genv, pRenderer);

    for (int x = 0; x < game_nb_cols(g); x++) {
        for (int y = 0; y < game_nb_rows(g); y++) {
            if (game_is_black(g, x, y)) {
                SDL_Texture* draw = NULL;
                int val = game_get_black_number(g, x, y);
                switch (val) {
                    case (-1):
                        draw = genv->wallu;
                        break;
                    case (0):
                        draw = genv->wall0;
                        break;
                    case (1):
                        draw = genv->wall1;
                        break;
                    case (2):
                        draw = genv->wall2;
                        break;
                    case (3):
                        draw = genv->wall3;
                        break;
                    case (4):
                        draw = genv->wall4;
                        break;
                    default:
                        break;
                }
                draw_texture_at_pos(draw, pRenderer, genv, y, x);
            }
            if (game_is_lightbulb(g, x, y)) {
                draw_texture_at_pos(genv->lightbulb, pRenderer, genv, y, x);
            }
            if (game_is_marked(g, x, y)) {
                draw_texture_at_pos(genv->mark, pRenderer, genv, y, x);
            }
        }
    }
}

void printDebug(SDL_Renderer* pRenderer, SDL_Window* pWindow) {
    int videoDriverIndex = SDL_GetNumVideoDrivers();
    SDL_Log("Video driver: %d -> %s\n", videoDriverIndex, SDL_GetVideoDriver(videoDriverIndex));
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(pRenderer, &info) >= 0) {
        SDL_Log("Renderer name: %s\n", info.name);
        int width, height;
        SDL_GetWindowSize(pWindow, &width, &height);
        SDL_Log("Window size %dx%d\n", width, height);
    }
}

void render(game_env genv, SDL_Renderer* pRenderer, SDL_Window* pWindow, double fps, game g) {
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    int ret = SDL_RenderClear(pRenderer);
    if (ret != 0) {
        SDL_printError(true);
    }

    SDL_DrawGame(g, genv, pRenderer);

    SDL_drawGrid(genv, pRenderer);
    SDL_DrawCase(genv, pRenderer);
    SDL_Draw_back(genv, pRenderer);
    SDL_Draw_help(genv, pRenderer, pWindow);

    SDL_RenderPresent(pRenderer);

// compute fps TODO: utiliser une moyenne plutot que un calcule à chaque frame
#ifdef DEBUG
    gettimeofday(&genv->endTime, NULL);
    double delta = deltaTime(genv->startTime, genv->endTime);
    fps = 1000 / delta;
    SDL_Log("fps: %.2lf", fps);
#endif
}

bool process(SDL_Event event, SDL_Window* pWindow, game_env genv, game g) {
    bool ret = true;
    SDL_GetWindowSize(pWindow, &genv->windows_width, &genv->window_height);
    int w = genv->windows_width;
    if (genv->window_height < genv->windows_width) {
        genv->sprite_size = (genv->window_height - SPACE*3) / genv->nb_cols;
    } else {
        genv->sprite_size = (genv->windows_width - SPACE*3) / genv->nb_rows;
    }
    genv->button = SDL_GetMouseState(&genv->mouse_x, &genv->mouse_y);
    SDL_MouseToCase(genv);

    switch (event.type) {
        case SDL_QUIT:
            genv->state = "exit";
            ret = false;
            break;

        default:
            break;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (event.button.button == (SDL_BUTTON_LEFT)) {
            if (genv->mouse_x > SPACE/2 && genv->mouse_x < SPACE*4 && genv->mouse_y > SPACE/2 && genv->mouse_y < SPACE*2) {
                genv->state = "main_menu";
                ret = false;
            }
            if (genv->mouse_x > (w - SPACE*4) && genv->mouse_x < w && genv->mouse_y > SPACE/2 && genv->mouse_y < SPACE*2) {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                         "HELP",
                         "Left click to put a lightbulb\n"
                        "Right click to put a mark\n"
                        "Tap 's' to solve the grid\n\n"
                        "You must illuminate the entire grid !\n"
                        "Good Luck !",
                         pWindow);
            }
            if (game_check_move(g, genv->case_y, genv->case_x,
                                S_LIGHTBULB)) {  // Check if the move on the grid is legit for a lightbulb

                if (game_is_lightbulb(g, genv->case_y, genv->case_x)) {
                    game_play_move(g, genv->case_y, genv->case_x, S_BLANK);
                } else {
                    game_play_move(g, genv->case_y, genv->case_x, S_LIGHTBULB);
                }
            }
        }
        if (game_check_move(g, genv->case_y, genv->case_x,
                            S_MARK)) {                        // Check if the move on the grid is legit for a mark
            if (event.button.button == (SDL_BUTTON_RIGHT)) {  // if right click on the mouse put a mark
                if (game_is_marked(g, genv->case_y, genv->case_x)) {
                    game_play_move(g, genv->case_y, genv->case_x, S_BLANK);
                } else {
                    game_play_move(g, genv->case_y, genv->case_x, S_MARK);
                }
            }
        }
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_z) {
            game_undo(g);
        }
        if (event.key.keysym.sym == SDLK_y) {
            game_redo(g);
        }
        if (event.key.keysym.sym == SDLK_s) {
            game_solve(g);
        }
        if (event.key.keysym.sym == SDLK_w) {
            game_save(g, "sdl_game_save.txt");
        }
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            genv->state = "main_menu";
            ret = false;
        }
    }
#ifdef DEBUG
    SDL_Log("Mouse coordinate x: %d, y: %d\n", genv->mouse_x, genv->mouse_y);
    SDL_Log("case coordinate x: %d, y: %d\n", genv->case_x, genv->case_y);
#endif
    return ret;
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
    genv->pFont = TTF_OpenFont("./font/04B_20__.ttf", 1000);

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

void update_genv(game_env genv, game g) {
    genv->nb_cols = game_nb_cols(g);
    genv->nb_rows = game_nb_rows(g);
}