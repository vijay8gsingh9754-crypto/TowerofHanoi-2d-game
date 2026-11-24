
#include "raylib.h"
#include <cmath>

int main()
{
    InitWindow(800, 600, "Tower of Hanoi Game");
    SetTargetFPS(90);

    int peg[3] = {0, 0, 0};
    int width[3] = {80, 120, 160};
    int x[3] = {150, 400, 650};

    int selected = -1;
    int moves = 0;

    Vector2 diskPos[3];

    // Game states
    bool menu = true;
    bool playing = false;
    bool paused = false;
    bool gameOver = false;

    Rectangle startBtn = {300, 220, 200, 50};
    Rectangle resumeBtn = {300, 290, 200, 50};
    Rectangle stopBtn = {680, 20, 90, 40};
    Rectangle exitBtn = {300, 360, 200, 50};

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        // MAIN MENU
        if (menu)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mouse, startBtn))
                {
                    peg[0] = 0;
                    peg[1] = 0;
                    peg[2] = 0;
                    moves = 0;
                    selected = -1;
                    paused = false;
                    gameOver = false;
                    menu = false;
                    playing = true;
                }

                if (CheckCollisionPointRec(mouse, exitBtn))
                    break;
            }
        }

        // GAME CONTROLS
        else if (playing)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(mouse, stopBtn))
            {
                paused = true;
                selected = -1;
            }

            if (!paused)
            {
                // Calculate disk positions
                for (int i = 0; i < 3; i++)
                {
                    int level = 0;

                    for (int j = 0; j < 3; j++)
                        if (j != i && peg[j] == peg[i] && j > i)
                            level++;

                    diskPos[i] = {
                        (float)x[peg[i]],
                        (float)(450 - level * 35)
                    };
                }

                // Select top disk
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    for (int i = 0; i < 3; i++)
                    {
                        bool top = true;

                        for (int j = 0; j < i; j++)
                            if (peg[i] == peg[j])
                                top = false;

                        if (top && CheckCollisionPointRec(
                            mouse,
                            {diskPos[i].x - width[i]/2,
                             diskPos[i].y - 15,
                             (float)width[i], 30}))
                        {
                            selected = i;
                            break;
                        }
                    }
                }

                // Release and move disk
                if (selected != -1 &&
                    IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    int destination = mouse.x < 275 ? 0 :
                                      mouse.x < 525 ? 1 : 2;

                    bool valid = true;

                    for (int j = 0; j < selected; j++)
                        if (peg[j] == destination)
                            valid = false;

                    if (valid && destination != peg[selected])
                    {
                        peg[selected] = destination;
                        moves++;
                    }

                    selected = -1;
                }
            }

            // Resume button
            if (paused &&
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mouse, resumeBtn))
                    paused = false;

                if (CheckCollisionPointRec(mouse, exitBtn))
                    menu = true, playing = false;
            }
        }

        // DRAWING
        BeginDrawing();
        ClearBackground({245, 248, 255, 255});

        // MAIN MENU SCREEN
        if (menu)
        {
            DrawText("TOWER OF HANOI", 205, 100, 40, DARKBLUE);
            DrawText("3 DISK PUZZLE GAME", 275, 160, 22, GRAY);

            DrawRectangleRec(startBtn, GREEN);
            DrawText("START GAME", 330, 233, 25, WHITE);

            DrawRectangleRec(exitBtn, RED);
            DrawText("EXIT GAME", 340, 373, 25, WHITE);
        }

        // GAME SCREEN
        else if (playing)
        {
            DrawText("TOWER OF HANOI", 250, 25, 32, DARKBLUE);

            DrawText(TextFormat("MOVES: %d", moves),
                     30, 90, 22, DARKGRAY);

            DrawRectangleRec(stopBtn, ORANGE);
            DrawText("STOP", 697, 30, 20, WHITE);

            // Towers
            DrawRectangle(50, 480, 700, 15, DARKBROWN);

            for (int i = 0; i < 3; i++)
            {
                DrawRectangle(x[i] - 5, 180, 10, 300, GRAY);

                DrawText(TextFormat("PEG %c", 'A' + i),
                         x[i] - 35, 510, 20, DARKGRAY);
            }

            // Calculate positions for drawing
            for (int i = 0; i < 3; i++)
            {
                int level = 0;

                for (int j = 0; j < 3; j++)
                    if (j != i && peg[j] == peg[i] && j > i)
                        level++;

                diskPos[i] = {
                    (float)x[peg[i]],
                    (float)(450 - level * 35)
                };
            }

            // Draw disks
            for (int i = 2; i >= 0; i--)
            {
                if (i == selected && !paused)
                    continue;

                DrawRectangleRounded(
                    {diskPos[i].x - width[i]/2,
                     diskPos[i].y - 15,
                     (float)width[i], 30},
                    0.3f, 8,
                    i == 0 ? SKYBLUE :
                    i == 1 ? ORANGE : RED);
            }

            // Dragging disk
            if (selected != -1 && !paused)
            {
                DrawRectangleRounded(
                    {mouse.x - width[selected]/2,
                     mouse.y - 15,
                     (float)width[selected], 30},
                    0.3f, 8,
                    selected == 0 ? SKYBLUE :
                    selected == 1 ? ORANGE : RED);
            }

            // Win condition
            if (peg[0] == 2 && peg[1] == 2 && peg[2] == 2)
            {
                DrawText("YOU ARE AWESOME!", 270, 550, 30, GREEN);
                DrawText("PUZZLE COMPLETED!", 300, 125, 25, GREEN);
            }

            DrawText("Drag the top disk to another tower",
                     220, 570, 18, DARKGRAY);

            // PAUSE SCREEN
            if (paused)
            {
                DrawRectangle(0, 0, 800, 600,
                              Fade(BLACK, 0.65f));

                DrawText("GAME PAUSED", 280, 180,
                         40, YELLOW);

                DrawRectangleRec(resumeBtn, GREEN);
                DrawText("RESUME", 345, 303, 25, WHITE);

                DrawRectangleRec(exitBtn, RED);
                DrawText("MAIN MENU", 330, 373, 25, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
