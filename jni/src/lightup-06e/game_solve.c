#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgame/game.h"
#include "libgame/game_aux.h"
#include "libgame/game_tools.h"

void usage(char* cmd) {
    fprintf(stderr, "Usage : %s <option> <input> <output>\n", cmd);
    fprintf(stderr, "Options are only '-c' or '-s'\n");
    fprintf(stderr, "Input is is the name of the input file containing the problem instance to solve\n");
    fprintf(stderr, "Output is the name of the output file containing the result of the program.\n");
}

int main(int argc, char* argv[]) {
    bool saveInFile = false;
    if (argc < 3) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 4) {
        saveInFile = true;
    }

    game g = game_load(argv[2]);
    char* option = argv[1];

    if (strcmp(option, "-s") == 0) {
        if (game_solve(g)) {
            if (saveInFile) {
                game_save(g, argv[3]);
            } else {
                game_print(g);
            }
        } else {
            return EXIT_FAILURE;
        }
    } else if (strcmp(option, "-c") == 0) {
        uint nbSolution = game_nb_solutions(g);
        if (saveInFile) {
            FILE* file = fopen(argv[3], "w");
            if (file == NULL) {
                fprintf(stderr, "Error on opening file!\n");
                return EXIT_FAILURE;
            }
            fprintf(file, "%u\n", nbSolution);
            fclose(file);
        } else {
            printf("Find %u solution(s)\n", nbSolution);
        }
    } else {
        usage(argv[0]);
    }
    return EXIT_SUCCESS;
}