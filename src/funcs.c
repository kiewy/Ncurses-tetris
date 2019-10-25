#ifndef BLOCKS
    #define BLOCKS
    #include "blocks.h"
#endif

#ifndef STDLIB
    #define STDLIB
    #include <stdlib.h>
#endif

#ifndef TIME
    #define TIME
    #include <time.h>
#endif

#include "funcs.h"

int moveBlock(int iarr_field[],const int i_fieldHeight, const int i_fieldWidth, const int block[4][4], const int y, const int x){
    int success = 1; //Return 1 if block was put into iarr_field (i.e: no boundary overflow)
    for(int i = 0; i < 4; ++i){ //Use i for vert coords
        for(int k = 0; k < 4; ++k){ //Use k for horiz coords

            if(block[i][k] != 0){
                //Two different ifs: a "0" block shouldn't prvent success
                if((x + k ) < i_fieldWidth && (y + i) < i_fieldHeight
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
                    iarr_field[(y+i)* i_fieldWidth + x + k] = block[i][k];   
        }
    };
    return success;
}

void copyBlock(int block_in[4][4], const int block_original[4][4]){
    for(int i = 0; i < 16; ++i){
        block_in[i/4][i%4] = block_original[i/4][i%4];   
    }
}

int colCheck(const int iarr_field[], const int i_fieldHeight, const int i_fieldWidth,
             const int block[4][4], const int blockY, const int blockX){
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
                if(blockY + i < 1 || iarr_field[ (blockY + i - 1)*i_fieldWidth + blockX + k ] != 0){
                    direction = direction | 0b0001;
                }
                //Right collision
                if(blockX + k >= i_fieldWidth-1 || iarr_field[ (blockY + i)*i_fieldWidth + blockX + k + 1] != 0){
                    direction = direction | 0b0010;
                }
                //Bottom collision
                if(blockY + i >= i_fieldHeight-1 || iarr_field[ (blockY + i + 1)*i_fieldWidth + blockX + k ] != 0){
                    direction = direction | 0b0100;
                }
                //Left collision
                if(blockX + k < 1 || iarr_field[ (blockY + i)*i_fieldWidth + blockX + k  - 1] != 0){
                    direction = direction | 0b1000;
                }
                //These blockX and blockY-checks are a bit different than the previous ones;
                //the previous set the collision status if a block was to the side of an edge of the field,
                //these just make sure we're still in the arena.
                if(blockX+k < 0 || blockY+i > i_fieldHeight-1 || blockY+i < 0 || blockX+k > i_fieldWidth-1
                   || (iarr_field[ (blockY + i)*i_fieldWidth + blockX + k] != 0) ){
                    direction = direction | 0b1111;   
                }
            }
        }
    }
    return direction; 
}

//Some blocks are four sub-blocks high/wide, others only three.
//i_rows adapts the rotation algorithm to make the center of rotation be correct
void rotateBlock(int block[4][4], const int i_rows){
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

int clearFullLines(int field[], const int i_fieldHeight, const int i_fieldWidth, const int i_pointsPerLine){
    int i_score = 0;
    int i_fullBlocks = 1; //store amount of solid blocks on line here.
    int i_prevSolids = 1;
    for(int i = i_fieldHeight-1; i_fullBlocks > 0 && i > 0; --i){
        i_fullBlocks = 0;
        for(int k = 0; k < i_fieldWidth; ++k){
              if(field[i* i_fieldWidth + k] != 0)
                  ++i_fullBlocks;
        }
        if(i_fullBlocks == i_fieldWidth){ //If full line
            i_score += i_pointsPerLine;
            //First clear the line
            for(int j = 0; j < i_fieldWidth; ++j){
                field[i*i_fieldWidth+ j] = 0; //Empty line
            }
            //then clear all lines above it
            for(int j = i; i_prevSolids > 0 && j > 1; --j){
                i_prevSolids = 0;
                for(int k = 0; k < i_fieldWidth; ++k){
                //Overwrite character with character one line above
                if(field[j*i_fieldWidth + k ] != 1)
                    ++i_prevSolids;

                field[(j)* i_fieldWidth + k] = field[(j-1)*i_fieldWidth + k];
                }
            }
        }
    }
    return i_score; 
}

//There's only 7 tetrominoes, so we'll use one int to return which block was placed.
int copyRandomBlock(int block_in[4][4]){
    srand(clock());
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

double adjustSpeed(int i_score){
	double i_speed = 0.7;
	
	if(i_score >= 10000)
		i_speed -= 0.5;
	else if(i_score >= 8000)
		i_speed -= 0.4;
	else if(i_score >= 5000)
		i_speed -= 0.3;
	else if(i_score >= 1000){
		i_speed -= 0.2;
	}
	return i_speed;
}

