# README

<a href="https://gitlab.emi.u-bordeaux.fr/pt2/lightup/-/commits/main"><img alt="pipeline status" src="https://gitlab.emi.u-bordeaux.fr/pt2/lightup/badges/main/pipeline.svg" /></a>
<a href="https://gitlab.emi.u-bordeaux.fr/pt2/lightup/-/commits/main"><img alt="coverage report" src="https://gitlab.emi.u-bordeaux.fr/pt2/lightup/badges/main/coverage.svg" /></a>

## Rules

This game is a simple one-player puzzle, called *Light Up* in the
[Simon Tatham's Puzzle Collection](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html).

According to *Simon Tatham*:

> You have a grid of squares. Some are filled in black; some of the black
> squares are numbered, the rest of them are blank. Your aim is to *light up*
> all the blank squares by placing light bulbs in some of them.
>
> Each light bulb illuminates the square it is on, plus all squares in line with
> it horizontally or vertically unless a black square is blocking the way.
>
> To win the game, you must satisfy the following conditions:
>
> * All non-black squares are lit.
> * No light is lit by another light.
> * Each numbered black square must be orthogonally adjacent to exactly the
>   given number of lights.
> * Non-numbered black squares may have any number of lights adjacent to them.

Besides, it is also possible to place a *mark* in a non-black square, indicating
that you are sure there is no light bulb on this square. The *mark* square just
aids players to solve the game.

In summary, the game uses 4 different main square states:

* blank (' '),
* light bulb ('*'),
* black walls, that can be either unumbered ('w') or numbered ('0', '1', '2',
  '3', or '4'),
* mark ('-').

The state of each square can also be completed by two additionnal flags:

* A *lighted* flag, that indicates a square is lit ('.'); this can be added to
  any non-black square.
* An *error* flag, that indicates that an error occurs explicitly on this
  square, if a light is lit by another, or if there are too many lights or marks
  around a numbered black wall.

By convention, the square in the *i*-th row and *j*-th column of the grid is
referred to as the coordinate (i,j), and the coordinate (0,0) corresponds to the
top left corner (like in
[matrices](https://en.wikipedia.org/wiki/Matrix_(mathematics))). The dimension
of the default game is 7x7.

You can try the original game
[here](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html).

## Example: Default Game

As an example, let's see the instance of the game, considered thereafter as the
*default* game:

```text
   0123456
   -------
0 |  1    |
1 |  2    |
2 |     w2|
3 |       |
4 |1w     |
5 |    2  |
6 |    w  |
   -------
```

Hereafter, its solution:

```text
   0123456
   -------
0 |*.1*...|
1 |.*2...*|
2 |..*..w2|
3 |......*|
4 |1w..*..|
5 |*...2*.|
6 |.*..w..|
   -------
```

Try it: <https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html#7x7:b1f2iB2g1Bi2fBb>

## Programming Interface

The game interface is divided into 2 files:

* Basic Game Functions: @ref game.h
* Auxiliarry Game Functions: @ref game_aux.h
* Extended Game Functions: @ref game_ext.h

It manipulates the *opaque* @ref game structure by means of pointers or const pointers (@ref cgame).

* <https://en.wikipedia.org/wiki/Opaque_data_type>

About Const Pointer:

* See also: <http://www.geeksforgeeks.org/const-qualifier-in-c/>
* See also this more technical discussion: <http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness>

## Error Policy

There are many different ways to deal with program errors... Concerning the
"game" library, we suppose that function calls to these interfaces (*game.h*,
...) must systematically be made with valid arguments (see preconditions). Also,
the behavior of the library is not defined if a third-party program passes bad
arguments to it. In practice, this means that the responsibility of checking
that the arguments are valid is relieved to the high-level program (*game_text*,
...). In short, [garbage in, garbage out](https://en.wikipedia.org/wiki/Garbage_in,_garbage_out).

In order to debug your program easily, we recommand to use the `assert()` macro,
as it allows you to activate checks only in DEBUG mode and not in RELEASE mode.
More precisely, if you pass the `-DNDEBUG` option at compilation time, the macro
`assert()` generates no code, and hence does nothing at all.

For example:

```C
#include <assert.h>
#include <stdlib.h>

/** duplicate an array of integer and return it */
int* copyArray(int size, int * array)
{
  assert(size > 0);                           // check size parameter
  assert(array != NULL);                      // check array parameter
  int * newarray = malloc(size*sizeof(int));  // allocate memory for a new array
  assert(newarray != NULL);                   // check memory allocation
  for(int i = 0 ; i < size ; i++)             // copy array values
    newarray[i] = array[i];
  return newarray;
}
```

## Game Extension

Here, we will consider a generalisation of the original game (or *version 1*).
This new version (called *version 2*) is characterized by the following changes:

* The grid can have an arbitrary number of rows and columns, as for instance 4x4, 5x7, 2x2, 6x1. The default game
  dimensions were a square grid of size 7x7.
* The game grid has the possibility to have a "toric" **topology** (`wrapping`
  option): the rightmost column is adjacent to the leftmost column and the
  highest row is adjacent to the lowest row.
* We introduce in this version an history management of game moves
  (`game_play_move()`), in order to be able to *undo* the last moves played
  (`game_undo()`), or to replay the last undone moves if necessary
  (`game_redo()`). After playing a new move with `game_play_move()`, it is no
  longer possible to redo an old cancelled move. The function `game_restart()`
  clears the history. The other functions are not involved in the history
  management.

The file @ref game_ext.h contains the description of these new features. Note
that version 2 of the game keeps the compatibility with version 1.

## References

* <https://www.nikoli.co.jp/en/puzzles/akari.html>
* <https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/lightup.html>
* <https://en.wikipedia.org/wiki/Light_Up_(puzzle)>

---
