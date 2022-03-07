#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libgame/game.h"
#include "libgame/game_aux.h"
#include "libgame/game_tools.h"

void usage() {
    fprintf(stderr, "Usage : game_solve <option> <input> <output>\n");
    fprintf(stderr, "Options are only '-c' or '-s'\n");
    fprintf(stderr, "Input is is the name of the input file containing the problem instance to solve\n");
    fprintf(stderr, "Output is the name of the output file containing the result of the program.\n");
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return EXIT_FAILURE;
    }
    game g = game_load(argv[2]);
    char* option;
    option = argv[1];
    if (strcmp(option, "-s") == 0) {
        if (game_solve(g)) {
            game_save(g, argv[3]);
        } else {
            EXIT_FAILURE;
        }
    } else if (strcmp(option, "-c") == 0) {
        FILE* file = fopen(argv[3], "w");
        if (file == NULL) {
            fprintf(stderr, "Inexistant file!\n");
            exit(5);
        }
        uint count = game_nb_solutions(g);
        fprintf(file, "%u\n", count);
        fclose(file);
    } else {
        usage();
    }

    /* game g = game_load(argv[1]);
    game g2 = game_copy(g);
    game_solve(g);
    game_nb_solutions(g2); */
    return EXIT_SUCCESS;
}