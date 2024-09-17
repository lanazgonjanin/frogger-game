#include <unistd.h>  // used in kbhit()
#include <termios.h> // used in kbhit()
#include <fcntl.h>   // used in kbhit()
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

// Function prototypes (Foward declared)
void initGame();
// Declared all functions here, implementation is after int main()
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
        // (counter % 5 == 0) return either 0 or 1
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
    // The initial position of "F" (Frog) in lane9
    // When frogY == 0 the game is over
    frogX = WIDTH / 2;
    frogY = HEIGHT - 1;
}

// printing the board
void draw()
{
    // cleans the previous prints
    system("clear");

    // printing the position of Frog (frogX and frogY) with char 'F' and lanes[][]
    
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

// updating frogY and frogX based on user input from kbhit()
void input()
{
    if (kbhit())
    {
        switch (getchar())
        {
            // updating the poistion of frog (frogY/frogX) based on inputs from kbhit()
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
    // if (counter % 5 == 0) then updateLanes = 1
    // and if updateLanes = 1 then:
    // First: for i = 0; i < HEIGHT call function updateLane(i)
    // Second: Check win condition (If frogY == 0 you win then exit(0))
    // Note: The collision condition was already checked in function input()
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
    // lanes[lane] is updated based on speeds[lane]
    // (moving left or right based on their speed)
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

// receiving inputs from keyboard
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
