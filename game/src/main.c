#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 900
#define BALL_RADIUS 10
#define NUM_BALLS 20
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 60
#define NUM_BRICKS_X 16
#define NUM_BRICKS_Y 16

typedef struct {
    Vector2 position;
    Vector2 direction;
    Vector2 cursorPos;
    float radius;
    bool is_clicked;
    Color color;
    float ball_speed;
} Ball;

typedef struct {
    Rectangle rect;
    int durability;
    Color color;
    bool isActive;
} Brick;

Ball balls[NUM_BALLS];
Brick bricks[NUM_BRICKS_X][NUM_BRICKS_Y];

bool allowNewClick = true;
int ballResetCounter = 0;

bool gameStarted = false;
bool levelCompleted = false;
bool retryScreen = false;
bool continueScreen = false;
bool gameOverScreen = false;

Rectangle level1ButtonRec = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 200, 40 };
Rectangle level2ButtonRec = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 200, 40 };
Rectangle continueButtonRec = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 40, 200, 40 };
Rectangle retryButtonRec = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 200, 40 };
Rectangle exitButtonRec = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 100, 200, 40 };


int currentLevel = 1;

void InitBricks1() {
    Rectangle brick_rects[8] = {
            {148, 56, BRICK_WIDTH, BRICK_HEIGHT},
            {453, 117, BRICK_WIDTH, BRICK_HEIGHT},
            {331, 178, BRICK_WIDTH, BRICK_HEIGHT},
            {392, 178, BRICK_WIDTH, BRICK_HEIGHT},
            {209, 239, BRICK_WIDTH, BRICK_HEIGHT},
            {270, 239, BRICK_WIDTH, BRICK_HEIGHT},
            {87, 300, BRICK_WIDTH, BRICK_HEIGHT},
            {148, 300, BRICK_WIDTH, BRICK_HEIGHT}
    };

    int durability_values[8] = {
            45, 40, 30, 35, 20, 25, 10, 15
    };

    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            bricks[i][j].isActive = false;
        }
    }

    int brick_index = 0;
    for (int i = 0; i < 8; i++) {
        int x = brick_rects[i].x / BRICK_WIDTH;
        int y = brick_rects[i].y / BRICK_HEIGHT;
        bricks[x][y].rect = brick_rects[i];
        bricks[x][y].durability = durability_values[i];
        bricks[x][y].color = PURPLE;
        bricks[x][y].isActive = true;
    }
}

void InitBricks2() {
    Rectangle brick_rects[16] = {
        {280, 200, BRICK_WIDTH, BRICK_HEIGHT}, {345, 135, BRICK_WIDTH, BRICK_HEIGHT},
        {410, 105, BRICK_WIDTH, BRICK_HEIGHT}, {475, 135, BRICK_WIDTH, BRICK_HEIGHT},
        {510, 200, BRICK_WIDTH, BRICK_HEIGHT}, {475, 265, BRICK_WIDTH, BRICK_HEIGHT},
        {410, 335, BRICK_WIDTH, BRICK_HEIGHT}, {345, 400, BRICK_WIDTH, BRICK_HEIGHT},
        {280, 460, BRICK_WIDTH, BRICK_HEIGHT}, {215, 400, BRICK_WIDTH, BRICK_HEIGHT},
        {150, 335, BRICK_WIDTH, BRICK_HEIGHT}, {85, 270, BRICK_WIDTH, BRICK_HEIGHT},
        {55, 205, BRICK_WIDTH, BRICK_HEIGHT}, {85, 140, BRICK_WIDTH, BRICK_HEIGHT},
        {150, 105, BRICK_WIDTH, BRICK_HEIGHT}, {215, 135, BRICK_WIDTH, BRICK_HEIGHT}
    };

    int durability_values[16] = {
        20, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 20, 20, 20, 20, 20
    };

    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            bricks[i][j].isActive = false;
        }
    }

    int brick_index = 0;
    for (int i = 0; i < 16; i++) {
        int x = brick_rects[i].x / BRICK_WIDTH;
        int y = brick_rects[i].y / BRICK_HEIGHT;
        bricks[x][y].rect = brick_rects[i];
        bricks[x][y].durability = durability_values[i];
        bricks[x][y].color = PURPLE;
        bricks[x][y].isActive = true;
    }
}

void DrawBricks() {
    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            if (bricks[i][j].isActive) {
                DrawRectangleRec(bricks[i][j].rect, bricks[i][j].color);

                DrawText(TextFormat("%d", bricks[i][j].durability),
                    bricks[i][j].rect.x + BRICK_WIDTH / 2 - MeasureText(TextFormat("%d", bricks[i][j].durability), 10) / 2,
                    bricks[i][j].rect.y + BRICK_HEIGHT / 2 - 5, 10, WHITE);

            }
        }
    }
}

void UpdateBricks() {
    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            if (bricks[i][j].isActive) {
                for (int b = 0; b < NUM_BALLS; b++) {
                    if (CheckCollisionCircleRec(balls[b].position, BALL_RADIUS, bricks[i][j].rect)) {
                        bricks[i][j].durability--;
                        if (bricks[i][j].durability <= 0) {
                            bricks[i][j].isActive = false;
                        }
                    }
                }
            }
        }
    }
}

void ShiftBricksDown() {
    for (int i = NUM_BRICKS_Y - 1; i >= 0; i--) {
        for (int j = 0; j < NUM_BRICKS_X; j++) {
            if (bricks[j][i].isActive) {
                bricks[j][i].rect.y += BRICK_HEIGHT;
            }
        }
    }
}

void BallReset(Ball* balls) {
    balls->position = (Vector2){ 300, 750 };
    balls->direction = (Vector2){ 0,0 };
    balls->is_clicked = false;
    balls->ball_speed = 300.0f;
    ballResetCounter++;
    if (ballResetCounter >= NUM_BALLS) {
        allowNewClick = true;
        ballResetCounter = 0;
        ShiftBricksDown();
    }
}

void InitBalls(Ball* balls) {
    for (int i = 0; i < NUM_BALLS; i++) {
        balls[i].position = (Vector2){ SCREEN_WIDTH / 2,750 };
        balls->is_clicked = false;
        balls[i].radius = BALL_RADIUS;
        balls[i].color = WHITE;
        balls[i].ball_speed = 300.0f;
    }
}

void DrawBalls(Ball* balls) {
    for (int i = 0; i < NUM_BALLS; i++) {
        DrawCircleV(balls[i].position, balls[i].radius, balls[i].color);
    }
}

void UpdateBalls(Ball* balls) {

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && allowNewClick) {

        for (int i = 0; i < NUM_BALLS; i++) {
            if (!balls[i].is_clicked) {
                balls[i].is_clicked = true;
                balls[i].cursorPos = GetMousePosition();


                balls[i].direction.x = balls[i].cursorPos.x - balls[i].position.x;
                balls[i].direction.y = balls[i].cursorPos.y - balls[i].position.y;

                float magnitude = sqrt(balls[i].direction.x * balls[i].direction.x + balls[i].direction.y * balls[i].direction.y);
                if (magnitude != 0) {
                    balls[i].direction.x /= magnitude;
                    balls[i].direction.y /= magnitude;
                }

                balls[i].direction.x *= balls[i].ball_speed;
                balls[i].direction.y *= balls[i].ball_speed;
            }
            allowNewClick = false;
        }
    }

    for (int i = 0; i < NUM_BALLS; i++) {
        if (balls[i].is_clicked) {
            balls[i].position.x += balls[i].direction.x * GetFrameTime();
            balls[i].position.y += balls[i].direction.y * GetFrameTime();


            if (balls[i].position.x <= 0 || balls[i].position.x >= GetScreenWidth() - BALL_RADIUS * 2) {
                balls[i].direction.x *= -1;
            }

            if (balls[i].position.y <= 0) {
                balls[i].direction.y *= -1;
            }


            for (int j = 0; j < NUM_BRICKS_X; j++) {
                for (int k = 0; k < NUM_BRICKS_Y; k++) {
                    if (bricks[j][k].isActive) {
                        Rectangle brickRect = bricks[j][k].rect;
                        if (CheckCollisionCircleRec(balls[i].position, balls[i].radius, brickRect)) {

                            bricks[j][k].durability--;
                            if (bricks[j][k].durability <= 0) {
                                bricks[j][k].isActive = false;
                            }

                            if (balls[i].position.x <= brickRect.x || balls[i].position.x >= brickRect.x + brickRect.width) {
                                balls[i].direction.x *= -1;
                            }
                            if (balls[i].position.y <= brickRect.y || balls[i].position.y >= brickRect.y + brickRect.height) {
                                balls[i].direction.y *= -1;
                            }
                        }
                    }
                }
            }

            if (balls[i].position.y >= GetScreenHeight() - 150) {
                BallReset(&balls[i]);
            }
        }
    }
}

void DrawStartPage() {
    DrawText("Brick-Breaker Hit", SCREEN_WIDTH / 2 - MeasureText("Brick-Breaker Hit", 30) / 2, SCREEN_HEIGHT / 4, 30, WHITE);
    DrawRectangleRec(level1ButtonRec, BLUE);
    DrawText("LEVEL 1", level1ButtonRec.x + 70, level1ButtonRec.y + 10, 20, WHITE);
    DrawRectangleRec(level2ButtonRec, BLUE);
    DrawText("LEVEL 2", level2ButtonRec.x + 70, level2ButtonRec.y + 10, 20, WHITE);
}

void StartGame() {
    gameStarted = true;
    levelCompleted = false;
    retryScreen = false;
    continueScreen = false;
    gameOverScreen = false;
}

void StartNextLevel() {
    currentLevel++;
    for (int i = 0; i < NUM_BALLS; i++) {
        BallReset(&balls[i]);
    }
    gameStarted = true;
}

void DrawRetryScreen() {
    DrawText("You lost! Retry?", SCREEN_WIDTH / 2 - MeasureText("You lost! Retry?", 30) / 2, SCREEN_HEIGHT / 4, 30, WHITE);
    DrawRectangleRec(retryButtonRec, RED);
    DrawText("Retry", retryButtonRec.x + 70, retryButtonRec.y + 10, 20, WHITE);
}

void DrawContinueScreen() {
    DrawText("Level completed! Continue?", SCREEN_WIDTH / 2 - MeasureText("Level completed! Continue?", 30) / 2, SCREEN_HEIGHT / 4, 30, WHITE);
    DrawRectangleRec(continueButtonRec, GREEN);
    DrawText("Continue", continueButtonRec.x + 50, continueButtonRec.y + 10, 20, WHITE);
}

void DrawGameOverScreen() {
    DrawText("Game Over!", SCREEN_WIDTH / 2 - MeasureText("Game Over!", 30) / 2, SCREEN_HEIGHT / 4, 30, WHITE);
    DrawRectangleRec(exitButtonRec, RED);
    DrawText("Exit", exitButtonRec.x + 70, exitButtonRec.y + 10, 20, WHITE);
}


bool CheckBrickCollisionWithLine() {
    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            if (bricks[i][j].isActive) {
                if (CheckCollisionRecs(bricks[i][j].rect, (Rectangle) { 0, 760, SCREEN_WIDTH, 2 })) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool CheckAllBricksDestroyed() {
    for (int i = 0; i < NUM_BRICKS_X; i++) {
        for (int j = 0; j < NUM_BRICKS_Y; j++) {
            if (bricks[i][j].isActive) {
                return false;
            }
        }
    }
    return true;
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Brick Breaker");

    for (int i = 0; i < NUM_BALLS; i++) {
        InitBalls(&balls[i]);
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!gameStarted) {
            if (!levelCompleted) {
                if (!retryScreen && !continueScreen && !gameOverScreen) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        Vector2 mousePoint = GetMousePosition();
                        if (CheckCollisionPointRec(mousePoint, level1ButtonRec)) {
                            currentLevel = 1;
                            StartGame();
                            InitBricks1();
                        }
                        else if (CheckCollisionPointRec(mousePoint, level2ButtonRec)) {
                            currentLevel = 2;
                            StartGame();
                            InitBricks2();
                        }
                    }
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawStartPage();
                    EndDrawing();
                }
                else if (retryScreen) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        Vector2 mousePoint = GetMousePosition();
                        if (CheckCollisionPointRec(mousePoint, retryButtonRec)) {
                            retryScreen = false;
                            if (currentLevel == 1) {
                                InitBricks1();
                            }
                            else {
                                InitBricks2();
                            }
                            StartGame();
                        }
                    }
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawRetryScreen();
                    EndDrawing();
                }
                else if (continueScreen) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        Vector2 mousePoint = GetMousePosition();
                        if (CheckCollisionPointRec(mousePoint, continueButtonRec)) {
                            continueScreen = false;
                            if (currentLevel == 1) {
                                InitBricks2();
                            }
                            StartNextLevel();
                        }
                    }
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawContinueScreen();
                    EndDrawing();
                }
                else if (gameOverScreen) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        Vector2 mousePoint = GetMousePosition();
                        if (CheckCollisionPointRec(mousePoint, exitButtonRec)) {
                            CloseWindow();
                        }
                    }
                    BeginDrawing();
                    ClearBackground(BLACK);
                    DrawGameOverScreen();
                    EndDrawing();
                }
            }
        }
        else {
            if (CheckBrickCollisionWithLine()) {
                gameStarted = false;
                retryScreen = true;
                BeginDrawing();
                ClearBackground(BLACK);
                DrawRetryScreen();
                EndDrawing();
            }
            else if (CheckAllBricksDestroyed()) {
                gameStarted = false;
                if (currentLevel == 2) {
                    gameOverScreen = true;
                }
                else {
                    continueScreen = true;
                }
                BeginDrawing();
                ClearBackground(BLACK);
                if (currentLevel == 2) {
                    DrawGameOverScreen();
                }
                else {
                    DrawContinueScreen();
                }
                EndDrawing();
            }
            else {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawLine(0, 760, 600, 760, RED);
                DrawBricks();
                for (int i = 0; i < NUM_BALLS; i++) {
                    UpdateBalls(&balls[i]);
                    DrawBalls(&balls[i]);
                }
                UpdateBricks();
                EndDrawing();
            }
        }
    }

    CloseWindow();

    return 0;
}