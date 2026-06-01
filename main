#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>

using namespace sf;
using namespace std;

// Grid dimensions
const int ROWS = 20;
const int COLS = 10;
const int TILE_SIZE = 30;

// Backend PF Logic Grid
int grid[ROWS][COLS] = { 0 };

// Standard Shapes (Relative coordinates)
int shapes[7][4] = {
    1,3, 5,7, // 0: I (Cyan)
    2,4, 5,7, // 1: Z (Red)
    3,5, 4,6, // 2: S (Green)
    3,5, 4,7, // 3: T (Purple)
    2,3, 5,7, // 4: L (Orange)
    3,5, 7,6, // 5: J (Blue)
    2,3, 4,5, // 6: O (Yellow)
};

// Colors for the 7 shapes
Color colors[7] = {
    Color::Cyan,
    Color::Red,
    Color::Green,
    Color(128, 0, 128), // Purple
    Color(255, 165, 0), // Orange
    Color::Blue,
    Color::Yellow
};

struct Point { int x, y; } falling_shape_points[4], backup_points[4];

// Global Variables
float timer = 0, delay = 0.3;
int score = 0;
bool gameOver = false;

// Nayi shape spawn karna
void spawnNewShape(int& dx, int& n) {
    n = rand() % 7;
    for (int i = 0; i < 4; i++) {
        falling_shape_points[i].x = shapes[n][i] % 2 + 4; // Centered at top
        falling_shape_points[i].y = shapes[n][i] / 2;
    }
    dx = 0;
}

// Collision Check Function
bool check() {
    for (int i = 0; i < 4; i++) {
        if (falling_shape_points[i].x < 0 || falling_shape_points[i].x >= COLS || falling_shape_points[i].y >= ROWS) {
            return false; // Boundary check
        }
        else if (grid[falling_shape_points[i].y][falling_shape_points[i].x] != 0) {
            return false; // Collision with already placed blocks
        }
    }
    return true;
}

int main() {
    srand(time(0));
    
    // Window create ki (Grid + Score Area)
    RenderWindow window(VideoMode(COLS * TILE_SIZE + 150, ROWS * TILE_SIZE), "Tetris - Final PF Project");

    // Background Setup
    Texture bg_tex;
    bg_tex.loadFromFile("images/background.jpg"); 
    Sprite s_bg(bg_tex);

    // BACKGROUND SCALING LOGIC
    Vector2u textureSize = bg_tex.getSize(); 
    float scaleX = (float)(COLS * TILE_SIZE + 150) / textureSize.x;
    float scaleY = (float)(ROWS * TILE_SIZE) / textureSize.y;
    s_bg.setScale(scaleX, scaleY);

    // Font Setup
    Font font;
    font.loadFromFile("arial.ttf"); 
    
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(COLS * TILE_SIZE + 20, 20);

    Text gameOverText("GAME OVER!", font, 30);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition((COLS * TILE_SIZE) / 2 - 60, (ROWS * TILE_SIZE) / 2 - 20);

    // Rectangle Setup (Filled Blocks ke liye)
    RectangleShape rectangle(Vector2f(TILE_SIZE - 1, TILE_SIZE - 1));
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(Color(50, 50, 50)); 

    // --- NAYA HISSA: Empty Grid Box Setup ---
    RectangleShape emptyCell(Vector2f(TILE_SIZE, TILE_SIZE));
    emptyCell.setFillColor(Color::Transparent); // Andar se khali taake background nazar aye
    emptyCell.setOutlineThickness(1);
    emptyCell.setOutlineColor(Color(255, 255, 255, 60)); // Halki si safaid (White) outline
    // ----------------------------------------

    int dx = 0;
    bool rotate_request = false;
    int n_shape_type = 0;
    Clock clock;

    spawnNewShape(dx, n_shape_type);

    // Main Game Loop
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed && !gameOver) {
                if (e.key.code == Keyboard::Up) rotate_request = true;
                else if (e.key.code == Keyboard::Left) dx = -1;
                else if (e.key.code == Keyboard::Right) dx = 1;
            }
        }

        if (!gameOver) {
            if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;
            else delay = 0.3;

            // Movement
            if (dx != 0) {
                for (int i = 0; i < 4; i++) backup_points[i] = falling_shape_points[i];
                for (int i = 0; i < 4; i++) falling_shape_points[i].x += dx;
                if (!check()) {
                    for (int i = 0; i < 4; i++) falling_shape_points[i] = backup_points[i];
                }
                dx = 0;
            }

            // Rotation
            if (rotate_request) {
                for (int i = 0; i < 4; i++) backup_points[i] = falling_shape_points[i];
                Point p = falling_shape_points[1];
                for (int i = 0; i < 4; i++) {
                    int original_x = falling_shape_points[i].x - p.x;
                    int original_y = falling_shape_points[i].y - p.y;
                    falling_shape_points[i].x = -original_y + p.x;
                    falling_shape_points[i].y = original_x + p.y;
                }
                if (!check()) {
                    for (int i = 0; i < 4; i++) falling_shape_points[i] = backup_points[i];
                }
                rotate_request = false;
            }

            // Falling & Locking
            if (timer > delay) {
                for (int i = 0; i < 4; i++) backup_points[i] = falling_shape_points[i];
                for (int i = 0; i < 4; i++) falling_shape_points[i].y += 1;

                if (!check()) {
                    for (int i = 0; i < 4; i++) {
                        grid[backup_points[i].y][backup_points[i].x] = n_shape_type + 1;
                    }

                    for (int row = ROWS - 1; row >= 0; row--) {
                        int col_count = 0;
                        for (int col = 0; col < COLS; col++) {
                            if (grid[row][col] != 0) col_count++;
                        }
                        if (col_count == COLS) {
                            score += 100;
                            for (int k = row; k > 0; k--) {
                                for (int col = 0; col < COLS; col++) {
                                    grid[k][col] = grid[k - 1][col];
                                }
                            }
                            row++; 
                        }
                    }

                    spawnNewShape(dx, n_shape_type);
                    
                    if (!check()) {
                        gameOver = true;
                    }
                }
                timer = 0;
            }
        }

        // --- DRAWING SECTION ---
        window.clear(Color::Black); 

        window.draw(s_bg);

        // Grid Draw Loop (Ab Empty aur Filled dono draw honge)
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (grid[i][j] == 0) {
                    // Agar jagah khali hai, toh transparent grid box draw karo
                    emptyCell.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                    window.draw(emptyCell);
                }
                else { 
                    // Agar block hai, toh color wala rectangle draw karo
                    rectangle.setFillColor(colors[grid[i][j] - 1]);
                    rectangle.setPosition(j * TILE_SIZE, i * TILE_SIZE);
                    window.draw(rectangle);
                }
            }
        }

        // Draw Active Falling Block
        if (!gameOver) {
            for (int i = 0; i < 4; i++) {
                rectangle.setFillColor(colors[n_shape_type]);
                rectangle.setPosition(falling_shape_points[i].x * TILE_SIZE, falling_shape_points[i].y * TILE_SIZE);
                window.draw(rectangle);
            }
        }

        scoreText.setString("Score:\n" + to_string(score));
        window.draw(scoreText);

        if (gameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }
    return 0;
}
