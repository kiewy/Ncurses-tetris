#include "keyHandler.h"
#ifndef NCURSES
    #define NCURSES
    #include <ncurses.h>
#endif
#ifndef FUNCS
    #define FUNCS
    #define FUNCS
    #include "funcs.h"
#endif
    
// Returns which key was pressed
void keyHandler(int key,
                int block_active[4][4], int block_next[4][4],
                int iarr_field[], int iarr_tempField[], int i_fieldHeight, int i_fieldWidth,
                int * ip_activeYpos, int * ip_activeXpos, int i_col, int * score,
                int i_blockPoints, int * ip_blockType, int * ip_blockTypeNext,
                double * dp_dropTime){
    switch(key){
        case KEY_RIGHT:
            if( (i_col & 2) < 2) 
                *ip_activeXpos += moveBlock(iarr_tempField,i_fieldHeight,i_fieldWidth, block_active,*ip_activeYpos,*ip_activeXpos+1);
            break;
        case KEY_LEFT:
            if( (i_col & 8) < 8)
                *ip_activeXpos -= moveBlock(iarr_tempField,i_fieldHeight,i_fieldWidth, block_active,*ip_activeYpos,*ip_activeXpos-1);
            break;
        case KEY_DOWN:
            if( (i_col & 4) < 4)
                *ip_activeYpos += moveBlock(iarr_tempField,i_fieldHeight,i_fieldWidth, block_active,*ip_activeYpos+1,*ip_activeXpos);
            if( (i_col & 4) == 4){
                moveBlock(iarr_field,i_fieldHeight,i_fieldWidth,block_active,*ip_activeYpos,*ip_activeXpos);   
                *ip_blockType = *ip_blockTypeNext;
                copyBlock(block_active,block_next);
                *ip_blockTypeNext = copyRandomBlock(block_next);
                *ip_activeXpos = i_fieldWidth/2 - 2;
                *ip_activeYpos = 0;
                *score += i_blockPoints;
                *dp_dropTime = adjustSpeed(*score); 
            }
            break;
        case ' ':
            while( (colCheck(iarr_field,i_fieldHeight,i_fieldWidth,block_active,*ip_activeYpos,*ip_activeXpos) & 4) != 4){
                *ip_activeYpos += 1;
                moveBlock(iarr_tempField,i_fieldHeight,i_fieldWidth,block_active,*ip_activeYpos,*ip_activeXpos);
            }
            moveBlock(iarr_field,i_fieldHeight,i_fieldWidth,block_active,*ip_activeYpos,*ip_activeXpos);
            *ip_activeXpos = i_fieldWidth/2 - 1;
            *ip_activeYpos = 0;
            *ip_blockType = *ip_blockTypeNext;
            copyBlock(block_active,block_next);
            *ip_blockTypeNext = copyRandomBlock(block_next);
            *score += i_blockPoints;
            *dp_dropTime = adjustSpeed(*score);
            break;
        case KEY_UP:
            do{}while(0);
            int block_copy[4][4] = {0};
			//jank
            copyBlock(block_copy, block_active);
            int rot = *ip_blockType > 2 ? 3 : 4;
            rotateBlock(block_active,rot);
            int tries = 0;
            while( (colCheck(iarr_field,i_fieldHeight,i_fieldWidth,block_active,*ip_activeYpos,*ip_activeXpos) == 15 ) 
                    && tries < 4){
                //Half-assed implementation of "wallkicks" and "floor-kicks" here.
                ++tries;
                switch(tries){
                    case 1:
                        ++(*ip_activeXpos); //Check what happens when moving block one to the right
                        break;
                    case 2:
                        --(*ip_activeXpos); //Undo step in case 0
                        --(*ip_activeXpos); //Block one to left
                        break;
                    case 3:
                        ++(*ip_activeXpos);
                        *ip_activeYpos += 1;
                        break;
                    case 4:
                        *ip_activeYpos -= 1;
                        break;
				}//End switch
            }//End while
            if(tries == 4){
                copyBlock(block_active,block_copy);   
            }
            break; //End case 'r'
        default:
            break;
    }//End switch case
}
