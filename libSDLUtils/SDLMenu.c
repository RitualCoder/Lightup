#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../libgame/game.h"
#include "../libgame/game_aux.h"
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
        SDL_RenderCopy(pRenderer, items[i], NULL, &rItem);

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

static SDL_Texture** make_all_text_texture(SDL_Renderer* pRenderer, char* text[], int nbtext, TTF_Font* pFont) {
    SDL_Texture** tab = malloc(sizeof(SDL_Texture*) * nbtext);

    for (int i = 0; i < nbtext; i++) {
        SDL_Texture* texture = text_at_texture(pRenderer, text[i], pFont);
        tab[i] = texture;
    }

    return tab;
}

static bool menu_process(SDL_Event event, SDL_Window* pWindow) {
    switch (event.type) {
        case SDL_QUIT:
            return false;
            break;

        default:
            break;
    }

    return true;
}

game main_menu(SDL_Renderer* pRenderer, SDL_Window* pWindow, TTF_Font* pFont) {
    bool run = true;
    char* items[] = {"New game", "Settings", "Quit!"};
    SDL_Texture** items_texture = make_all_text_texture(pRenderer, items, 3, pFont);
    SDL_Event event;

    while (run) {
        draw_menu(pRenderer, pWindow, items_texture, 3);
        while (SDL_PollEvent(&event)) {  // process input
            run = menu_process(event, pWindow);
        }
    }

    return game_default();
}