#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <iomanip>
using namespace std;
const int COLS = 10; //keep more than 6
const int ROWS = 11; //keep more than 5
const float cellsize= 50.f;
int *currentblockinitials[12]={0};

void block_1(int arr[][COLS], int rows)
{
    for(int i = 0; i < 12; i++)
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
    int k=0;
    for (int i = 0; i < length_vertical; i++)
    {
        arr[i][start] = 1;
        currentblockinitials[k]=&arr[i][start];
        k++;
        
    }
    for (int j = start + 1; j <= length_horizontal + start; j++)
    {
        arr[0][j] = 1;
        currentblockinitials[k]=&arr[0][j];
        k++;
    }
}
void block_2(int arr[][COLS], int rows)
{
    for(int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    int k=0;
    // 1 1 1
    int length = rand() % 3 + 1;
    int start = rand() % (COLS -length);
    for (int i = start; i < length + start; i++)
    {
        arr[0][i] = 1;
        currentblockinitials[k]=&arr[0][i];
        k++;
    }
}
void block_3(int arr[][COLS], int rows)
{
    for(int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    int k=0;
    // 1
    // 1
    // 1
    int length = rand() % 3 + 1;
    int start = rand() % (COLS - length) ;
    for (int i = 0; i < length; i++)
    {
        arr[i][start] = 1;
        currentblockinitials[k]=&arr[i][start];
        k++;
    }
}
void block_4(int arr[][COLS], int rows)
{
    for(int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    int k=0;
    // 1 0 0
    // 1 0 0
    // 1 0 0
    // 1 1 1
    int length_horizontal = rand() % 3 + 1;
    int length_vertical = rand() % 3 + 1;
    int start = rand() % (COLS -  length_horizontal);
    for (int i = 0; i <= length_vertical; i++)
    {
        arr[i][start] = 1;
        currentblockinitials[k]=&arr[i][start];
        k++;
    }

    for (int j = start + 1; j < length_horizontal + start; j++)
    {
        arr[length_vertical][j] = 1;
        currentblockinitials[k]=&arr[length_vertical][j];
        k++;
    }
}
void block_5(int arr[][COLS], int rows)
{
    for(int i = 0; i < 12; i++)
    {
        currentblockinitials[i] = nullptr;
    }
    // 1 1 0
    // 0 1 1
    int length = 2;
    int start = rand() % (COLS - 2 - length) + 0;
    arr[0][start] = 1;
    currentblockinitials[0]=&arr[0][start];
    arr[0][start + 1]=1;
    currentblockinitials[1]=&arr[0][start+1];
    arr[1][start + 1]=1;
    currentblockinitials[2]=&arr[1][start+1];
    arr[1][start + 2]=1;
    currentblockinitials[3]=&arr[1][start+2];
}
int main()
{
    sf::Font font;
    bool gameOverFlag = false;
    if (!font.openFromFile("ARIALI.TTF"))
    {
        return -1;
    }
    bool activeBlock = false;
    int score = 0;
    srand(time(0));
    
    sf::RectangleShape rect(sf::Vector2f(50.f, 50.f));
    rect.setFillColor(sf::Color::Red);
    int arr[ROWS][COLS] = {0};
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Block Buster");
    sf::RectangleShape gridcell(sf::Vector2f(cellsize-2,cellsize-2));
    gridcell.setFillColor(sf::Color::Blue);
    

    int row = 0;
    auto lastSpawn = std::chrono::steady_clock::now();
    auto lastGravity = std::chrono::steady_clock::now();
    while (window.isOpen()&& gameOverFlag==false)
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

        if(std::chrono::duration<float>(now-lastMove).count() > 0.15f)
        { 
        
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                bool canMoveLeft = true;

                for(int k=0;k<12;k++)
                {
                    if(currentblockinitials[k]!=nullptr)
                    {
                        int col = (currentblockinitials[k] - &arr[0][0]) % COLS;

                        if(col == 0)
                        {
                            canMoveLeft = false;
                            break;
                        }

                        if(*(currentblockinitials[k]-1) == 1)
                        {
                            bool ownBlock=false;

                            for(int l=0;l<12;l++)
                            {
                                if(currentblockinitials[l] ==currentblockinitials[k]-1)
                                {
                                    ownBlock=true;
                                    break;
                                }
                            }

                            if(!ownBlock)
                            {
                                canMoveLeft=false;
                                break;
                            }
                        }
                    }
                }
                if(canMoveLeft)
                {
                    for(int k=0;k<12;k++)
                    {
                        if(currentblockinitials[k]!=nullptr)
                            *currentblockinitials[k]=0;
                    }

                    for(int k=0;k<12;k++)
                    {
                        if(currentblockinitials[k]!=nullptr)
                            currentblockinitials[k]--;
                    }

                    for(int k=0;k<12;k++)
                    {
                        if(currentblockinitials[k]!=nullptr)
                            *currentblockinitials[k]=1;
                    }
                }
                lastMove=now;
            }
        
        
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                bool canMoveRight = true;

                for(int k=0;k<12;k++)
                {
                    if(currentblockinitials[k]!=nullptr)
                    {
                        int col = (currentblockinitials[k] - &arr[0][0]) % COLS;

                        if(col == COLS-1)
                        {
                            canMoveRight = false;
                            break;
                        }

                        if(*(currentblockinitials[k]+1) == 1)
                        {
                            bool ownBlock=false;

                            for(int l=0;l<12;l++)
                            {
                                if(currentblockinitials[l] ==
                                currentblockinitials[k]+1)
                                {
                                    ownBlock=true;
                                    break;
                                }
                            }

                            if(!ownBlock)
                            {
                                canMoveRight=false;
                                break;
                            }
                        }
                    }
                }
                if(canMoveRight)
                {
                    for(int k=0;k<12;k++)
                    {
                        if(currentblockinitials[k]!=nullptr)
                            *currentblockinitials[k]=0;
                    }

                    for(int k=0;k<12;k++)
                    {
                        if(currentblockinitials[k]!=nullptr)
                            currentblockinitials[k]++;
                    }

                    for(int k=0;k<12;k++)
                    {
                        if(currentblockinitials[k]!=nullptr)
                            *currentblockinitials[k]=1;
                    }
                }
                lastMove=now;
            }          
        }
        
        now = std::chrono::steady_clock::now();

        std::chrono::duration<float> spawnElapsed = now - lastSpawn;

        if(!activeBlock)
        {
            row++;
            lastSpawn = now;
            int newblock = rand() % 5 + 1;
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
            else
            {
                block_5(arr, ROWS);
            }

            activeBlock = true;
        }
        float gravitySpeed = 1.0f; //gravity working after each 1 second
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            gravitySpeed = 0.5f; // 2x faster if down key presssed
        }
                    
        std::chrono::duration<float> gravityElapsed = now - lastGravity;
        
        if(gravityElapsed.count() >= gravitySpeed)//if (time of gravity passes )
        {
            lastGravity=now;
            bool canMove = true;
            for(int k=0;k<12;k++)
            {
                if(currentblockinitials[k]!=nullptr)
                {
                    int row = (currentblockinitials[k] - &arr[0][0])/ COLS;
                    if(row == ROWS-1)
                    {
                        canMove = false;
                    }
                    else if(*(currentblockinitials[k]+COLS)==1)
                    {
                        canMove=false;
                        for(int l=0;l<12;l++)
                        {
                            if(currentblockinitials[k]+COLS==currentblockinitials[l])
                            {
                                canMove=true;
                            }
                        }
                    }
                }
            }
            if(!canMove)
            {
                //check for more space to end game: 
                for(int j=0;j<COLS;j++)
                {
                    if(arr[0][j]==1)
                    {
                        gameOverFlag = true;
                    }
                }
            }
            if(!canMove)
            {
                activeBlock=false;
                if(!canMove)
                {
                    activeBlock=false;

                    for(int k=0;k<12;k++)
                    {
                        currentblockinitials[k]=nullptr;
                    }
                }
                //Line Clearing: 
                for(int row=ROWS-1; row>=0; row--)
                {
                    bool full=true;

                    for(int col=0; col<COLS; col++)
                    {
                        if(arr[row][col]==0)
                        {
                            full=false;
                            break;
                        }
                    }

                    if(full)
                    {
                        for(int r=row; r>0; r--)
                        {
                            for(int c=0; c<COLS; c++)
                            {
                                arr[r][c]=arr[r-1][c];
                            }
                        }
                        for(int c=0;c<COLS;c++)
                        {
                            arr[0][c]=0;
                        }
                        score += 100;   
                        row++; 
                    }
                }
            }
            else
            {
                for(int k=0;k<12;k++)
                {
                    if(currentblockinitials[k]!=nullptr)
                    {
                        *currentblockinitials[k]=0;
                    }
                }
                for(int k=0;k<12;k++)
                {
                    if(currentblockinitials[k]!=nullptr)
                    {
                        currentblockinitials[k]+=COLS;
                    }
                }
                for(int k=0;k<12;k++)
                {
                    if(currentblockinitials[k]!=nullptr)
                    {
                        *currentblockinitials[k]=1;
                    }
                }
            }
            
        }        
        window.clear();
        for(int i=1;i<ROWS+1;i++)
        {
            for(int j=3;j<COLS+3;j++)
            {
                gridcell.setPosition(sf::Vector2f(j*cellsize,i*cellsize));
                window.draw(gridcell);
            }
        }
        for(int i=1;i<ROWS+1;i++)
        {
            for(int j=3;j<COLS+3;j++)
            {
                
                if(arr[i-1][j-3]==1)
                {
                    rect.setPosition(sf::Vector2f(j*cellsize,i*cellsize));
                    window.draw(rect);
                }  

                
            }
        }
        auto lastUpdateforgravity = std::chrono::steady_clock::now();
        sf::Text scoreDisplay(font);
        scoreDisplay.setString("Score: " + std::to_string(score));
        window.draw(scoreDisplay);
        window.display();
        
    } 
    
    sf::RenderWindow gameOver(sf::VideoMode({800, 600}),"Block Buster - Game Over");

    sf::Text gameOverText(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({220.f, 150.f});

    sf::Text scoreText(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({280.f, 280.f});

    while (gameOver.isOpen())
    {
        while (const std::optional event = gameOver.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                gameOver.close();
        }

        gameOver.clear(sf::Color::Black);

        gameOver.draw(gameOverText);
        gameOver.draw(scoreText);

        gameOver.display();
    }
    return 0;
}
