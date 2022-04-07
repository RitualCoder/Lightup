#include "game_tools.h"

#include <SDL.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

static char* getFilePathAndroid(char* file) {
    struct stat st = {0};
    const char* androidPath = SDL_AndroidGetInternalStoragePath();
    // const char* androidPath = "/storage/emulated/0/LightUp";
    SDL_Log("Android path: %s", androidPath);
    size_t size = strlen(androidPath) + strlen(file) + 2;  // 2: le / et le \0
    char* fullPath = malloc(sizeof(char) * size);
    sprintf(fullPath, "%s/%s", androidPath, file);
    return fullPath;
}

static void copy_asset(char* src, char* dst) {
    SDL_RWops* file = SDL_RWFromFile(src, "r");
    if (!file) SDL_Log("[ERROR] SDL_RWFromFile: %s\n", src);
    int size = SDL_RWsize(file);
    SDL_Log("copy file %s (%d bytes) into %s\n", src, size, dst);
    char* buf = (char*)malloc(size + 1);
    if (!buf) SDL_Log("[ERROR] malloc\n");
    int r = SDL_RWread(file, buf, 1, size);
    SDL_Log("read %d\n", r);
    if (r != size) SDL_Log("[ERROR] fail to read all file (%d bytes)\n", r);
    FILE* out = fopen(dst, "w+");
    if (!out) SDL_Log("[ERROR] fail to create file %s\n", dst);
    int w = fwrite(buf, 1, r, out);
    if (r != w) SDL_Log("[ERROR] fail to write all file (%d bytes)\n", w);
    fclose(out);
    SDL_RWclose(file);
    free(buf);
}

game game_load(char* filename) {
    // Check filename and file existance
    if (filename == NULL) {
        SDL_Log("Invalid file name in game_load()\n");
        exit(5);
    }

#ifdef __ANDROID__

    char* android_path = getFilePathAndroid(filename);
    copy_asset(filename, android_path);
    filename = android_path;

#endif

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        SDL_Log("Unable to open file!\n");
        SDL_Log("errno = %d\n", errno);
        SDL_Log("errnostr: %s\n", strerror(errno));
        exit(5);
    }

    // Read size and wrapping information
    uint nb_rows, nb_cols, wrapping;
    int ret;
    SDL_Log("Start loading file");
    ret = fscanf(file, " %d  %d %d", &nb_rows, &nb_cols, &wrapping);

    if (nb_rows > 10 || nb_cols > 10 || ret != 3) {
        SDL_Log("Error when loading game!");
        exit(5);
    }

    SDL_Log("rows: %d, cols: %d, wrapping:%d", nb_rows, nb_cols, wrapping);

    // read unit find the first \n for the starting grid
    char c = getc(file);
    SDL_Log("%c", c);
    while (c != '\n') {
        c = getc(file);
    }
    SDL_Log("Start Loading game tab");
    square* read_data = malloc(sizeof(square) * nb_rows * nb_cols);
    for (uint i = 0; i < nb_rows * nb_cols; i++) {
        c = getc(file);
        switch (c) {
            case 'b':
                read_data[i] = S_BLANK;
                break;
            case '*':
                read_data[i] = S_LIGHTBULB;
                break;
            case '0':
                read_data[i] = S_BLACK0;
                break;
            case '1':
                read_data[i] = S_BLACK1;
                break;
            case '2':
                read_data[i] = S_BLACK2;
                break;
            case '3':
                read_data[i] = S_BLACK3;
                break;
            case '4':
                read_data[i] = S_BLACK4;
                break;
            case 'w':
                read_data[i] = S_BLACKU;
                break;
            case '-':
                read_data[i] = S_MARK;
                break;
            case '\n':
                i--;
                break;
            default:
                SDL_Log("unable to read game");
                exit(5);
                break;
        }
    }

    // convert int to bool, wrapping
    bool bool_wrapping;
    bool_wrapping = wrapping != 0;

    game rgame = game_new_ext(nb_rows, nb_cols, read_data, bool_wrapping);

    free(read_data);
    fclose(file);

#ifdef __ANDROID__
    free(android_path);
#endif
    game_update_flags(rgame);

    return rgame;
}

void game_save_int(cgame g, char* filename, bool appendMode) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        SDL_Log("inexistant file\n");
        exit(5);
    }
    if (appendMode) {
        int ch = getc(f);
        while (ch != EOF) {
            ch = getc(f);
        }
    }
    uint wrap;
    if (g->wrapping) {
        wrap = 1;
    } else {
        wrap = 0;
    }
    fprintf(f, "%d ", g->nb_rows);
    fprintf(f, "%d ", g->nb_cols);
    fprintf(f, "%d\n", wrap);
    for (uint i = 0; i < game_nb_rows(g); i++) {
        for (uint j = 0; j < game_nb_cols(g); j++) {
            if (game_get_state(g, i, j) == S_LIGHTBULB) {
                fprintf(f, "*");
            }
            if (game_get_state(g, i, j) == S_BLACKU) {
                fprintf(f, "w");
            }
            if (game_is_black(g, i, j) && game_get_state(g, i, j) != S_BLACKU) {
                fprintf(f, "%d", game_get_black_number(g, i, j));
            }
            if (game_is_blank(g, i, j)) {
                fprintf(f, "b");
            }
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void game_save(cgame g, char* filename) { game_save_int(g, filename, false); }

static bool _checkErr(game g) {
    for (int x = g->nb_cols - 1; x >= 0; x--) {
        for (int y = g->nb_rows - 1; y >= 0; y--) {
            if (game_has_error(g, x, y)) {
                return true;
            }
        }
    }
    return false;
}

static bool genGame(int pos, int size, game g, bool stopAtFirstSolution, int* generatedGame, int* solutionFind) {
    game_update_flags(g);

    if (pos == size) {
        *generatedGame = *generatedGame + 1;
        if (game_is_over(g)) {
            *solutionFind = *solutionFind + 1;
            return true;
        }

        return false;
    }

    bool ret;  // TODO: creer un fonction auxilière pour suprimer le code copier-coller
    if (_checkErr(g)) {
        return false;
    }
    if ((g->squares[pos] & S_MASK) & S_BLACK) {  // Si c'est un mur alors on saute la case
        ret = genGame(pos + 1, size, g, stopAtFirstSolution, generatedGame, solutionFind);
        if (stopAtFirstSolution && ret) {
            return ret;
        }
    } else {
        if ((g->squares[pos] & F_MASK) != F_LIGHTED) {  // cas d'un lightbulb
            g->squares[pos] = S_LIGHTBULB;
            ret = genGame(pos + 1, size, g, stopAtFirstSolution, generatedGame, solutionFind);
            if (stopAtFirstSolution && ret) {
                return ret;
            }
        }

        g->squares[pos] = S_BLANK;  // cas ou on pose un blanc
        ret = genGame(pos + 1, size, g, stopAtFirstSolution, generatedGame, solutionFind);
        if (stopAtFirstSolution && ret) {
            return ret;
        }
    }
    return false;
}

int min(int a, int b) {
    if (b < a) {
        return b;
    }
    return a;
}

bool game_solve(game g) {
    int generatedGame = 0;
    int solutionFound = 0;

    if (genGame(0, (g->nb_cols * g->nb_rows), g, true, &generatedGame, &solutionFound)) {
        return true;
    }

    return false;
}

uint game_nb_solutions(cgame g) {
    int generatedGame = 0;
    int solutionFound = 0;
    game workingGame = game_copy(g);
    genGame(0, (g->nb_cols * g->nb_rows), workingGame, false, &generatedGame, &solutionFound);
    game_delete(workingGame);

    return solutionFound;
}