#include "game_tools.h"

#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"

game game_load(char* filename) {
    // Check filename and file existance
    if (filename == NULL) {
        fprintf(stderr, "Invalid file name in game_load()\n");
        exit(5);
    }
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Unable to open file!\n");
        exit(5);
    }

    // Read size and wrapping information
    uint nb_rows, nb_cols, wrapping;
    int ret;

    ret = fscanf(file, " %d  %d %d", &nb_rows, &nb_cols, &wrapping);

    if (nb_rows > 10 || nb_cols > 10 || ret != 3) {
        fprintf(stderr, "Error when loading game!");
        exit(5);
    }

    // read unit find the first \n for the starting grid
    char c = fgetc(file);
    while (c != '\n') {
        c = fgetc(file);
    }
    square* read_data = malloc(sizeof(square) * nb_rows * nb_cols);
    for (uint i = 0; i < nb_rows * nb_cols; i++) {
        c = fgetc(file);
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
                fprintf(stderr, "unable to read game");
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
    game_update_flags(rgame);

    return rgame;
}

void game_save_int(cgame g, char* filename, bool appendMode) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "inexistant file\n");
        exit(5);
    }
    if(appendMode){
        int ch = getc(f);
        while (ch != EOF)
        {
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

void game_save(cgame g, char* filename){
    game_save_int(g, filename, false);
}

bool genGame(int pos, int size, game g, bool stopAtFirstSolution, bool saveInFile, char* filename, int nbLightbulb,
             int posedLight) {
    if (pos == size) {
        if (posedLight == nbLightbulb) {
            game_update_flags(g);
            if (game_is_over(g)) {
                if (saveInFile) {
                    game_save(g, filename);  // TODO: faire une fonction auxiliaire qui gère plusieurs écritures dans un
                                             // même fichier
                } else {
                    game_print(g);
                }
                return true;
            }
            return false;
        }
        return false;
    }

    bool ret;  // TODO: creer une fonction auxiliaire pour supprimer le code copier-coller
    if ((g->squares[pos] & S_MASK) & S_BLACK) { // Si c'est un mur alors on saute la case
        ret = genGame(pos + 1, size, g, stopAtFirstSolution, saveInFile, filename, nbLightbulb, posedLight);
        if (stopAtFirstSolution && ret) {
            return ret;
        }
    } else {
        g->squares[pos] = S_BLANK; //cas ou on pose un blanc

        ret = genGame(pos + 1, size, g, stopAtFirstSolution, saveInFile, filename, nbLightbulb, posedLight);
        if (stopAtFirstSolution && ret) {
            return ret;
        }

        if (posedLight + 1 <= nbLightbulb) { // cas d'un lightbulb
            g->squares[pos] = S_LIGHTBULB;
            ret = genGame(pos + 1, size, g, stopAtFirstSolution, saveInFile, filename, nbLightbulb, posedLight + 1);
            if (stopAtFirstSolution && ret) {
                return ret;
            }
        }
    }
    return false;
}

bool game_solve(game g) {
    int len_g = g->nb_cols * g->nb_rows;
    for (int i = 1; i < len_g; i++) {
        printf("Trying with %d lightBulb\n", i);
        if (genGame(0, (g->nb_cols * g->nb_rows), g, true, false, "0", i, 0)) {
            return true;
        }
    }
    return false;
}

uint game_nb_solutions(cgame g) { return 0; }