#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

using namespace sf;

// Grid dimensions
const int ROWS = 20;
const int COLS = 10;
const int TILE_SIZE = 30;

// BACKEND PF GRID: 0 matlab khali space, baki numbers matlab filled blocks
int gameGrid[ROWS][COLS] = { 0 };

// SHAPES MATRIX: 1 matlab block mojood hai, 0 matlab khali hai
int shapes[7][4][4] = {
    // 0: I-Shape
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    // 1: Z-Shape
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // 2: S-Shape
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // 3: T-Shape
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // 4: L-Shape
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // 5: J-Shape
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // 6: O-Shape
    {
        {1,1,0,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

// Colors for shapes
Color shapeColors[7] = {
    Color::Cyan, Color::Red, Color::Green,
    Color(128, 0, 128), Color(255, 165, 0),
    Color::Blue, Color::Yellow
};

// Current falling shape details
int currentShape[4][4];
int currentX = 3; // Center top position
int currentY = 0;
int currentType = 0;

// Game states
float gameTimer = 0;
float dropDelay = 0.3;
bool isGameOver = false;

// Nayi shape pick kar ke copy karne ka function
void spawnNewShape() {
    currentType = rand() % 7;
    currentX = 3; // Top center starting point
    currentY = 0;

    // Simple PF concept: Aik array se data doosri array mein copy karna
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            currentShape[r][c] = shapes[currentType][r][c];
        }
    }
}

// COLLISION CHECK: Simple array boundary aur grid state check logic
bool isValidMove(int targetX, int targetY, int tempShape[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (tempShape[r][c] == 1) { // Agar shape ka block mojood hai
                int nextX = targetX + c;
                int nextY = targetY + r;

                // Boundary Checks (Diwaron se bahar na jaye)
                if (nextX < 0 || nextX >= COLS || nextY >= ROWS) {
                    return false;
                }
                // Grid Overlap Check (Pehle se mojood blocks se na takraye)
                if (nextY >= 0 && gameGrid[nextY][nextX] != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

// ROTATION LOGIC: 2D Array Matrix Transpose aur Reverse ka simple PF concept
void rotateShape() {
    int rotated[4][4] = { 0 };

    // Standard Matrix Rotation concept (Rows ko Columns banana)
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            rotated[c][3 - r] = currentShape[r][c];
        }
    }

    // Agar move valid hai toh original shape ko update karo
    if (isValidMove(currentX, currentY, rotated)) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                currentShape[r][c] = rotated[r][c];
            }
        }
    }
}

int main() {
    srand(time(0));

    // Window setup (Score area khatam kar ke width choti kar di taake sirf grid dikhe)
    RenderWindow window(VideoMode(COLS * TILE_SIZE, ROWS * TILE_SIZE), "Tetris - Pure PF Project");

    // FIXED BACKGROUND IMAGE LOGIC
    Texture bgTexture;
    bgTexture.loadFromFile("images/background.jpg");
    Sprite backgroundSprite(bgTexture);
    
    // Scaling background to fit window size
    Vector2u bgSize = bgTexture.getSize();
    backgroundSprite.setScale((float)(COLS * TILE_SIZE) / bgSize.x, (float)(ROWS * TILE_SIZE) / bgSize.y);

    // SFML Rectangle Setup (Drawing ke liye single mold/template)
    RectangleShape block(Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));
    block.setOutlineThickness(1);
    block.setOutlineColor(Color(50, 50, 50));

    RectangleShape emptyCell(Vector2f(TILE_SIZE, TILE_SIZE));
    emptyCell.setFillColor(Color::Transparent);
    emptyCell.setOutlineThickness(1);
    emptyCell.setOutlineColor(Color(255, 255, 255, 40)); // Grid Lines

    Clock clock;
    spawnNewShape();

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        gameTimer += time;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed && !isGameOver) {
                if (event.key.code == Keyboard::Up) {
                    rotateShape(); // Up arrow se rotate
                }
                else if (event.key.code == Keyboard::Left) {
                    if (isValidMove(currentX - 1, currentY, currentShape)) currentX--;
                }
                else if (event.key.code == Keyboard::Right) {
                    if (isValidMove(currentX + 1, currentY, currentShape)) currentX++;
                }
            }
        }

        // Fast Down Logic
        if (Keyboard::isKeyPressed(Keyboard::Down)) dropDelay = 0.05;
        else dropDelay = 0.3;

        // AUTOMATIC FALLING LOGIC
        if (!isGameOver && gameTimer > dropDelay) {
            if (isValidMove(currentX, currentY + 1, currentShape)) {
                currentY++; // Niche move karo
            } else {
                // Agar niche nahi ja sakta, toh grid mein lock kar do
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        if (currentShape[r][c] == 1) {
                            gameGrid[currentY + r][currentX + c] = currentType + 1;
                        }
                    }
                }

                // LINE CLEARING: Row check and shift logic
                for (int r = ROWS - 1; r >= 0; r--) {
                    int count = 0;
                    for (int c = 0; c < COLS; c++) {
                        if (gameGrid[r][c] != 0) count++;
                    }
                    if (count == COLS) {
                        for (int k = r; k > 0; k--) {
                            for (int c = 0; c < COLS; c++) {
                                gameGrid[k][c] = gameGrid[k - 1][c];
                            }
                        }
                        r++; // Recheck row
                    }
                }

                // Spawn next piece
                spawnNewShape();

                // Game Over Check
                if (!isValidMove(currentX, currentY, currentShape)) {
                    isGameOver = true;
                }
            }
            gameTimer = 0;
        }

        // --- DRAWING SECTION ---
        window.clear();

        // 1. Draw Fixed Background First
        window.draw(backgroundSprite);

        // 2. Draw Grid Layout and Locked Blocks based on Array Logic
        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {
                if (gameGrid[r][c] == 0) {
                    emptyCell.setPosition(c * TILE_SIZE, r * TILE_SIZE);
                    window.draw(emptyCell);
                } else {
                    block.setFillColor(shapeColors[gameGrid[r][c] - 1]);
                    block.setPosition(c * TILE_SIZE, r * TILE_SIZE);
                    window.draw(block);
                }
            }
        }

        // 3. Draw Active Falling Shape
        if (!isGameOver) {
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    if (currentShape[r][c] == 1) {
                        block.setFillColor(shapeColors[currentType]);
                        block.setPosition((currentX + c) * TILE_SIZE, (currentY + r) * TILE_SIZE);
                        window.draw(block);
                    }
                }
            }
        }

        window.display();
    }
    return 0;
}
