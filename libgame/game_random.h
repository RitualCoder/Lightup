/**
 * @file game_random.h
 * @brief Game Random.
 * @details See @ref index for further details.
 *
 **/

#ifndef __GAME_RANDOM_H__
#define __GAME_RANDOM_H__
#include <stdbool.h>
#include <stdio.h>

#define MIN 2
#define MAX 7

#include "game.h"

/**
 * @name Game Random
 * @{
 */

static uint nb_neigh_lightbulbs(cgame g, uint i, uint j);

static uint nb_unlit_squares(cgame g);

/**
 * Create a random game with a given size and number of walls
 *
 * @param nb_rows the number of rows of the game
 * @param nb_cols the number of columns of the game
 * @param wrapping wrapping option
 * @param nb_walls the number of walls to add
 * @param with_solution if true, the game contains the solution, otherwise only walls
 *
 * @return the generated random game
 */

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_walls, bool with_solution);

#endif  // __GAME_RANDOM_H__