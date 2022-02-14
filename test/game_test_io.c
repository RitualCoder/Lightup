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

    game_delete(g_load);
    game_delete(g_private);

    if (ok) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

/* ************************************************************************** */

int test_save() {
    bool ok = true;

    // Test with a saved grid default and an unsaved one
    game default1 = game_default();
    game_save(default1, "save.txt");
    game loaded = game_load("save.txt");
    if (!game_equal(default1, loaded)){
        ok = false;
    }
    game_delete(default1);
    remove("save.txt");

    // Test with two default solution & if the grid sol is won
    game default_s = game_default_solution();
    game_save(default_s, "save2.txt");
    loaded = game_load("save2.txt");
    game_update_flags(loaded);
    if (!game_equal(loaded, default_s) || !game_is_over(loaded)){
        ok = false;
    }
    game_delete(default_s);
    remove("save2.txt");

    // Test with two different grid
    game default3 = game_default();
    game default4 = game_default();
    game_play_move(default3, 0, 0, S_LIGHTBULB);
    game_save(default3, "save3.txt");
    loaded = game_load("save3.txt");
    game_update_flags(loaded);
    if (game_equal(loaded, default4)){
        ok = false;
    }
    game_delete(loaded);
    game_delete(default3);
    remove("save3.txt");

    if (ok) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}