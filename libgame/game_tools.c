#include "game_tools.h"
#include "game.h"
#include "game_ext.h"

#include <stdlib.h>

game game_load(char * filename){
    //Check filename and file existance
    if(filename == NULL){
        fprintf(stderr, "Invalid file name in game_load()\n");
        exit(5);
    }
    FILE* file= fopen(filename, "r");

    if(file == NULL){
        fprintf(stderr, "Unable to open file!\n");
        exit(5);
    }

    //Read size and wrapping information
    uint nb_rows, nb_cols, wrapping;
    int ret;

    ret = fscanf(file, " %d  %d %d", &nb_rows, &nb_cols, &wrapping);

    if(nb_rows > 10 || nb_cols > 10 || ret != 3){
        fprintf(stderr, "Error when loading game!");
        exit(5);
    }

    //read unit find the first \n for the starting grid
    char c = fgetc(file);
    while(c != '\n'){
        c = fgetc(file);
    }
    square* read_data = malloc(sizeof(square)*nb_rows*nb_cols);
    for(uint i = 0; i < nb_rows*nb_cols; i++){
        c = fgetc(file);
        switch (c)
        {
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
            //exit(5);
            break;
        }
    }
    
    //convert int to bool, wrapping
    bool bool_wrapping;
    bool_wrapping = wrapping == 0;

    game rgame = game_new_ext(nb_rows, nb_cols, read_data, bool_wrapping);

    free(read_data);
    fclose(file);

    return rgame;
}