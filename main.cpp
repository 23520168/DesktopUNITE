#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

class SnakeGame 
{
private:
    bool gameOver;
    const int width = 60;
    const int height = 25;
    int x, y, fruitX, fruitY, score, highScore;
    int tailX[100], tailY[100];
    int nTail;
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;
    HANDLE hConsole;

public:
    SnakeGame();
    void HideCursor();
    void ShowCursor();
    void DrawBackground();
    void DrawStartScreen();
    void StartGame();
    void Draw();
    void Input();
    void Logic();
    void RunGame();
    void ResetGame();
};

SnakeGame::SnakeGame() 
{
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    highScore = 0;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
}

void SnakeGame::HideCursor() 
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}

void SnakeGame::ShowCursor() 
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 10;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &info);
}

void SnakeGame::DrawBackground() 
{
    system("cls");

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    cout << "       ---_ ......._-_--." << endl;
    cout << "      (|\\ /      / /| \\  \\" << endl;
    cout << "      /  /     .'  -=-'   `." << endl;
    cout << "     /  /    .'             )" << endl;
    cout << "   _/  /   .'        _.)   /" << endl;
    cout << "  / o   o        _.-' /  .'" << endl;
    cout << "  \\          _.-'    / .'*|" << endl;
    cout << "   \\______.-'//    .'.' \\*|" << endl;
    cout << "    \\|  \\ | //   .'.' _ |*|" << endl;
    cout << "     `   \\|//  .'.'_ _ _|*|" << endl;
    cout << "      .  .// .'.' | _ _ \\*|" << endl;
    cout << "      \\`-|\\_/ /    \\ _ _ \\*\\" << endl;
    cout << "       `/'\\__/      \\ _ _ \\*\\" << endl;
    cout << "      /^|            \\ _ _ \\*" << endl;
    cout << "     '  `             \\ _ _ \\     " << endl;
    cout << "                       \\_" << endl;
    cout << " ..   ..  ..   ..  .. ______  ______                      " << endl;
    cout << " ||   ||  |\\\\  ||  || ------ |/                   " << endl;
    cout << " ||   ||  ||\\\\ ||  ||   ||   |______             " << endl;
    cout << " ||   ||  || \\\\||  ||   ||   ||             " << endl;
    cout << " \\====//  ||  \\||  ||   ||   |\\_____          " << endl;
}

void SnakeGame::DrawStartScreen() 
{
    DrawBackground();

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    cout << "  ================================" << endl;
    cout << "  ||        Snake Game         ||" << endl;
    cout << "  ================================" << endl;
    cout << "  ||                           ||" << endl;
    cout << "  ||       Press 's' to        ||" << endl;
    cout << "  ||        Start Game         ||" << endl;
    cout << "  ||                           ||" << endl;
    cout << "  ================================" << endl;
}

void SnakeGame::StartGame() 
{
    DrawStartScreen();
    char startKey = _getch();
    if (startKey == 's') {
        gameOver = false;
        ResetGame();
    }
    else
        gameOver = true;
}

void SnakeGame::ResetGame() 
{
    score = 0;
    x = width / 2;
    y = height / 2;
    nTail = 0;
    fruitX = rand() % width;
    fruitY = rand() % height;
}

void SnakeGame::Draw() 
{
    COORD pos;
    pos.X = 0;
    pos.Y = 0;
    SetConsoleCursorPosition(hConsole, pos);

    for (int i = 0; i < width + 2; i++)
        cout << "\xB2";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) 
        {
            if (j == 0)
                cout << "\xB2";
            if ((i == y && j == x)) 
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                cout << "\xDB";
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }
            else if (i == fruitY && j == fruitX)
                cout << "0";
            else 
            {
                bool print = false;
                for (int k = 0; k < nTail; k++) 
                {
                    if (tailX[k] == j && tailY[k] == i) 
                    {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        cout << "\xDB";
                        print = true;
                    }
                }
                if (!print)
                    cout << " ";
            }

            if (j == width - 1)
                cout << "\xB2";
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "\xB2";
    cout << endl;

    cout << "Score:" << score << "   High Score:" << highScore << "            " << endl;
    cout << "Press W A S D to move, X to quit" << "          " << endl;
    if (gameOver) 
    {
        cout << "Game Over! Press any key to continue." << endl;
    }
}

void SnakeGame::Input() 
{
    if (_kbhit()) 
    {
        switch (_getch()) 
        {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

void SnakeGame::Logic() 
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) 
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) 
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) 
    {
        score += 10;
        if (score > highScore)
            highScore = score;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

void SnakeGame::RunGame() 
{
    while (true) 
    {
        StartGame();
        while (!gameOver) 
        {
            Draw();
            Input();
            Logic();
            Sleep(100);
        }
        _getch(); 
    }
    ShowCursor();
}

int main() {
    srand(static_cast<unsigned>(time(nullptr))); 
    SnakeGame game;
    game.RunGame();
    return 0;
}
