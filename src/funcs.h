
double adjustSpeed(int score);

void copyBlock(int block_in[4][4], const int block_original[4][4]);
int clearFullLines(int field[], const int i_fieldHeight, const int i_fieldWidth, const int i_pointsPerLine);

int moveBlock(int iarr_field[],const int i_fieldHeight, const int i_fieldWidth, const int block[4][4], int y, int x);
int colCheck(const int iarr_field[], const int i_fieldHeight, const int i_fieldWidth, const int block[4][4], const int blockY, const int blockX);

void rotateBlock(int block[4][4], int i_rows);

int copyRandomBlock(int block_in[4][4]);

