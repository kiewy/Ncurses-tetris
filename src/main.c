#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "blocks.h"

#define FIELD_WIDTH 10 
#define FIELD_HEIGHT 20

void drawField(WINDOW *w_field, const int iarr_field[FIELD_WIDTH*FIELD_HEIGHT], char cFill, int clearZeroes);
void copyBlock(int block_in[4][4], const int block_original[4][4]);
int clearFullLines(int field[FIELD_WIDTH*FIELD_HEIGHT], const int i_pointsPerLine);

int moveBlock(int iarr_field[FIELD_WIDTH*FIELD_HEIGHT], const int block[4][4], int y, int x);
int colCheck(int iarr_field[FIELD_WIDTH*FIELD_HEIGHT], const int block[4][4], int blockY, int blockX);

void rotateBlock(int block[4][4], int i_rows);

int copyRandomBlock(int block_in[4][4]);

int main(int argc, char *argv[])
{

    //---------------------------------------------------
    //     DECLARING DATA 
    //---------------------------------------------------
    //Time between automatic downwards movements of blocc 
    double d_dropTime = 0.5;
    int    i_linePoints  = 500;
    int    i_blockPoints = 20;

    WINDOW *w_blocks; //This will be the main playing field.
    WINDOW *w_instrucs;
    WINDOW *w_score; 

    //Store the active/moving block here:
    int block_active[4][4] = {0};
    int i_blockType = 1;
    /*Block-types
     * 1: L
     * 2: Square
     * */
    //Coords of left top corner of active block:
    int i_activeXpos = 5,
        i_activeYpos = 5;
    //Use moveBlock the active block, and drawField this field apart from the main
    //field. This way, there's an easy way to clear coords previously occupied by the 
    //active block, and no data that should get modified by the active block gets modified by 
    //the active block.
    int iarr_tempField[FIELD_WIDTH*FIELD_HEIGHT]; //SET THESE TO A CALCULATED VALUE TO CENTER SHIT NICELY
    int i_startx = 5,
        i_starty = 6;

    int score = 0; //duh

    int iarr_field[FIELD_HEIGHT*FIELD_WIDTH];

    //Use with time.h
    //Reminder: use 
    //             clock() to get clockticks since beginning of program
    //             CLOCKS_PER_SEC to get clocks per sec
    //so divide clock() by clocks per sec to get current time.
    double d_currentTime = 0; 
    double d_prevTime = 0;
    double d_timeAccum = 0;

    //------------------------------------------
    //       Setting up NCURSES HERE
    //-----------------------------------------
    initscr(); //START CURSES MODE
    noecho(); //Don't echo input to terminal
    keypad(stdscr,TRUE); //Allow keypad
    timeout(10); //Make button input non-blocking

    if(has_colors() == FALSE){
        endwin();
        printf("Terminal doesn't have colors. Go succ a dicc.");
        exit(1);
    }
    start_color(); //duh
    curs_set(0); //Disable cursor
    
    init_pair(0,COLOR_BLACK,COLOR_BLACK);
    init_pair(1,COLOR_BLACK, COLOR_RED);
    init_pair(2,COLOR_BLACK, COLOR_GREEN);
    init_pair(3,COLOR_BLACK, COLOR_YELLOW);
    init_pair(4,COLOR_BLACK, COLOR_BLUE);
    init_pair(5,COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6,COLOR_BLACK, COLOR_CYAN);
    init_pair(7,COLOR_BLACK, COLOR_WHITE);
    //------------------------------------------
    //         END OF NCURSES SETTINGS
    //------------------------------------------
    
    //-------------------------------------------
    //        SETTING WINDOWS
    //-------------------------------------------
    
    int i_maxX,i_maxY;
    getmaxyx(stdscr,i_maxY,i_maxX);
  
    //This should about center everything
    i_startx = i_maxX/2 - FIELD_WIDTH - 17;
    i_starty = i_maxY/2 - FIELD_HEIGHT/2 - 1; 

    w_blocks = newwin(FIELD_HEIGHT+2,FIELD_WIDTH*2+2,i_starty,i_startx);
    w_instrucs = newwin(12,40,i_starty,i_startx + FIELD_WIDTH*2 + 5);
    w_score = newwin(5,30,i_starty+13,i_startx + FIELD_WIDTH*2 + 5);

    box(w_blocks,0,0);
    box(w_instrucs,0,0);
    box(w_score,0,0);

    mvwprintw(w_score,2,2,"Score: %d",score);
    mvwprintw(w_instrucs,2,2,"PRESS 'q' TO QUIT");
    mvwprintw(w_instrucs,4,2,"PRESS ANY BUTTON TO ADD POINTS");
    mvwprintw(w_instrucs,6,2,"PRESS '-' TO DECREMENT SCORE");
 
    //FIRST REFRESH:
    refresh();
    wrefresh(w_blocks);
    wrefresh(w_instrucs);
    wrefresh(w_score);
    //-----------------------------------------
    //            TETRIS LOGIC
    //-----------------------------------------

    //Use copy to restore block to original state after failed rotation / rotation clipping
    int block_copy[4][4];
    
    copyBlock(block_active,block_L);

    for(int i = 0; i < FIELD_HEIGHT*FIELD_WIDTH;++i){
        iarr_field[i]     = 0;
        iarr_tempField[i] = 0;
    }

    moveBlock(iarr_tempField, block_active, 5,5);
    
    int cTemp = 'i';
    //----------------------------------------------
    //            MAIN GAME LOOP YEEEEEE
    //----------------------------------------------
    while(cTemp != 'q'){ // PRESS Q TO QUIT, LOSER
        d_prevTime = d_currentTime;
        d_currentTime = (double)clock() / (double)CLOCKS_PER_SEC;
        
        for(int i = 0; i < FIELD_HEIGHT*FIELD_WIDTH;++i)
            iarr_tempField[i] = 0;

        //Apparently "clock" doesn't account for the 10ms caused by the "getch"-delay. Add them manually.
        d_timeAccum += d_currentTime - d_prevTime + 0.01;
        
        mvwprintw(w_score, 2, 2, "            ");
        mvwprintw(w_score, 2, 2, "Score: %d",score);
        wrefresh(w_score);

        int i_col = 0; 
        i_col = colCheck(iarr_field,block_active,i_activeYpos,i_activeXpos);
        mvwprintw(w_instrucs,8,2,"colstatus: %2d",i_col);
        wrefresh(w_instrucs);
         
        if(d_timeAccum >= d_dropTime){
            if( (i_col & 4) != 4){
                ++i_activeYpos;
                moveBlock(iarr_tempField,block_active,i_activeYpos,i_activeXpos);
            }
            else{
                moveBlock(iarr_field,block_active,i_activeYpos,i_activeXpos);
                i_activeXpos = FIELD_WIDTH/2 - 2;
                i_activeYpos = 0;
                moveBlock(iarr_tempField,block_active,i_activeYpos,i_activeXpos);
                score += i_blockPoints;
                i_blockType = copyRandomBlock(block_active); 
            }
            d_timeAccum = 0;
        }

        cTemp = getch();
        switch(cTemp){
            case KEY_RIGHT:
                if( (i_col & 2) < 2) 
                    i_activeXpos += moveBlock(iarr_tempField, block_active,i_activeYpos,i_activeXpos+1);
                break;
            case KEY_LEFT:
                if( (i_col & 8) < 8)
                    i_activeXpos -= moveBlock(iarr_tempField, block_active,i_activeYpos,i_activeXpos-1);
                break;
            case KEY_DOWN:
                if( (i_col & 4) < 4)
                    i_activeYpos += moveBlock(iarr_tempField, block_active,i_activeYpos+1,i_activeXpos);
                if( (i_col & 4) == 4){
                    moveBlock(iarr_field,block_active,i_activeYpos,i_activeXpos);   
                    i_activeXpos = FIELD_WIDTH/2 - 2;
                    i_activeYpos = 0;
                    score += i_blockPoints;
                    i_blockType = copyRandomBlock(block_active);
                }
                break;
            case ' ':
                while( (colCheck(iarr_field,block_active,i_activeYpos,i_activeXpos) & 4) != 4){
                    ++i_activeYpos;
                    moveBlock(iarr_tempField,block_active,i_activeYpos,i_activeXpos);
                }
                moveBlock(iarr_field,block_active,i_activeYpos,i_activeXpos);
                i_activeXpos = FIELD_WIDTH/2 - 2;
                i_activeYpos = 0;
                score += i_blockPoints;
                i_blockType = copyRandomBlock(block_active);
                break;
            case KEY_UP:
                copyBlock(block_copy, block_active);
                int rot = i_blockType > 2 ? 3 : 4;
                rotateBlock(block_active,rot);
                int tries = 0;
                while( (colCheck(iarr_field,block_active,i_activeYpos,i_activeXpos) == 15 ) 
                        && tries < 5){
                    //Half-assed implementation of "wallkicks" and "floor-kicks" here.
                    ++tries;
                    switch(tries){
                        case 1:
                            ++i_activeXpos; //Check what happens when moving block one to the right
                            break;
                        case 2:
                            --i_activeXpos; //Undo step in case 0
                            --i_activeXpos; //Block one to left
                            break;
                        case 3:
                            ++i_activeXpos;
                            ++i_activeYpos;
                            break;
                        case 4:
                            --i_activeYpos;
                            --i_activeYpos;
                            break;
                        case 5:
                            ++i_activeYpos;
                            break;
                    }//End switch
                }//End while
                if(tries == 5){
                    copyBlock(block_active,block_copy);   
                }
                break; //End case 'r'
            default:
                break;
        }//End switch case
        
        score += clearFullLines(iarr_field,i_linePoints);

        moveBlock(iarr_tempField, block_active, i_activeYpos, i_activeXpos); 

        drawField(w_blocks,iarr_field, ' ', 1);
        drawField(w_blocks,iarr_tempField,' ', 0);
        wrefresh(w_blocks);
    }//End main game loop
     
    //-----------------------------------------
    //         FREEING MEMORY
    //----------------------------------------
    wborder(w_blocks,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wborder(w_instrucs,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wborder(w_score,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    delwin(w_blocks);
    delwin(w_instrucs);
    delwin(w_score);
    endwin(); // VERY IMPORTANT!!!

    return 0;
}

int moveBlock(int iarr_field[FIELD_WIDTH*FIELD_HEIGHT], const int block[4][4], int y, int x){
    int success = 1; //Return 1 if block was put into iarr_field (i.e: no boundary overflow)
    for(int i = 0; i < 4; ++i){ //Use i for vert coords
        for(int k = 0; k < 4; ++k){ //Use k for horiz coords

            if(block[i][k] != 0){
                //Two different ifs: a "0" block shouldn't prvent success
                if((x + k ) < FIELD_WIDTH && (y + i) < FIELD_HEIGHT
                 && x + k >= 0 && y + i >= 0){}
                else{
                    success = 0;
                }
            }//end check if block element is 0
        }//end for k
    }//end for i
    if(success){
        for(int i = 0; i < 4; ++i){
            for(int k = 0; k < 4; ++k)
                if(block[i][k] != 0)
                    iarr_field[(y+i)*FIELD_WIDTH + x + k] = block[i][k];   
        }
    };
    return success;
}

void drawField(WINDOW *w_field, const int iarr_field[FIELD_WIDTH*FIELD_HEIGHT], char cFill, int clearZeroes){
    for(int i = 0; i < FIELD_WIDTH*FIELD_HEIGHT; ++i){
        if(iarr_field[i] != 0 || clearZeroes){
            wattron(w_field, COLOR_PAIR(iarr_field[i]));
            mvwaddch(w_field, i / FIELD_WIDTH+1, (i % FIELD_WIDTH)*2+1,cFill); 
            mvwaddch(w_field, i / FIELD_WIDTH+1, (i % FIELD_WIDTH)*2+2,cFill);
            wattroff(w_field, COLOR_PAIR(iarr_field[i]));
        }
    }
}

void copyBlock(int block_in[4][4], const int block_original[4][4]){
    for(int i = 0; i < 16; ++i){
        block_in[i/4][i%4] = block_original[i/4][i%4];   
    }
}

int colCheck(int iarr_field[FIELD_WIDTH*FIELD_HEIGHT], const int block[4][4], int blockY, int blockX){
    //Int direction: 0b(top)(right)(bottom)(left) is solid
    //e.g: if the block of field[] that is below the lowest solid element in block[][] is solid,
    //return collision-direction = bottom => direction = 0b0010 = 2
    int direction = 0b0000;

    //Say we have an x-y coordinate, get the field-array-element as follows:
    // field[a] = (x,y);
    // => a = y * FIELD_WIDTH + x
    for(int i = 0; i < 4; ++i){
        for(int k = 0; k < 4; ++k){
            //The checks with blockX and blockY make sure the function returns collision numbers for 
            //'out of bounds' situations as well. Also , we shouldn't get index outofbounds like this.
            if(block[i][k] != 0){
                //Top collision
                if(blockY + i < 1 || iarr_field[ (blockY + i - 1)*FIELD_WIDTH + blockX + k ] != 0){
                    direction = direction | 0b0001;
                }
                //Right collision
                if(blockX + k >= FIELD_WIDTH-1 || iarr_field[ (blockY + i)*FIELD_WIDTH + blockX + k + 1] != 0){
                    direction = direction | 0b0010;
                }
                //Bottom collision
                if(blockY + i >= FIELD_HEIGHT-1 || iarr_field[ (blockY + i + 1)*FIELD_WIDTH + blockX + k ] != 0){
                    direction = direction | 0b0100;
                }
                //Left collision
                if(blockX + k < 1 || iarr_field[ (blockY + i)*FIELD_WIDTH + blockX + k  - 1] != 0){
                    direction = direction | 0b1000;
                }
                //These blockX and blockY-checks are a bit different than the previous ones;
                //the previous set the collision status if a block was to the side of an edge of the field,
                //these just make sure we're still in the arena.
                if(blockX+k < 0 || blockY+i > FIELD_HEIGHT-1 || blockY+i < 0 || blockX+k > FIELD_WIDTH-1
                   || (iarr_field[ (blockY + i)*FIELD_WIDTH + blockX + k] != 0) ){
                    direction = direction | 0b1111;   
                }
            }
        }
    }
    return direction; 
}

//Some blocks are four sub-blocks high/wide, others only three.
//i_rows adapts the rotation algorithm to make the center of rotation be correct
void rotateBlock(int block[4][4], int i_rows){
    int temp = 0;
    //Dir 0 for left rotation, 1 for right
    //Permutate, then reverse rows
    for(int i = 0; i < i_rows; ++i){
        for(int k = i; k < i_rows; ++k){
            temp = block[i][k];
            block[i][k] = block[k][i];
            block[k][i] = temp;
        }
    }
    for(int i = 0; i < i_rows; ++i){
        for(int k = 0; k < 2; ++k){
            temp = block[i][k];
            block[i][k] = block[i][i_rows-1-k];
            block[i][i_rows-1-k] = temp;
        }
    }
}

int clearFullLines(int field[FIELD_WIDTH * FIELD_HEIGHT], const int i_pointsPerLine){
    int i_score = 0;
    int i_fullBlocks = 1; //store amount of solid blocks on line here.
    int i_prevSolids = 1;
    for(int i = FIELD_HEIGHT-1; i_fullBlocks > 0 && i > 0; --i){
        i_fullBlocks = 0;
        for(int k = 0; k < FIELD_WIDTH; ++k){
              if(field[i*FIELD_WIDTH + k] != 0)
                  ++i_fullBlocks;
        }
        if(i_fullBlocks == FIELD_WIDTH){ //If full line
            i_score += i_pointsPerLine;
            //First clear the line
            for(int j = 0; j < FIELD_WIDTH; ++j){
                field[i*FIELD_WIDTH + j] = 0; //Empty line
            }
            //then clear all lines above it
            for(int j = i; i_prevSolids > 0 && j > 1; --j){
                i_prevSolids = 0;
                for(int k = 0; k < FIELD_WIDTH; ++k){
                //Overwrite character with character one line above
                if(field[j*FIELD_WIDTH + k ] != 1)
                    ++i_prevSolids;

                field[(j)*FIELD_WIDTH + k] = field[(j-1)*FIELD_WIDTH + k];
                }
            }
        }
    }
    return i_score; 
}

//There's only 7 tetrominoes, so we'll use one int to return which block was placed.
int copyRandomBlock(int block_in[4][4]){
    int i_oneToSeven = rand() % 7 + 1;
    switch(i_oneToSeven){
        case 1:    
            copyBlock(block_in,block_square);
            break;
        case 2:
            copyBlock(block_in,block_I);
            break;
        case 3:
            copyBlock(block_in,block_T);
            break;
        case 4:
            copyBlock(block_in,block_S);
            break;
        case 5:
            copyBlock(block_in, block_Z);
            break;
        case 6:
            copyBlock(block_in, block_L);
            break;
        case 7:
            copyBlock(block_in,block_J);
            break;
        default:
            i_oneToSeven = 0;
            break;
    }//End switch
    return i_oneToSeven;
}
