#include <raylib.h>
#include <iostream>

const int NUM_ROWS = 6;
const int NUM_COLS = 7;
const int CELL_SIZE = 80;

class ConnectFourGame
{
public:
    ConnectFourGame();
    void Play();

    Rectangle playButton;
    Color playButtonColor;

    Rectangle restartButton;
    Color restartButtonColor;

private:
    std::vector<std::vector<int>> board;
    int currentPlayer;
    bool gameStarted;
    bool gameFinished;
    int winner;

    void UpdateStartScreen();
    void DrawStartScreen();
    void Update();
    void Draw();
    void UpdateWinner();
    void DrawWinner();
    bool CanPlaceToken(int col) const;
    void PlaceToken(int col);
    bool CheckWin() const;
    bool IsBoardFull() const;
    void ResetGame();
};

ConnectFourGame::ConnectFourGame()
{
    board.resize(NUM_COLS, std::vector<int>(NUM_ROWS, 0));
    currentPlayer = 1;
    gameStarted = false;
}

void ConnectFourGame::Play()
{
    while (!WindowShouldClose())
    {
        if (!gameStarted)
        {
            UpdateStartScreen();
            DrawStartScreen();
        }
        else
        {
            if (!gameFinished)
            {
                Update();
                Draw();
            }
            else
            {
                UpdateWinner();
                DrawWinner();
            }
        }
    }
}

void ConnectFourGame::UpdateStartScreen()
{
    if (CheckCollisionPointRec(GetMousePosition(), playButton))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            gameStarted = true;
        }
        // highlight the button when the mouse is hovering over it
        playButtonColor = GRAY;
    }
    else
    {
        playButtonColor = DARKGRAY;
    }
}

void ConnectFourGame::DrawStartScreen()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("Connect Four", GetScreenWidth() / 2 - MeasureText("Connect Four", 40) / 2, 100, 40, BLACK);

    DrawRectangleRec(playButton, playButtonColor);
    DrawText("Play", playButton.x + playButton.width / 2 - MeasureText("Play", 30) / 2,
             playButton.y + playButton.height / 2 - 30 / 2, 30, WHITE);

    EndDrawing();
}

void ConnectFourGame::Update()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        int col = GetMouseX() / CELL_SIZE;
        if (col >= 0 && col < NUM_COLS && CanPlaceToken(col))
        {
            PlaceToken(col);
            if (CheckWin())
            {
                std::cout << "Player " << currentPlayer << " wins!" << std::endl;
                gameFinished = true;
            }
            else if (IsBoardFull())
            {
                std::cout << "It's a tie!" << std::endl;
                ResetGame();
            }
            else
            {
                currentPlayer = 3 - currentPlayer;
            }
        }
    }
}

void ConnectFourGame::Draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int col = 0; col < NUM_COLS; col++)
    {
        for (int row = 0; row < NUM_ROWS; row++)
        {
            int x = col * CELL_SIZE;
            int y = (NUM_ROWS - row - 1) * CELL_SIZE;
            int cellValue = board[col][row];
            Color color = cellValue == 0 ? LIGHTGRAY : cellValue == 1 ? RED
                                                                      : BLUE;
            DrawCircle(x + CELL_SIZE / 2, y + CELL_SIZE / 2, CELL_SIZE * 0.4, color);
        }
    }

    EndDrawing();
}

void ConnectFourGame::UpdateWinner()
{
    if (CheckCollisionPointRec(GetMousePosition(), restartButton))
    {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            gameStarted = false;
            gameFinished = false;
            ResetGame();
        }
        restartButtonColor = GRAY;
    }
    else
    {
        restartButtonColor = DARKGRAY;
    }
}

void ConnectFourGame::DrawWinner()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    std::string message = (winner == 0) ? "It's a tie!" : "Player " + std::to_string(currentPlayer) + " wins!";
    DrawText(message.c_str(), GetScreenWidth() / 2 - MeasureText(message.c_str(), 40) / 2,
             GetScreenHeight() / 2 - 40 / 2, 40, BLACK);

    DrawRectangleRec(restartButton, restartButtonColor);
    DrawText("Restart", restartButton.x + restartButton.width / 2 - MeasureText("Restart", 30) / 2,
             restartButton.y + restartButton.height / 2 - 30 / 2, 30, WHITE);

    EndDrawing();
}

bool ConnectFourGame::CanPlaceToken(int col) const
{
    return board[col][NUM_ROWS - 1] == 0;
}

void ConnectFourGame::PlaceToken(int col)
{
    for (int row = 0; row < NUM_ROWS; row++)
    {
        if (board[col][row] == 0)
        {
            board[col][row] = currentPlayer;
            break;
        }
    }
}

bool ConnectFourGame::CheckWin() const
{
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};

    for (int row = 0; row < NUM_ROWS; ++row)
    {
        for (int col = 0; col < NUM_COLS; ++col)
        {
            int currentPlayerToken = board[col][row];
            if (currentPlayerToken == 0)
                continue;

            for (const auto &dir : directions)
            {
                int dx = dir[0];
                int dy = dir[1];

                int count = 1; // Count of consecutive tokens

                // Check consecutive tokens in the current direction
                for (int i = 1; i < 4; ++i)
                {
                    int nextCol = col + dx * i;
                    int nextRow = row + dy * i;

                    if (nextCol >= 0 && nextCol < NUM_COLS &&
                        nextRow >= 0 && nextRow < NUM_ROWS &&
                        board[nextCol][nextRow] == currentPlayerToken)
                    {
                        count++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (count == 4)
                    return true;
            }
        }
    }

    return false;
}

bool ConnectFourGame::IsBoardFull() const
{
    for (int col = 0; col < NUM_COLS; col++)
    {
        if (board[col][NUM_ROWS - 1] == 0)
        {
            return false;
        }
    }
    return true;
}

void ConnectFourGame::ResetGame()
{
    for (auto &col : board)
    {
        for (auto &cell : col)
        {
            cell = 0;
        }
    }

    currentPlayer = 1;
}

int main(int argc, char **argv)
{
    ConnectFourGame game;

    InitWindow(NUM_COLS * CELL_SIZE, NUM_ROWS * CELL_SIZE, "Connect Four");

    SetTargetFPS(60);

    game.playButton = {(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 50, 200, 100};
    game.playButtonColor = DARKGRAY;

    game.restartButton = {(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 50, 200, 100};
    game.restartButtonColor = DARKGRAY;

    game.Play();

    CloseWindow();

    return 0;
}