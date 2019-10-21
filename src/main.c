#ifndef NCURSES
    #define NCURSES
    #include <ncurses.h>
#endif

#ifndef TIME
    #define TIME
    #include <time.h>
#endif

#ifndef STDLIB
    #define STDBLIB
    #include <stdlib.h>
#endif

#ifndef BLOCKS
    #define BLOCKS
    #include "blocks.h"
#endif

#ifndef FUNCS
    #define FUNCS
    #include "funcs.h"
#endif

#ifndef KEYHANDLER
    #define KEYHANDLER
    #include "keyHandler.h"
#endif

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 25 

void drawField(WINDOW *w_field, const int iarr_field[], const int i_fieldHeight, const int i_fieldWidth, 
               char cFill, int clearZeroes);

void drawNextBlock(WINDOW *w_next, const int block_next[4][4], int size);

void shadowBlock(WINDOW* w_field,int iarr_field[], const int block[4][4],int i_ypos, int i_xpos,char c_fill);

int main(int argc, char *argv[])
{
    //---------------------------------------------------
    //     DECLARING DATA 
    //---------------------------------------------------
    //Time between automatic downwards movements of blocc 
   
    enum GAMESTATES{
        STATE_ALIVE = 1,
        STATE_DEAD = 2,
        STATE_PAUSED = 3,
        STATE_MENU = 4
    };
     
    int i_gameState = STATE_ALIVE;

    double d_dropTime = 0.5;
    int    i_linePoints  = 100;
    int    i_blockPoints = 5;

    WINDOW *w_blocks; //This will be the main playing field.
    WINDOW *w_instrucs;
    WINDOW *w_score; 
	WINDOW *w_nextBlock;

    //Store the active/moving block here, along with the block to be played next:
    int block_active[4][4]  = {0};
	int block_next[4][4]    = {0};
    int i_blockType         = 1;
	int i_blockTypeNext     = 1;
    //Coords of left top corner of active block:
    int i_activeXpos = 5,
        i_activeYpos = 5;
    //Use moveBlock the active block, and drawField this field apart from the main
    //field. This way, there's an easy way to clear coords previously occupied by the 
    //active block, and no data that should get modified by the active block gets modified by 
    //the active block.
    int iarr_tempField[FIELD_WIDTH*FIELD_HEIGHT]; 
    int i_startx = 5, //SET THESE TO A CALCULATED VALUE TO CENTER SHIT NICELY
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
    
    init_pair(0,COLOR_BLACK, COLOR_BLACK);
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
	w_nextBlock = newwin(6, 12, i_starty, i_startx + FIELD_WIDTH * 2 + 5);
    w_instrucs = newwin(12,30,i_starty + 7,i_startx + FIELD_WIDTH*2 + 5);
    w_score = newwin(5,20,i_starty+20,i_startx + FIELD_WIDTH*2 + 5);



    box(w_blocks,0,0);
    box(w_instrucs,0,0);
    box(w_score,0,0);
	box(w_nextBlock,0,0);

    mvwprintw(w_score,2,2,"Score: %d",score);
    mvwprintw(w_instrucs,2,2,"- arrows to move");
    mvwprintw(w_instrucs,3,2,"- spacebar to drop");
    mvwprintw(w_instrucs,4,2,"- UP arrow to rotate");
    mvwprintw(w_instrucs,5,2, "- q button to quit");
    mvwprintw(w_instrucs,7,2, "- p to pause");
    mvwprintw(w_instrucs,8,2, "- k to die");
 
    //FIRST REFRESH:
    //wrefresh(w_nextBlock);
    wrefresh(w_blocks);
    wrefresh(w_instrucs);
    wrefresh(w_score);
    //-----------------------------------------
    //            TETRIS LOGIC
    //-----------------------------------------
   
	i_blockType = copyRandomBlock(block_active);
	i_blockTypeNext = copyRandomBlock(block_next);

    for(int i = 0; i < FIELD_HEIGHT*FIELD_WIDTH;++i){
        iarr_field[i]     = 0;
        iarr_tempField[i] = 0;
    }

    moveBlock(iarr_tempField, FIELD_HEIGHT, FIELD_WIDTH, block_active, 5,5);
    
    int cTemp = 'i';
    //----------------------------------------------
    //            MAIN GAME LOOP YEEEEEE
    //----------------------------------------------
    while(cTemp != 'q'){ // PRESS Q TO QUIT, LOSER

		box(w_nextBlock,0,0);
		box(w_blocks,0,0);
		box(w_instrucs,0,0);
		box(w_score,0,0);

		mvwprintw(w_blocks,0,(FIELD_WIDTH*2+2) / 2 - 3,"TETRIS");
		mvwprintw(w_nextBlock,0,4,"NEXT");
		mvwprintw(w_instrucs,0,11,"CONTROLS");
		mvwprintw(w_score,0,7,"SCORE");

        d_prevTime = d_currentTime;
        d_currentTime = (double)clock() / (double)CLOCKS_PER_SEC;
        
        switch(i_gameState){
            case STATE_ALIVE:   
				//Draw "next block"
				drawNextBlock(w_nextBlock, block_next, i_blockTypeNext > 2 ? 3 : 4);

                for(int i = 0; i < FIELD_HEIGHT*FIELD_WIDTH;++i)
                    iarr_tempField[i] = 0;

                //Apparently "clock" doesn't account for the 10ms caused by the "getch"-delay. Add them manually.
                d_timeAccum += d_currentTime - d_prevTime + 0.01;
                
                mvwprintw(w_score, 2, 2, "            ");
                mvwprintw(w_score, 2, 2, "Score: %d",score);
                wrefresh(w_score);

                int i_col = 0; 
                i_col = colCheck(iarr_field,FIELD_HEIGHT,FIELD_WIDTH,block_active,i_activeYpos,i_activeXpos);
                wrefresh(w_instrucs);
                 
                if(d_timeAccum >= d_dropTime){
                    if( (i_col & 4) != 4){
                        ++i_activeYpos;
                        moveBlock(iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH,block_active,i_activeYpos,i_activeXpos);
                    }
                    else{
                        moveBlock(iarr_field,FIELD_HEIGHT,FIELD_WIDTH,block_active,i_activeYpos,i_activeXpos);
						copyBlock(block_active, block_next);
						i_blockType         = i_blockTypeNext;
                        i_blockTypeNext     = copyRandomBlock(block_next);
                        i_activeXpos        = FIELD_WIDTH/2 - 2;
                        i_activeYpos        = 0;
                        moveBlock(iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH,block_active,i_activeYpos,i_activeXpos);
                        score += i_blockPoints;
                    }
                    d_timeAccum = 0;
                }

                if(cTemp == 'p')
                    i_gameState = STATE_PAUSED;
                
                cTemp = getch();
               
                /* THIS HANDLES KEYS REALLY WELL */
                //he typed, hoping this time the program would compile
                //And lo and behold, it fuckin did
                keyHandler(cTemp,block_active,block_next,
                           iarr_field,iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH,
                           &i_activeYpos,&i_activeXpos, i_col, &score,
                           i_blockPoints, &i_blockType, &i_blockTypeNext);

                score += clearFullLines(iarr_field,FIELD_HEIGHT,FIELD_WIDTH,i_linePoints);

                if((colCheck(iarr_field,FIELD_HEIGHT,FIELD_WIDTH,block_active,i_activeYpos,i_activeXpos) == 15 
                    && i_activeYpos <= 2) || cTemp == 'k')
                    i_gameState = STATE_DEAD;

                moveBlock(iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH, block_active, i_activeYpos, i_activeXpos); 
                //END ALIVE-GAMESTATE-LOOP
                drawField(w_blocks,iarr_field,FIELD_HEIGHT,FIELD_WIDTH, ' ', 1);
                shadowBlock(w_blocks,iarr_field,block_active,i_activeYpos,i_activeXpos,'|');
                drawField(w_blocks,iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH,' ', 0);

                break;

            case STATE_DEAD:
                
                for(int i = 0; i < FIELD_HEIGHT*FIELD_WIDTH;++i)
                    iarr_field[i] = 0;

                drawField(w_blocks,iarr_field,FIELD_HEIGHT,FIELD_WIDTH, ' ', 1);
                drawField(w_blocks,iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH,' ', 0);

                wattron(w_blocks,COLOR_PAIR(7));
                mvwprintw(w_blocks,FIELD_HEIGHT / 2 - 2, FIELD_WIDTH/2 + 2,"YOU DIED");
                mvwprintw(w_blocks,FIELD_HEIGHT/2-1,FIELD_WIDTH/2-3,"PRESS r TO RESTART");
                wattroff(w_blocks,COLOR_PAIR(7));

                cTemp = getch();
                if(cTemp == 'r'){
                    i_activeXpos = FIELD_WIDTH / 2 - 2;
                    i_blockType = copyRandomBlock(block_active);
                    i_activeYpos = 0;
                    score = 0;
                    i_gameState = STATE_ALIVE;
                    cTemp = 'i';
                }
                break;
            case STATE_PAUSED:
                
                cTemp = getch();

                drawField(w_blocks,iarr_field,FIELD_HEIGHT,FIELD_WIDTH, ' ', 1);
                drawField(w_blocks,iarr_tempField,FIELD_HEIGHT,FIELD_WIDTH,' ', 0);

                wattron(w_blocks,COLOR_PAIR(7));
                mvwprintw(w_blocks,FIELD_HEIGHT/2,FIELD_WIDTH-2,"PAUSED");
                mvwprintw(w_blocks,FIELD_HEIGHT/2+1,FIELD_WIDTH-9,"PRESS 'p' TO RESUME");
                wattroff(w_blocks,COLOR_PAIR(7));
                if(cTemp == 'p'){
                    i_gameState = STATE_ALIVE;
                    cTemp = 'i';
                }
                break;

            case STATE_MENU:

                break;
        }

        wrefresh(w_blocks);
		wrefresh(w_nextBlock);
        refresh();
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

void drawField(WINDOW *w_field, const int iarr_field[], const int i_fieldHeight, const int i_fieldWidth, 
               char cFill, int clearZeroes){
    for(int i = 0; i < i_fieldWidth*i_fieldHeight; ++i){
        if(iarr_field[i] != 0 || clearZeroes){
            wattron(w_field, COLOR_PAIR(iarr_field[i]));
            mvwaddch(w_field, i / i_fieldWidth+1, (i % i_fieldWidth)*2+1,cFill); 
            mvwaddch(w_field, i / i_fieldWidth+1, (i % i_fieldWidth)*2+2,cFill);
            wattroff(w_field, COLOR_PAIR(iarr_field[i]));
        }
    }
}

void shadowBlock(WINDOW* w_field,int iarr_field[], const int block[4][4],int i_ypos, int i_xpos,char c_fill){
	int i_ymax = i_ypos;
	while((colCheck(iarr_field,FIELD_HEIGHT,FIELD_WIDTH,block,i_ymax,i_xpos) & 4) < 4){
		++i_ymax;
	}
	for(int i = 0; i < 4; ++i){
		for(int k = 0; k < 4; ++k){
			if(block[i][k] != 0){
				wattron(w_field,COLOR_PAIR(block[i][k]));
				mvwaddch(w_field,i_ymax+i+1,(i_xpos+k)*2+1,c_fill);
				mvwaddch(w_field,i_ymax+i+1,(i_xpos+k)*2+2,c_fill);
				wattroff(w_field,COLOR_PAIR(block[i][k]));
			}
		}
	}
}
 

void drawNextBlock(WINDOW *w_next, const int block_next[4][4], int i_size){
	for(int i = 0; i < 4; ++i){
	    for(int k = 1; k < 8; ++k){
			wattron(w_next,COLOR_PAIR(0));
			mvwaddch(w_next, i + 1, k,' ');
			wattroff(w_next,COLOR_PAIR(0));
	    } 
	}
	int offset = 5 - i_size;
	for(int i = 0; i < i_size; ++i){
		for(int k = 0; k < i_size; ++k){
			wattron(w_next, COLOR_PAIR(block_next[i][k]));
			mvwaddch(w_next,i+offset,2*k+offset,' ');
			mvwaddch(w_next,i+offset,2*k+offset+1,' ');
			wattroff(w_next,COLOR_PAIR(block_next[i][k]));
		}
	}
}
