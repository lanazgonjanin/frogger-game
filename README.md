# Frogger Game

A terminal-based implementation of the classic **Frogger** game written in C.

The player controls a frog using keyboard input and must navigate through moving obstacles to reach the top of the game board without colliding with an obstacle.

## Features

* Real-time keyboard input using `W`, `A`, `S`, and `D`
* Moving obstacles with different directions and speeds
* Collision detection
* Win and game-over conditions
* Option to quit during gameplay
* Dynamic terminal-based game board

## Technologies

* **Language:** C
* **Libraries & APIs:** `termios`, `fcntl`, `unistd`, `stdio`, `stdlib`, `string`

## How to Run

### Requirements

* GCC
* A Unix-based terminal environment such as macOS or Linux

### 1. Clone the repository

```bash
git clone git@github.com:lanazgonjanin/frogger-game.git
cd frogger-game
```

### 2. Compile the program

```bash
gcc -o FroggerGame frogger.c
```

### 3. Run the game

```bash
./FroggerGame
```

## Controls

| Key | Action     |
| --- | ---------- |
| `W` | Move up    |
| `A` | Move left  |
| `S` | Move down  |
| `D` | Move right |
| `Q` | Quit       |

Reach the top of the board to win while avoiding the moving obstacles.

## Technical Concepts

* C programming
* Arrays and character manipulation
* Game-state management
* Keyboard input and terminal control
* Collision detection
* Iterative game loops
* Dynamic game board updates
* Terminal and system-level programming
