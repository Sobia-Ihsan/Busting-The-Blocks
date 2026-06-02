#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
using namespace std;
const int COLS = 10; // keep more than 6
const int ROWS = 20; // keep more than 5
const float cellsize = 25.f;
int *currentblockinitials[12] = {0};

bool checkCollision(int arr[][COLS])
{
    bool canMove = true;

    for (int k = 0; k < 12; k++)
    {
        if (currentblockinitials[k] == nullptr)
            continue;

        int row = (currentblockinitials[k] - &arr[0][0]) / COLS;

        if (row == ROWS - 1)
        {
            canMove = false;
            break;
        }

        if (*(currentblockinitials[k] + COLS) == 1)
        {
            canMove = false;
            break;
        }
    }
    return canMove;
}
int ClearLines(int arr[][COLS], int score)
{
    for (int row = 0; row < ROWS; row++)
    {
        bool full = true;

        for (int col = 0; col < COLS; col++)
        {
            if (arr[row][col] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int r = row; r > 0; r--) // going backwards because first the lowest complete row will be copied then next then next and else wise it will copy the contents of first row into all others
            {
                for (int c = 0; c < COLS; c++)
                {
                    arr[r][c] = arr[r - 1][c];
                }
            }
            for (int c = 0; c < COLS; c++) // clearing top most row
            {
                arr[0][c] = 0;
            }
            score += 100;
            row++;
        }
    }
    return score;
}
void moveBlock()
{
    for (int k = 0; k < 12; k++)
    {
        if (currentblockinitials[k] != nullptr)
        {
            *currentblockinitials[k] = 0; // clear the current block at its place
        }
    }
    for (int k = 0; k < 12; k++)
    {
        if (currentblockinitials[k] != nullptr)
        {
            currentblockinitials[k] += COLS; // changing pointers to the next row by adding number of columns
        }
    }
    for (int k = 0; k < 12; k++)
    {
        if (currentblockinitials[k] != nullptr)
        {
            *currentblockinitials[k] = 2; // set the values at the new pointer to 1
        }
    }
}
void block_1(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    // 1 1 1 1
    // 1 0 0 0
    // 1 0 0 0
    // 1 0 0 0
    int length_horizontal = rand() % 3 + 1;
    int length_vertical = rand() % 3 + 1;
    int start = rand() % (COLS - length_horizontal);
    int k = 0;
    for (int i = 0; i < length_vertical; i++)
    {
        arr[i][start] = 2;
        currentblockinitials[k] = &arr[i][start];
        k++;
    }
    for (int j = start + 1; j <= length_horizontal + start; j++)
    {
        arr[0][j] = 2;
        currentblockinitials[k] = &arr[0][j];
        k++;
    }
}
void block_2(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    int k = 0;
    // 1 1 1
    int length = rand() % 3 + 1;
    int start = rand() % (COLS - length);
    for (int i = start; i < length + start; i++)
    {
        arr[0][i] = 2;
        currentblockinitials[k] = &arr[0][i];
        k++;
    }
}
void block_3(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    int k = 0;
    // 1
    // 1
    // 1
    int length = rand() % 3 + 1;
    int start = rand() % (COLS - length);
    for (int i = 0; i < length; i++)
    {
        arr[i][start] = 2;
        currentblockinitials[k] = &arr[i][start];
        k++;
    }
}
void block_4(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    int k = 0;
    // 1 0 0
    // 1 0 0
    // 1 0 0
    // 1 1 1
    int length_horizontal = rand() % 3 + 1;
    int length_vertical = rand() % 3 + 1;
    int start = rand() % (COLS - length_horizontal);
    for (int i = 0; i <= length_vertical; i++)
    {
        arr[i][start] = 2;
        currentblockinitials[k] = &arr[i][start];
        k++;
    }

    for (int j = start + 1; j < length_horizontal + start; j++)
    {
        arr[length_vertical][j] = 2;
        currentblockinitials[k] = &arr[length_vertical][j];
        k++;
    }
}
void block_5(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    // 1 1 0
    // 0 1 1
    int length = 2;
    int start = rand() % (COLS - 2 - length) + 0;
    arr[0][start] = 2;
    currentblockinitials[0] = &arr[0][start];
    arr[0][start + 1] = 2;
    currentblockinitials[1] = &arr[0][start + 1];
    arr[1][start + 1] = 2;
    currentblockinitials[2] = &arr[1][start + 1];
    arr[1][start + 2] = 2;
    currentblockinitials[3] = &arr[1][start + 2];
}
void block_6(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    // 0 1 0
    // 1 1 1
    int length = 2;
    int start = rand() % (COLS - 2 - length) + 0;
    arr[0][start + 1] = 2;
    currentblockinitials[0] = &arr[0][start + 1];
    arr[1][start] = 2;
    currentblockinitials[1] = &arr[1][start];
    arr[1][start + 1] = 2;
    currentblockinitials[2] = &arr[1][start + 1];
    arr[1][start + 2] = 2;
    currentblockinitials[3] = &arr[1][start + 2];
}
void block_7(int arr[][COLS], int rows)
{
    for (int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    // 1 0 0
    // 1 1 0
    // 1 0 0
    int length = 2;
    int start = rand() % (COLS - 2 - length) + 0;
    arr[0][start] = 2;
    currentblockinitials[0] = &arr[0][start];
    arr[1][start] = 2;
    currentblockinitials[1] = &arr[1][start];
    arr[1][start + 1] = 2;
    currentblockinitials[2] = &arr[1][start + 1];
    arr[2][start] = 2;
    currentblockinitials[3] = &arr[2][start];
}
void spawnNewPiece(int arr[][COLS])
{
    int newblock = rand() % 7 + 1;
    if (newblock == 1)
    {
        block_1(arr, ROWS);
    }
    else if (newblock == 2)
    {
        block_2(arr, ROWS);
    }
    else if (newblock == 3)
    {
        block_3(arr, ROWS);
    }
    else if (newblock == 4)
    {
        block_4(arr, ROWS);
    }
    else if (newblock == 5)
    {
        block_5(arr, ROWS);
    }
    else if (newblock == 6)
    {
        block_6(arr, ROWS);
    }
    else
    {
        block_7(arr, ROWS);
    }
}
int main()
{
    bool playAgain = true;

    while(playAgain)
    {
        playAgain=false;
        int arr[ROWS][COLS] = {0};

        int blockColour;
        sf::Font font;
        bool gameOverFlag = false;
        if (!font.openFromFile("ARIALI.TTF"))
        {
            return -1;
        }
        bool activeBlock = false;
        int score = 0;
        srand(time(0));

        sf::RectangleShape rect(sf::Vector2f(cellsize - 2, cellsize - 2));
        rect.setFillColor(sf::Color::Red);
        // setting the background:
        sf::Texture bgTexture;

        if (!bgTexture.loadFromFile("gameoverbackground.png"))
        {
            return -1; // Handle error
        }

        sf::Sprite backgroundSpriteforend(bgTexture);

        // Scale background to fit window
        sf::Vector2u bgSize = bgTexture.getSize();

        backgroundSpriteforend.setScale({800.f / bgSize.x, 600.f / bgSize.y});
        // setting background for game over window:
        sf::Texture bgTextureforend;

        if (!bgTextureforend.loadFromFile("gameoverbackground.png"))
        {
            return -1; // Handle error
        }

        sf::Sprite backgroundSprite(bgTextureforend);

        // Scale background to fit window
        sf::Vector2u bgSizeforend = bgTextureforend.getSize();

        backgroundSprite.setScale({800.f / bgSize.x, 600.f / bgSize.y});
        // creating windows and grid
        sf::RenderWindow window(sf::VideoMode({800, 600}), "Block Buster");
        sf::RectangleShape gridcell(sf::Vector2f(cellsize, cellsize));
        gridcell.setFillColor(sf::Color::Black);
        gridcell.setOutlineThickness(1);
        gridcell.setOutlineColor(sf::Color(40, 40, 40));

        auto lastGravity = std::chrono::steady_clock::now();
        while (window.isOpen() && gameOverFlag == false)
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
            }
            static auto lastMove = std::chrono::steady_clock::now();

            auto now = std::chrono::steady_clock::now();

            if (std::chrono::duration<float>(now - lastMove).count() > 0.15f)
            {

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                {
                    bool canMoveLeft = true;

                    for (int k = 0; k < 12; k++)
                    {
                        if (currentblockinitials[k] != nullptr)
                        {
                            int col = (currentblockinitials[k] - &arr[0][0]) % COLS;

                            if (col == 0)
                            {
                                canMoveLeft = false;
                                break;
                            }

                            if (*(currentblockinitials[k] - 1) == 1)
                            {
                                canMoveLeft = false;
                                break;
                            }
                        }
                    }
                    if (canMoveLeft)
                    {
                        for (int k = 0; k < 12; k++)
                        {
                            if (currentblockinitials[k] != nullptr)
                            {
                                *currentblockinitials[k] = 0;
                            }
                        }

                        for (int k = 0; k < 12; k++)
                        {
                            if (currentblockinitials[k] != nullptr)
                            {
                                currentblockinitials[k]--;
                            }
                        }

                        for (int k = 0; k < 12; k++)
                        {
                            if (currentblockinitials[k] != nullptr)
                            {
                                *currentblockinitials[k] = 2;
                            }
                        }
                    }
                    lastMove = now;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                {
                    bool canMoveRight = true;

                    for (int k = 0; k < 12; k++)
                    {
                        if (currentblockinitials[k] != nullptr)
                        {
                            int col = (currentblockinitials[k] - &arr[0][0]) % COLS;

                            if (col == COLS - 1)
                            {
                                canMoveRight = false;
                                break;
                            }

                            if (*(currentblockinitials[k] + 1) == 1)
                            {
                                canMoveRight = false;
                                break;
                            }
                        }
                    }
                    if (canMoveRight)
                    {
                        for (int k = 0; k < 12; k++)
                        {
                            if (currentblockinitials[k] != nullptr)
                                *currentblockinitials[k] = 0;
                        }

                        for (int k = 0; k < 12; k++)
                        {
                            if (currentblockinitials[k] != nullptr)
                                currentblockinitials[k]++;
                        }

                        for (int k = 0; k < 12; k++)
                        {
                            if (currentblockinitials[k] != nullptr)
                                *currentblockinitials[k] = 2;
                        }
                    }
                    lastMove = now;
                }
            }

            now = std::chrono::steady_clock::now();
            // spawning new block
            if (!activeBlock)
            {
                spawnNewPiece(arr);
                activeBlock = true;
            }
            float gravitySpeed = 1.0f; // gravity working after each 1 second
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                gravitySpeed = 0.1f; // 10x faster if down key presssed
            }

            std::chrono::duration<float> gravityElapsed = now - lastGravity;

            if (gravityElapsed.count() >= gravitySpeed) // if (time of gravity passes ) only then move the blocks  down
            {
                lastGravity = now;
                // checking for collision
                bool canMove = checkCollision(arr);

                if (!canMove)
                {
                    // change active block to fixed block:
                    for (int k = 0; k < 12; k++)
                    {
                        if (currentblockinitials[k] != nullptr)
                        {
                            *currentblockinitials[k] = 1;
                        }
                    }
                    // check for more space to end game:
                    for (int j = 0; j < COLS; j++)
                    {
                        if (arr[0][j] == 1 || arr[0][j] == 2)
                        {
                            gameOverFlag = true;
                        }
                    }
                    activeBlock = false;

                    for (int k = 0; k < 12; k++)
                    {
                        currentblockinitials[k] = nullptr; // removing this block from current block pointers
                    }
                    // Line Clearing:
                    score = ClearLines(arr, score);
                }
                // moving block down
                else
                {
                    moveBlock();
                }
            }
            window.clear();
            window.draw(backgroundSprite);

            float boardWidth = COLS * cellsize;
            float boardHeight = ROWS * cellsize;

            float marginX = (800 - boardWidth) / 2.0f;
            float marginY = (600 - boardHeight) / 2.0f;
            // printing the grid
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    gridcell.setPosition({marginX + j * cellsize, marginY + i * cellsize});

                    window.draw(gridcell);
                }
            }
            // printing the blocks
            for (int i = 0; i < ROWS; i++)
            {
                for (int j = 0; j < COLS; j++)
                {
                    if (arr[i][j] == 1)
                    {
                        rect.setFillColor(sf::Color(120, 20, 20));
                    }
                    else if (arr[i][j] == 2)
                    {
                        rect.setFillColor(sf::Color(255, 50, 50));
                    }
                    if (arr[i][j] == 1 || arr[i][j] == 2)
                    {
                        rect.setPosition({marginX + j * cellsize, marginY + i * cellsize});

                        window.draw(rect);
                    }
                }
            }
            auto lastUpdateforgravity = std::chrono::steady_clock::now();
            // printing the score
            sf::Text scoreDisplay(font);
            scoreDisplay.setString("Score: " + std::to_string(score));
            window.draw(scoreDisplay);
            window.display();
        }
        int highscore = 0;
        int scorefromfile;
        bool fileopenedforread = true;
        bool fileopenedforwrite = true;
        // write score to file :
        ofstream fout;
        fout.open("score.txt", ios::app);
        if (!fout.fail())
        {
            fout << score << endl;
            fout.close();
        }
        else
        {
            fileopenedforwrite = false;
        }
        // retrieve score from file :
        ifstream fin;
        fin.open("score.txt");
        if (!fin.fail())
        {
            fin >> highscore;
            while (fin >> scorefromfile)
            {
                if (scorefromfile > highscore)
                {
                    highscore = scorefromfile;
                }
            }
            fin.close();
        }
        else
        {
            fileopenedforread = false;
        }
        sf::RenderWindow gameOver(sf::VideoMode({800, 600}), "Block Buster - Game Over");
        // settings for game over window
        sf::Text gameOverText(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(60);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition({220.f, 150.f});

        sf::Text scoreText(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(40);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition({260.f, 280.f});

        sf::Text highscoreText(font);
        highscoreText.setString("High Score: " + std::to_string(highscore));
        highscoreText.setCharacterSize(40);
        highscoreText.setFillColor(sf::Color::White);
        highscoreText.setPosition({260.f, 380.f});

        sf::Text filefailedtext(font);
        filefailedtext.setString("Could not retrieve High score!");
        filefailedtext.setCharacterSize(40);
        filefailedtext.setFillColor(sf::Color::White);
        filefailedtext.setPosition({260.f, 380.f});

        sf::Text filefailedtextwrite(font);
        filefailedtextwrite.setString("Could not update score in score records!");
        filefailedtextwrite.setCharacterSize(40);
        filefailedtextwrite.setFillColor(sf::Color::White);
        filefailedtextwrite.setPosition({260.f, 430.f});

        sf::Text replay(font);
        replay.setString("Press 'R' to Replay!");
        replay.setCharacterSize(40);
        replay.setFillColor(sf::Color::White);
        replay.setPosition({260.f, 480.f});

        // after game is over , display the game over window;
        while (gameOver.isOpen())
        {
           
            while (const std::optional event = gameOver.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    gameOver.close();
                    playAgain = false;
                }
                
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            {
                gameOver.close();
                playAgain=true;
            }

            gameOver.clear(sf::Color::Black);

            gameOver.draw(backgroundSpriteforend);

            gameOver.draw(gameOverText);

            gameOver.draw(scoreText);

            gameOver.draw(replay);
            if (!fileopenedforwrite)
            {
                gameOver.draw(filefailedtext);
            }
            if (fileopenedforread)
            {
                gameOver.draw(highscoreText);
            }
            else
            {
                gameOver.draw(filefailedtext);
            }
            gameOver.display();
        }
    }

    return 0;
}
