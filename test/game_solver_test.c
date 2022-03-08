#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libgame/game.h"
#include "../libgame/game_aux.h"
#include "../libgame/game_ext.h"
#include "../libgame/game_tools.h"
#include "game_examples.h"
#include "game_test.h"

/* ************************************************************************** */
/*                               SOLVER TEST                                  */
/* ************************************************************************** */

int test_game_solve() {
    int retVal = EXIT_SUCCESS;
    game g = game_load("3x3.txt");
    if (!game_solve(g)) {
        retVal = EXIT_FAILURE;
    }
    game_delete(g);
    g = game_load("3x3.txt");
    if(game_nb_solutions(g)!= 4){
        retVal = EXIT_FAILURE;
    }
    game_delete(g);
    return retVal;
}