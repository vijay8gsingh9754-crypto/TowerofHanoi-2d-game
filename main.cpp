
#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int SW = 900;
const int SH = 650;

vector<int> pegs[3];

int totalDisks = 3;
int selected = -1;
int selectedPeg = -1;
int moves = 0;

bool playing = false;
bool won = false;

double startTime = 0;
double finalTime = 0;

Vector2 mouseDisk = {0, 0};

int pegX[3] = {180, 450, 720};
int baseY = 500;
int diskH = 25;

// Draw tree
void DrawTree(int x, int y, float s)
{
    DrawRectangle(
        x - (int)(9*s),
        y,
        (int)(18*s),
        (int)(75*s),
        BROWN
    );

    DrawCircle(x, y - 15*s, 38*s, DARKGREEN);
    DrawCircle(x - 22*s, y + 5*s, 27*s, GREEN);
    DrawCircle(x + 22*s, y + 5*s, 27*s, GREEN);
}

// Background
void DrawBackground()
{
    ClearBackground(SKYBLUE);

    DrawCircle(780, 85, 45, YELLOW);

    DrawCircle(150, 100, 25, WHITE);
    DrawCircle(180, 100, 30, WHITE);
    DrawCircle(210, 100, 25, WHITE);

    DrawCircle(420, 130, 23, WHITE);
    DrawCircle(450, 130, 30, WHITE);
    DrawCircle(480, 130, 23, WHITE);

    DrawEllipse(450, 420, 550, 120, GREEN);
    DrawRectangle(0, 440, SW, 210, DARKGREEN);

    DrawTree(60, 370, 1.0f);
    DrawTree(830, 370, 1.0f);
    DrawTree(20, 430, 0.65f);
    DrawTree(880, 430, 0.65f);
}

// Initialize game
void ResetGame()
{
    for (int i = 0; i < 3; i++)
        pegs[i].clear();

    for (int i = totalDisks - 1; i >= 0; i--)
        pegs[0].push_back(i);

    moves = 0;
    selected = -1;
    selectedPeg = -1;

    won = false;
    playing = true;

    startTime = GetTime();
}

// Draw towers and disks
void DrawGame()
{
    DrawBackground();

    // Title
    DrawText("TOWER OF HANOI", 300, 20, 35, DARKBLUE);

    // Level
    DrawText(
        TextFormat("LEVEL: %d", totalDisks),
        35, 25, 23, WHITE
    );

    // Timer
    double elapsed = won ?
        finalTime : GetTime() - startTime;

    DrawText(
        TextFormat("TIME: %.1f sec", elapsed),
        35, 65, 23, WHITE
    );

    DrawText(
        TextFormat("MOVES: %d", moves),
        35, 100, 23, WHITE
    );

    // Towers
    for (int p = 0; p < 3; p++)
    {
        DrawRectangle(
            pegX[p] - 5, 210,
            10, 300, BROWN
        );

        DrawRectangle(
            pegX[p] - 105, baseY,
            210, 15, DARKBROWN
        );

        DrawText(
            TextFormat("PEG %d", p + 1),
            pegX[p] - 35, 530, 22, WHITE
        );

        for (int j = 0; j < (int)pegs[p].size(); j++)
        {
            int disk = pegs[p][j];

            if (p == selectedPeg &&
                j == (int)pegs[p].size() - 1)
                continue;

            int w = 65 + disk * 32;

            int x = pegX[p] - w / 2;
            int y = baseY - (j + 1) * diskH;

            Color colors[] = {
                SKYBLUE,
                ORANGE,
                RED,
                PURPLE
            };

            DrawRectangleRounded(
                {
                    (float)x,
                    (float)y,
                    (float)w,
                    (float)diskH - 2
                },
                0.35f,
                8,
                colors[disk]
            );

            DrawRectangleLinesEx(
                {
                    (float)x,
                    (float)y,
                    (float)w,
                    (float)diskH - 2
                },
                2,
                WHITE
            );
        }
    }

    // Dragging disk
    if (selected != -1)
    {
        int w = 65 + selected * 32;

        DrawRectangleRounded(
            {
                mouseDisk.x - w / 2,
                mouseDisk.y - diskH / 2,
                (float)w,
                (float)diskH - 2
            },
            0.35f,
            8,
            RED
        );
    }

    // Controls
    DrawText("Drag disks using mouse", 325, 585, 20, WHITE);
    DrawText("R = Restart     M = Menu", 325, 615, 18, YELLOW);
}

// Get peg from mouse position
int GetPeg(float x)
{
    if (x < 315) return 0;
    if (x < 585) return 1;
    return 2;
}

// Check victory
bool CheckWin()
{
    return pegs[2].size() == totalDisks;
}

// Main menu
void DrawMenu()
{
    DrawBackground();

    DrawText(
        "TOWER OF HANOI",
        230, 100, 50, DARKBLUE
    );

    DrawText(
        "SELECT YOUR LEVEL",
        320, 220, 25, WHITE
    );

    for (int i = 1; i <= 4; i++)
    {
        Rectangle btn = {
            230.0f + (i-1)*115,
            300,
            90,
            75
        };

        DrawRectangleRounded(
            btn, 0.2f, 8, DARKBLUE
        );

        DrawText(
            TextFormat("%d", i),
            (int)btn.x + 35,
            (int)btn.y + 15,
            40, WHITE
        );

        DrawText(
            TextFormat("%d Disk%s", i, i == 1 ? "" : "s"),
            (int)btn.x + 5,
            (int)btn.y + 85,
            17, WHITE
        );

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
            CheckCollisionPointRec(GetMousePosition(), btn))
        {
            totalDisks = i;
            ResetGame();
        }
    }

    DrawText(
        "Choose 1, 2, 3 or 4 plates",
        300, 450, 25, YELLOW
    );
}

// Win screen
void DrawWin()
{
    DrawRectangle(180, 200, 540, 240, DARKBLUE);

    DrawText(
        "CONGRATULATIONS!",
        265, 230, 35, YELLOW
    );

    DrawText(
        TextFormat("Total Moves: %d", moves),
        330, 295, 25, WHITE
    );

    DrawText(
        TextFormat("Time: %.1f seconds", finalTime),
        330, 335, 25, WHITE
    );

    DrawText(
        "Press R to play again",
        330, 385, 20, GREEN
    );

    DrawText(
        "Press M for Main Menu",
        330, 415, 20, WHITE
    );
}

int main()
{
    InitWindow(SW, SH, "Tower of Hanoi 2D Game");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (playing)
        {
            Vector2 mouse = GetMousePosition();

            if (IsKeyPressed(KEY_R))
                ResetGame();

            if (IsKeyPressed(KEY_M))
            {
                playing = false;
                won = false;
            }

            // Select top disk
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
                && selected == -1)
            {
                int p = GetPeg(mouse.x);

                if (!pegs[p].empty())
                {
                    int disk = pegs[p].back();

                    int w = 65 + disk * 32;

                    int y = baseY -
                        (int)pegs[p].size() * diskH;

                    Rectangle rect = {
                        (float)(pegX[p] - w/2),
                        (float)y,
                        (float)w,
                        (float)diskH
                    };

                    if (CheckCollisionPointRec(mouse, rect))
                    {
                        selected = disk;
                        selectedPeg = p;
                        mouseDisk = mouse;
                    }
                }
            }

            // Drag disk
            if (selected != -1 &&
                IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                mouseDisk = mouse;
            }

            // Release disk
            if (selected != -1 &&
                IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                int destination = GetPeg(mouse.x);

                bool valid = false;

                if (destination != selectedPeg)
                {
                    if (pegs[destination].empty() ||
                        pegs[destination].back() > selected)
                    {
                        valid = true;
                    }
                }

                if (valid)
                {
                    pegs[selectedPeg].pop_back();
                    pegs[destination].push_back(selected);
                    moves++;
                }

                selected = -1;
                selectedPeg = -1;

                if (CheckWin())
                {
                    finalTime = GetTime() - startTime;
                    won = true;
                }
            }
        }

        BeginDrawing();

        if (!playing)
        {
            DrawMenu();
        }
        else
        {
            DrawGame();

            if (won)
                DrawWin();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
