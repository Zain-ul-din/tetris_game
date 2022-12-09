

#include <iostream>


void moveBlocksHorizontally (int points[4][2], int points2[4][2], int* inputX, int pointRows);
void rotateBlock (const bool* isRotating, int point1[4][2], int point2[4][2], int rows);
void setBlockCoordinates (int points[4][2], int blockArray[7][4], const bool* canDraw, const int* currBlockIdx);
void fillGameGridColors (int points[4][2], const int* colorNum);
int  getScore (int gameMap [M][N]);
bool getGameOverStatus (int gameMap [M][N]);
void IncreaseGameDifficulity (const float* timePassed, int* downWardSpeed);


/// @brief Increase game difficulity on the base of time passed
/// @param timePassed 
/// @param downWardSpeed 
void IncreaseGameDifficulity (const float* timePassed, int* downWardSpeed)
{
    if ((int)*timePassed % (5*60)) // min*sec
        *downWardSpeed += 1;
}


/// Returns true if game has been failed
/// \param gameMap
/// \return
bool getGameOverStatus (int gameMap [M][N])
{
    if (!matchAllInRow (0,0, gameMap)) return true;
    return false;
}

/// Score Counter
/// \return score
int getScore (int gameMap [M][N])
{
    int score = 0;
    int xMap = M-1;

    for (int x = xMap; x > 0; x -= 1)
    {
        int idx = 0;
        bool allNotZero = true;
        for (int y = 0 ; y <N ; y += 1)
        {
            if (gameMap [xMap][y]  == 0) 
                allNotZero = false;
            if (gameMap [x] [y] != 0) idx += 1;
            gameMap [xMap][y] = gameMap [x][y];    
        } 

        if (allNotZero) score += 1;
        if (idx < N) xMap -= 1;
    }

    /// Match colors aslo
    // for (int x = xMap; x > 0; x -= 1)
    // {
    //     int idx = 0;
    //     int prevVal = gameMap [x] [0];
    //     bool allSame = true;
    //     for (int y = 0 ; y < N; y += 1)
    //     {
    //         if (gameMap [xMap][y] != prevVal || gameMap [xMap][y]  == 0) allSame = false;
    //         prevVal = gameMap [xMap][y];
    //         if (gameMap [x] [y] != 0) idx += 1;
    //         gameMap [xMap][y] = gameMap [x][y];
    //     }
    //     if (allSame) score += 1;
    //     if (idx < N) xMap -= 1;
    // }

    return score;
}

/// Sets GameGrids colors
/// \param points
/// \param colorNum
void fillGameGridColors (int points[4][2], const int* colorNum)
{
    for (int i = 0 ; i  < 4; i += 1)
        gameGrid [points [i] [1]][points [i] [0]] = *colorNum;
}

/// Sets Block Coordinates
/// \param points
/// \param BLOCKS
/// \param canDraw
/// \param currBlockIdx
void setBlockCoordinates (int points[4][2], int blockArray[7][4], const bool* canDraw, const int* currBlockIdx)
{
    if (!(*canDraw)) return;

    for (int i = 0 ; i < 4; i += 1)
    {
        point_1[i][0] = blockArray[*currBlockIdx][i] % 2;
        point_1[i][1] = blockArray[*currBlockIdx][i] / 2;
    }

}



/// Moves blocks in X direction
/// \param points
/// \param inputX
void moveBlocksHorizontally (int points[4][2], int points2[4][2], int* inputX, int pointRows)
{
    
    for (int i =0; i < pointRows; i += 1)
    {
        points2 [i] [0] = points [i] [0];
        points2 [i] [1] = points [i] [1];
        points  [i] [0] += *inputX;
    }

    if (!anamoly())
    {
        for (int i = 0 ; i < pointRows; i += 1)
        {
            points [i] [0] = points2 [i] [0];
            points [i] [1] = points2 [i] [1];
        }
    }
    *inputX = 0;
}

/// Rotates block on the base of  user input
/// \param isRotating
/// \param point1
/// \param point2
/// \param rows
void rotateBlock (const bool* isRotating, int point1[4][2], int point2[4][2], int rows)
{
    if (!(*isRotating)) return;

    int cX = point1 [1] [0];
    int cY = point1 [1] [1];


    for (int i = 0 ; i < rows; i += 1)
    {
        int x = point1 [i] [1] - cY;
        int y = point1 [i] [0] - cX;

        point1 [i] [0] = cX - x;
        point1 [i] [1] = cY + y;

        // prev state
        shadow_point_2 [i] [0] = shadow_point_1[i][0];
        shadow_point_2 [i] [1] = shadow_point_1[i][1];

        shadow_point_1[i][0] = point1 [i] [0];
        shadow_point_1[i][1] = point1 [i] [1];

        if (!anamoly_shadow ())
        {
            shadow_point_1[i][0] = shadow_point_2 [i] [0];
            shadow_point_1[i][1] = shadow_point_2 [i] [1];
        }
    }


    if (!anamoly())
    {
        for (int i =0; i < rows; i += 1)
        {
            point1 [i] [0] = point2 [i] [0];
            point1 [i] [1] = point2 [i] [1];
        }
    }
}

// horizontalMovement


//---Piece Starts to Fall When Game Starts---//
void fallingPiece(float& timer, float& delay, int* currBlockIdx, int* colorNum, const int MAX_COLOR_NUM, const int* downWardSpeed)
{
    const int MAX_BLOCKS = 7;
    if (timer>delay)
    {
        for (int i=0;i<4;i++)
        {
            point_2[i][0] = point_1[i][0];
            point_2[i][1] = point_1[i][1];

            shadow_point_1 [i] [0] = point_2[i][0];
            shadow_point_1 [i] [1] = point_2[i][1];

            shadow_point_2 [i] [0] = shadow_point_1[i][0];
            shadow_point_2 [i] [1] = shadow_point_1[i][1]; 

            point_1[i][1] += *downWardSpeed;
        }
        
        
        {
            // int i = 0;
            while (anamoly_shadow ())
            {
                for (int i = 0 ; i < 4; i += 1)
                {
                    shadow_point_2 [i] [1] = shadow_point_1[i][1];
                    shadow_point_1[i][1] += 1;
                }
            }
            
            if(!anamoly_shadow ())
            {
                for (int j = 0 ; j < 4; j += 1)
                {
                    shadow_point_1 [j] [0] = shadow_point_2[j][0];
                    shadow_point_1 [j] [1] = shadow_point_2[j][1];
                }
            }
        }
        
        if (!anamoly())
        {
            fillGameGridColors(point_2, colorNum);
            int randomBlock = rand() % MAX_BLOCKS-1;
            randomBlock = clamp (randomBlock, 0, MAX_BLOCKS - 1);
            *currBlockIdx = randomBlock;
            *colorNum += 1;
            *colorNum = 1+ rand() % MAX_BLOCKS;

            for (int i=0; i<4; i+=1)
            {
                point_1[i][0] = BLOCKS[randomBlock][i] % 2;
                point_1[i][1] = BLOCKS[randomBlock][i] / 2;
            }
        }

        timer=0;
    }
}
