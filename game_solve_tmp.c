#include <stdio.h>
#include <stdlib.h>

#include "libgame/game.h"
#include "libgame/game_aux.h"
#include "libgame/game_tools.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    game g  = game_load(argv[1]);
    game_solve(g);
    return EXIT_SUCCESS;
}