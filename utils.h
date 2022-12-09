#include <fstream>
#include <iostream>

using namespace std;

const char title[] = "Zain-ul-din";

//---Height and Width of the Actual Interactive Game---//
const int M = 20;   //Number of rows for a piece to cover on the screen (not the entire screen) = 20
const int N = 10;   //Number of columns for a piece to cover on the screen (not the entire screen) = 10

//---The Actual Interactive Game Grid - Built Over (MxN)---//
int gameGrid[M][N] = {0};


//---To Hold the Coordinates of the Piece---//
int point_1[4][2], point_2[4][2];

//---To Hold the Coordinates of the Shadow Piece---//
int shadow_point_1 [4] [2], shadow_point_2 [4] [2];


//---Check Uncertain Conditions---//
bool anamoly(){
    for (int i=0;i<4;i++)
        if (point_1[i][0]<0 || point_1[i][0]>=N || point_1[i][1]>=M)
            return false;
        else if (gameGrid[point_1[i][1]][point_1[i][0]])
            return false;
    return true;
};

bool anamoly_shadow ()
{
    for (int i=0;i<4;i++)
        if (shadow_point_1[i][0]<0 || shadow_point_1[i][0]>=N || shadow_point_1[i][1]>=M)
            return false;
        else if (gameGrid[shadow_point_1[i][1]][shadow_point_1[i][0]])    
            return false;
    return true;
}

///
/// Helper functions
///

/// Defination

int clamp (int value, int min, int max);
bool matchAllInRow (int valueToMatch, int row, int arr [M][N]);
int loadHighScore (const char* fileName);
void saveHighScore (int newHighScore, const char* fileName);

/// Implementation

/// @brief loads high score from file
/// @param fileName 
/// @return high score
int loadHighScore (const char* fileName) 
{
    ifstream in;
    in.open (fileName, ios::in);
    if (in.fail ())
    {
        saveHighScore (0, fileName);
        in.close ();
        return 0;
    }
    int score = 0;
    in >> score;
    in.close ();
    return score;
}

/// @brief save high score to file
/// @param newHighScore 
/// @param fileName 
void saveHighScore (int newHighScore, const char* fileName)
{
    ofstream out;
    out.open (fileName, ios::out);
    out << newHighScore;
    out.close ();
}


/// Clamps value between max and min
/// \param value
/// \param min
/// \param max
/// \return
int clamp (int value, int min, int max)
{
    if (value <= min) return  min;
    else if (value >= max) return max;
    return value;
}

/// return true if given value present inside 2D array
/// \param column
/// \param arr
/// \return boolean
bool matchAllInRow (int valueToMatch, int row, int arr [M][N])
{
    for (int i = 0 ; i < N; i += 1)
        if (arr [row] [i] != valueToMatch) return false;
    return true;    
}

