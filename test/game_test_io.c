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
/*                               IO TESTS (V2)                                */
/* ************************************************************************** */

int test_load(void) {
    /*Test on loading default game*/
    bool ok = true;
    game g_load = game_load("default.txt");
    game g_private = game_default();
    game_update_flags(g_load);
    game_update_flags(g_private);

    ok = game_equal(g_load, g_private);

    /*Test with default solution*/

    g_load = game_load("default_solution.txt");
    g_private = game_default_solution();
    game_update_flags(g_load);
    game_update_flags(g_private);

    ok = ok && game_is_over(g_load);
    ok = ok && game_equal(g_load, g_private);

    if (ok) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

/* ************************************************************************** */

int test_save() { return EXIT_SUCCESS; }