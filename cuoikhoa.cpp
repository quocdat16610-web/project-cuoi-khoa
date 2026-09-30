#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Point
{
    int x;
    int y;
};
vector<Point> snake;
Point food;

Direction dir = RIGHT;

bool gameOver = false;

int score = 0;
void gotoXY(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord
    );
}

void spawnFood()
{
    while (true)
    {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;

        bool trung = false;
        for (Point p : snake)
        {
            if (p.x == food.x && p.y == food.y)
            {
                trung = true;
                break;
            }
        }

        if (!trung)
            break;
    }
}

void setup()
{
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    snake.push_back({WIDTH / 2 - 1, HEIGHT / 2});
    snake.push_back({WIDTH / 2 - 2, HEIGHT / 2});

    dir = RIGHT;

    score = 0;
    gameOver = false;

    spawnFood();
}

void draw()
{
    gotoXY(0, 0);
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";

    cout << '\n';
    for (int y = 0; y < HEIGHT; y++)
    {
        cout << "#";

        for (int x = 0; x < WIDTH; x++)
        {
            bool printed = false;
            for (int i = 0; i < snake.size(); i++)
            {
                if (snake[i].x == x &&
                    snake[i].y == y)
                {
                    if (i == 0)
                        cout << "O";
                    else
                        cout << "o";

                    printed = true;
                    break;
                }
            }
            if (!printed &&
                food.x == x &&
                food.y == y)
            {
                cout << "@";
                printed = true;
            }

            if (!printed)
                cout << " ";
        }

        cout << "#\n";
    }
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";

    cout << "\nScore: " << score << "     ";
}


void input()
{
    if (_kbhit())
    {
        char key = _getch();
        if ((key == 'w' || key == 'W') && dir != DOWN)
        {
            dir = UP;
        }

        else if ((key == 's' || key == 'S') && dir != UP)
        {
            dir = DOWN;
        }

        else if ((key == 'a' || key == 'A') && dir != RIGHT)
        {
            dir = LEFT;
        }

        else if ((key == 'd' || key == 'D') && dir != LEFT)
        {
            dir = RIGHT;
        }

        else if (key == 27)
        {
            gameOver = true;
        }
    }
}

void update()
{
    Point newHead = snake[0];

    if (dir == UP)
        newHead.y--;

    else if (dir == DOWN)
        newHead.y++;

    else if (dir == LEFT)
        newHead.x--;

    else if (dir == RIGHT)
        newHead.x++;


    if (newHead.x < 0 ||
        newHead.x >= WIDTH ||
        newHead.y < 0 ||
        newHead.y >= HEIGHT)
    {
        gameOver = true;
        return;
    }


    for (int i = 0; i < snake.size(); i++)
    {
        if (newHead.x == snake[i].x &&
            newHead.y == snake[i].y)
        {
            gameOver = true;
            return;
        }
    }

    snake.insert(snake.begin(), newHead);


    if (newHead.x == food.x &&
        newHead.y == food.y)
    {
        score += 10;


        spawnFood();
    }
    else
    {
        snake.pop_back();
    }
}

int main()
{
    srand(time(NULL));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hConsole, &cursorInfo);

    cursorInfo.bVisible = false;

    SetConsoleCursorInfo(hConsole, &cursorInfo);

    setup();

    draw();


    while (!gameOver)
    {
        input();

        update();

        draw();
        Sleep(100);
    }


    gotoXY(0, HEIGHT + 3);

    cout << "=========================\n";
    cout << "       GAME OVER!\n";
    cout << "       Score: " << score << "\n";
    cout << "=========================\n";

    cout << "\nNhan phim bat ky de thoat...";

    _getch();

    return 0;
}
