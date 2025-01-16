#include <unistd.h>
#include <termios.h>
#include <fcntl.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Game constants
#define WIDTH 64
#define HEIGHT 10

// Game global variables
int frogX, frogY;
char lanes[HEIGHT][WIDTH + 1] = {
    "xxx..xxx..xxx..xxxxxxxxxxxxxxx..xxxxxxxxxxxxxxxxxxxxxxxxx..xxxxx", // lane0 (wall/end lane)
    "...xxxx..xxxxxx.......xxxx.....xx...xxxx.....xxxxxx...xxxxx.....", // lane1
    "....xxxx.....xxxx.....xxxx.......xxxxxxx.....xx....xxxxxx.......", // lane2
    "..xxx.....xxx.....xxx.....xxx...xxx....xx....xxxx....xx......xx.", // lane3
    "................................................................", // lane4 (safe lane)
    "....xxxx.......xxxx.........xxxx.......xxxx......xxxx....xxxx...", // lane5
    ".....xx...xx...xx......xx....xx.......xx..xx.xx......xx.......xx", // lane6
    "..xxx.....xx......xxxx..xx......xxxx......xxxx.......xxx...xxx..", // lane7
    "..xx.....xx.......xx.....xx.....xx..xx.xx........xx....xx.......", // lane8
    "................................................................"  // lane9 (start lane)
};
int speeds[HEIGHT] = {0, -2, +1, -1, 0, +2, -1, -1, +1, 0};

void initGame();
void draw();
void input();
void logic(int updateLanes);
void updateLane(int lane);
int kbhit(void);


int main()
{
    initGame();
    int counter = 0;
    while (1)
    {
        input();
        logic(counter % 5 == 0); // Update lanes every 5th iteration (t seconds)
        draw();
        usleep(200000); // Sleep for t/5 seconds (200 milliseconds)
        counter++;
    }
    return 0;
}

int i,j;

void initGame()
{
    frogX = WIDTH / 2;
    frogY = HEIGHT - 1;
}

void draw()
{
    system("clear");

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (j == frogX && i == frogY) 
            {
                printf("F");
            }
            else
            {
                printf("%c",lanes[i][j]);
            } 
        }
        printf("\n");
    }
    

    printf("Use W, A, S, D to move. Reach the top to win!\n");
            
    if (lanes[frogY][frogX] == 'x')
    {
        printf("Game over!\n");
        exit(0);
    }
}

void input()
{
    if (kbhit())
    {
        switch (getchar())
        {
            case 'w':
                if (frogY - 1 >= 0)
                {
                   frogY = frogY - 1; 
                }
                break;
            case 'a':
                if (frogX - 1 >= 0)
                {
                    frogX = frogX - 1;
                } 
                break;
            case 's':
                if (frogY + 1 < HEIGHT)
                {
                   frogY = frogY + 1; 
                }
                break;
            case 'd':
                if (frogX + 1 < WIDTH)
                {
                    frogX = frogX + 1;
                } 
                break;
            case 'q':
                printf("The typed key: q\n");
                printf("Do you want to quit? (y/n) then press Enter: ");
                kbhit();
                switch (getchar())
                {
                case 'y':
                    exit(0);
                case 'n':
                    break;
                }
                break;
        }

    }
}

void logic(int updateLanes)
{
    if (updateLanes == 1)
    {
        for (i = 0; i < HEIGHT; i++)
        {
            updateLane(i);
        }
    }

    if (frogY == 0)
    {
        draw();
        printf("You win!\n");
        exit(0);
    }
}

void updateLane(int lane)
{
    int k;

    for (k = 0; k < HEIGHT; k++)
    {
        int s = speeds[k];
        char newlane[WIDTH+1]; 
        newlane[WIDTH] = '\0';

        if (s < 0)
        {
            strncpy(newlane,lanes[k]+abs(s),WIDTH-abs(s));
            strncpy(newlane+WIDTH-abs(s),lanes[k],abs(s));
            strcpy(lanes[k],newlane);
        }
        else if (s > 0)
        {
            strncpy(newlane,lanes[k]+WIDTH-s,s);
            strncpy(newlane+s,lanes[k],WIDTH-s);
            strcpy(lanes[k],newlane);
        }
    }
    
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
