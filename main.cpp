#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib> // rand() aur srand() ke liye (Random shapes lane ke liye)
#include <ctime>   // time() ke liye

const int ROW_COUNT = 20;
const int COL_COUNT = 10;
const int BLOCK_SIZE = 30; 

// Hamara gameBoard ab sirf 0 ya 1 nahi track karega, balki Color ID track karega (0=Empty, 1-7 = Shape Colors)
int gameBoard[ROW_COUNT][COL_COUNT] = {0};
int score = 0;
bool isGameOver = false; // Game over status track karne ke liye

// 7 Tetris Shapes ki 3D Definition (Pure PF Logic)
int SHAPES[7][4][4] = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // I
    {{0,1,1,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, // O
    {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, // T
    {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}, // S
    {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}}, // Z
    {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}, // J
    {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}  // L
};

// Colors Array: Index matching with Shape ID
sf::Color COLORS[8] = {
    sf::Color::Black,        // 0: Empty space
    sf::Color::Cyan,         // 1: I Shape
    sf::Color::Yellow,       // 2: O Shape
    sf::Color::Magenta,      // 3: T Shape
    sf::Color::Green,        // 4: S Shape
    sf::Color::Red,          // 5: Z Shape
    sf::Color::Blue,         // 6: J Shape
    sf::Color(255, 165, 0)   // 7: L Shape (Orange)
};

// Active piece variables
int currentShape[4][4];
int currentX = 3; // Horizontal position of 4x4 matrix
int currentY = 0; // Vertical position of 4x4 matrix
int currentColorID = 1;

// Nayi Random Shape select karne ka function
void spawnNewPiece() {
    int randomIdx = rand() % 7; // 0 se 6 tak random number
    currentColorID = randomIdx + 1; // Color ID 1 se 7 tak hogi
    
    // 4x4 matrix mein random shape copy karein
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            currentShape[r][c] = SHAPES[randomIdx][r][c];
        }
    }
    
    // Position reset karein
    currentX = 3;
    currentY = 0;

    // Game Over Check: Agar piece spawn hote hi collision ho jaye
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (currentShape[r][c] != 0) {
                if (gameBoard[currentY + r][currentX + c] != 0) {
                    isGameOver = true; // Board upar tak bhar chuka hai
                }
            }
        }
    }
}

// Collision Check Function
bool checkCollision(int nextX, int nextY) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (currentShape[r][c] != 0) {
                int boardX = nextX + c;
                int boardY = nextY + r;

                // Deewaron ya floor se baahir check karein
                if (boardX < 0 || boardX >= COL_COUNT || boardY >= ROW_COUNT) {
                    return true; 
                }

                // Locked blocks se takraav check karein
                if (boardY >= 0 && gameBoard[boardY][boardX] != 0) {
                    return true;
                }
            }
        }
    }
    return false; // Koi collision nahi hai
}

// Line clear karne ka function
void checkAndClearLines() {
    for (int r = ROW_COUNT - 1; r >= 0; r--) {
        bool isLineFull = true;
        for (int c = 0; c < COL_COUNT; c++) {
            if (gameBoard[r][c] == 0) {
                isLineFull = false;
                break;
            }
        }
        if (isLineFull) {
            score += 100;
            std::cout << "Line Cleared! Score: " << score << std::endl;
            for (int r2 = r; r2 > 0; r2--) {
                for (int c = 0; c < COL_COUNT; c++) {
                    gameBoard[r2][c] = gameBoard[r2 - 1][c];
                }
            }
            for (int c = 0; c < COL_COUNT; c++) gameBoard[0][c] = 0;
            r++; 
        }
    }
}

int main() {
    srand(time(0)); // Random seed generate karne ke liye
    
    sf::RenderWindow window(sf::VideoMode(COL_COUNT * BLOCK_SIZE, ROW_COUNT * BLOCK_SIZE), "Tetris - Colorful Grid Project");
    window.setFramerateLimit(60);

    sf::RectangleShape blockShape(sf::Vector2f(BLOCK_SIZE - 2, BLOCK_SIZE - 2)); 
    
    // Grid Lines draw karne ke liye ek rectangle shape
    sf::RectangleShape gridLine(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    gridLine.setFillColor(sf::Color::Black);
    gridLine.setOutlineThickness(1);
    gridLine.setOutlineColor(sf::Color(40, 40, 40)); // Dark grey grid lines

    sf::Clock gravityClock;
    float timer = 0.0f;
    float normalDelay = 0.7f; // Thoda fast speed behtar game experience ke liye
    float currentDelay = normalDelay;

    // Pehla piece spawn karein
    spawnNewPiece();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!isGameOver && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    if (!checkCollision(currentX - 1, currentY)) currentX--;
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    if (!checkCollision(currentX + 1, currentY)) currentX++;
                }
            }
        }

        if (!isGameOver) {
            // Soft Drop Input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                currentDelay = 0.04f; 
            } else {
                currentDelay = normalDelay; 
            }

            // Gravity Logic
            float time = gravityClock.restart().asSeconds();
            timer += time;

            if (timer >= currentDelay) {
                if (!checkCollision(currentX, currentY + 1)) {
                    currentY++; 
                } 
                else {
                    // Lock active piece into gameBoard array with its Color ID
                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 4; c++) {
                            if (currentShape[r][c] != 0) {
                                gameBoard[currentY + r][currentX + c] = currentColorID;
                            }
                        }
                    }

                    checkAndClearLines();
                    spawnNewPiece(); // Naya colorful shape layen
                }
                timer = 0.0f; 
            }
        }

        // --- DRAWING SECTION ---
        window.clear(sf::Color::Black); 

        // 1. Colourful Background Grid Lines draw karein
        for (int r = 0; r < ROW_COUNT; r++) {
            for (int c = 0; c < COL_COUNT; c++) {
                gridLine.setPosition(c * BLOCK_SIZE, r * BLOCK_SIZE);
                window.draw(gridLine);
            }
        }

        // 2. Locked Blocks Draw karein (With respective saved colors)
        for (int r = 0; r < ROW_COUNT; r++) {
            for (int c = 0; c < COL_COUNT; c++) {
                if (gameBoard[r][c] != 0) {
                    blockShape.setPosition(c * BLOCK_SIZE, r * BLOCK_SIZE);
                    blockShape.setFillColor(COLORS[gameBoard[r][c]]); 
                    window.draw(blockShape);
                }
            }
        }

        // 3. Active Falling Shape Draw karein
        if (!isGameOver) {
            blockShape.setFillColor(COLORS[currentColorID]); 
            for (int r = 0; r < 4; r++) {
                for (int c = 0; c < 4; c++) {
                    if (currentShape[r][c] != 0) {
                        blockShape.setPosition((currentX + c) * BLOCK_SIZE, (currentY + r) * BLOCK_SIZE);
                        window.draw(blockShape);
                    }
                }
            }
        } else {
            // Agar Game Over ho jaye to screen par visual feedback (Pura board dark red tint de sakte hain)
            sf::RectangleShape gameOverOverlay(sf::Vector2f(COL_COUNT * BLOCK_SIZE, ROW_COUNT * BLOCK_SIZE));
            gameOverOverlay.setFillColor(sf::Color(150, 0, 0, 100)); // Transparent Red
            window.draw(gameOverOverlay);
        }

        window.display(); 
    }

    return 0;
}