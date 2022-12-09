#include <SFML/Graphics.hpp>
#include <time.h>
#include "utils.h"
#include "pieces.h"
#include "functionality.h"


using namespace sf;

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/// Loads texture from given path
/// \param texture
/// \param path
void loadTexture (Texture* texture, const string* path);
void loadFont (Font* font, const string* path);


int main(){
    srand(time(0));
    RenderWindow window(VideoMode(320, 480), title);
    Texture obj1, obj2, obj3, shadowBlock;

    Font font;

    const string OBJ1_LOADED_PATH = "./img/tiles.png";
    const string OBJ2_LOADED_PATH = "./img/background.png";
    const string OBJ3_LOADED_PATH = "./img/frame.png";
    const string FONT_LOADED_PATH = "./font/DMR.otf";
    const string SHADOW_BLOCK_LOADED_PATH = "./img/box.png";
    const char* SAVE_SYSTEM_FILE_PATH = "./score.txt";

    const int CELL_RADIUS = 18;
    const int MAX_COLOR_NUM = 8;
    const int SCORE_MULTIPLIER = 10;

    int gameScore = 0;
    int highScore = loadHighScore (SAVE_SYSTEM_FILE_PATH);
    bool gameOver = false;

    loadTexture(&obj1, &OBJ1_LOADED_PATH);
    loadTexture(&obj2, &OBJ2_LOADED_PATH);
    loadTexture(&obj3, &OBJ3_LOADED_PATH);
    loadTexture (&shadowBlock, &SHADOW_BLOCK_LOADED_PATH);
    loadFont (&font, &FONT_LOADED_PATH);

    //    https://www.sfml-dev.org/tutorials/2.5/graphics-text.php
    Text gameOverText, scoreText, highScoreText;
    gameOverText.setString("GAME OVER!");
    gameOverText.setFillColor(Color::Red);
    gameOverText.setFont(font);
    
    scoreText.setFont(font);
    scoreText.setFillColor(Color::White);

    highScoreText.setFont (font);
    highScoreText.setFillColor (Color::White);

    /*
        Load "frames.png","tiles.png" and "background.png" for Texture objects
    */
   

    Sprite sprite(obj1), background(obj2), frame(obj3), shadow (shadowBlock);
    int delta_x=0, colorNum=1;
    const float DEFAULT_DELAY = 0.3;
    float timer=0, delay=DEFAULT_DELAY;
    bool rotate= false;
    Clock clock;

    int blockIdx = 3;

    bool canSetBlockCoordinates = point_1 [0][0] == 0;
    setBlockCoordinates(point_1, BLOCKS, &canSetBlockCoordinates, &blockIdx);
    
    float timePassed = 0;
    int   downWardSpeed = 1;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        //---Event Listening Part---//

        Event e;
        while (window.pollEvent(e))
        {                    //Event is occurring - until the game is in running state
            if (e.type == Event::Closed)                   //If cross/close is clicked/pressed
                window.close();                            //Opened window disposes
            if (e.type == Event::KeyPressed)
            {             //If any other key (not cross) is pressed
                if (e.key.code == Keyboard::Up)            //Check if the other key pressed is UP key
                    rotate = true;                         //Rotation gets on
                else if (e.key.code == Keyboard::Left)     //Check if the other key pressed is LEFT key
                    delta_x = -1;                          //Change in X-Axis - Negative
                else if (e.key.code == Keyboard::Right)    //Check if the other key pressed is RIGHT key
                    delta_x = 1;                           //Change in X-Axis - Positive
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))   //Just another way to detect key presses without event listener
            delay=0.05;                                    //If DOWN key is pressed, delay in falling piece decreases from 0 to 0.05



        ///////////////////////////////////////////////
        ///*** START CALLING YOUR FUNCTIONS HERE ***///

        IncreaseGameDifficulity (&timePassed, &downWardSpeed);// game difficulty handler
        moveBlocksHorizontally (point_1, point_2, &delta_x, 4);// moves block left right
        rotateBlock (&rotate, point_1, point_2, 4);//  rotates block
        fallingPiece(timer, delay, &blockIdx, &colorNum, MAX_COLOR_NUM, &downWardSpeed);// moves block downward
        
        gameScore += getScore(gameGrid) * SCORE_MULTIPLIER; // gets game score
        gameOver = getGameOverStatus (gameGrid); // check if game has been overed
        scoreText.setString("Score: " + to_string(gameScore)); // updates game score
        highScoreText.setString ("High Score: " + to_string (highScore));// updates high score

        ///*** YOUR CALLING STATEMENTS END HERE ***///  
        //////////////////////////////////////////////

        
        window.clear(Color::Black);
        window.draw(background);

        /// renders shadows
        for (int i =0 ; i < 4; i += 1)
        {
            shadow.setTextureRect (IntRect(1*CELL_RADIUS,0,CELL_RADIUS,CELL_RADIUS));
            shadow.setPosition(shadow_point_1[i][0]*CELL_RADIUS,shadow_point_1[i][1]*CELL_RADIUS);
            shadow.move(28,31);
            window.draw (shadow);
        }

        // renders grid
        for (int i=0; i<M; i++)
        {
            for (int j=0; j<N; j++)
            {
                if (gameGrid[i][j]==0) continue;
                sprite.setTextureRect(IntRect(gameGrid[i][j]*CELL_RADIUS,0,CELL_RADIUS,CELL_RADIUS));
                sprite.setPosition(j*CELL_RADIUS,i*CELL_RADIUS);
                sprite.move(28,31); // fix X offset
                window.draw(sprite);   
            }
        }

        /// renders textures
        for (int i = 0; i< 4; i += 1)
        {
            sprite.setTextureRect(IntRect(colorNum*CELL_RADIUS,0,CELL_RADIUS,CELL_RADIUS));
            sprite.setPosition(point_1[i][0]*CELL_RADIUS,point_1[i][1]*CELL_RADIUS);
            sprite.move(28,31);
            window.draw(sprite);
        }


        delay = DEFAULT_DELAY;
        rotate = false;

        //---The Final on Which Everything is Drawn Over is Loaded---//
        window.draw(frame);

        // high score save system
        if (gameScore > highScore)
        {
            highScore = gameScore;
            saveHighScore (highScore, SAVE_SYSTEM_FILE_PATH);
        }

        // game over handler
        if (gameOver)
        {
            // RE-RENDER
            window.clear(Color::Black);
            window.draw(background);
            window.draw(frame);

            gameOverText.setCharacterSize(32);
            gameOverText.setPosition(32, 300);
            window.draw(gameOverText);
        }

        // draw score
        scoreText.setCharacterSize(15);
        scoreText.setPosition(200/10, 3);
        window.draw(scoreText);

        // draw high score
        highScoreText.setCharacterSize(15);
        highScoreText.setPosition(200/1.7, 3);
        window.draw (highScoreText);

        //---The Window that now Contains the Frame is Displayed---//
        window.display();
    }
    return 0;
}

/// Loads texture from given path
/// \param texture
/// \param path
void loadTexture (Texture* texture, const string* path)
{
    bool isLoaded = (*texture).loadFromFile(*path);
    if (!isLoaded)
        cout << "Warning: Can't load texture from path " << path << "\n";
}

/// Loads font from given path
/// \param font
/// \param path
void loadFont (Font* font, const string* path)
{
    bool isLoaded = (*font).loadFromFile(*path);
    if (!isLoaded)
        cout << "Warning: Can't load font from path " << path << "\n";
}